"""
    Runners — moteur générique (indépendant du modèle)

Niveau L2 : la mécanique de résolution est séparée de la définition du modèle.
Un *modèle* est un module fournissant la convention de fonctions :
  nclasses(p), init_classes(p,N,rng), prevalence_classes(p),
  rates(g)::Tuple, execute!(g,k),
  init_u0(p), meanfield_rhs!(du,u,p,t), u_prevalence(u,p),
  param_specs(), state_color(c), state_names()

`g::GState` est l'état générique de la simulation individu-centrée (un entier de
« classe » par nœud + pools par classe pour échantillonner/compter en O(1)).
"""
module Runners

using Graphs
using OrdinaryDiffEq
using Random: Xoshiro

export GState, set_class!, count_class, nodes_of, nbr_count, wchoose, pick_neighbor
export run_ssa, run_ode, frontier
export lattice_graph, smallworld_graph, empty_graph, tailmean

# --- état générique d'une simulation SSA ---
mutable struct GState
    node_class::Vector{Int}        # classe (1..nclasses) par nœud
    pools::Vector{Vector{Int}}     # liste de nœuds par classe
    pos::Vector{Int}               # position d'un nœud dans son pool
    nbr::Matrix{Int}               # nbr[classe, nœud] = # voisins dans cette classe (réseau)
    graph::SimpleGraph{Int}
    wellmixed::Bool
    params                         # NamedTuple de paramètres du modèle
    t::Float64
    rng
end

@inline count_class(g::GState, c::Int) = length(g.pools[c])
@inline nodes_of(g::GState, c::Int) = g.pools[c]
@inline nbr_count(g::GState, c::Int, node::Int) = g.nbr[c, node]   # # voisins du nœud en classe c

function GState(nclasses::Int, classes::Vector{Int}, graph, wellmixed, params, seed)
    N = length(classes)
    pools = [Int[] for _ in 1:nclasses]
    pos = zeros(Int, N)
    @inbounds for i in 1:N
        c = classes[i]; push!(pools[c], i); pos[i] = length(pools[c])
    end
    nbr = zeros(Int, nclasses, N)
    if !wellmixed
        @inbounds for v in 1:N, w in neighbors(graph, v)
            nbr[classes[v], w] += 1
        end
    end
    return GState(copy(classes), pools, pos, nbr, graph, wellmixed, params, 0.0, Xoshiro(seed))
end

"Change la classe d'un nœud (pools en swap-remove O(1) + maj des compteurs de voisins)."
function set_class!(g::GState, node::Int, newc::Int)
    oldc = g.node_class[node]
    oldc == newc && return
    p = g.pools[oldc]; i = g.pos[node]; lastid = p[end]
    p[i] = lastid; g.pos[lastid] = i; pop!(p)
    push!(g.pools[newc], node); g.pos[node] = length(g.pools[newc])
    g.node_class[node] = newc
    if !g.wellmixed
        @inbounds for w in neighbors(g.graph, node)
            g.nbr[oldc, w] -= 1; g.nbr[newc, w] += 1
        end
    end
    return
end

"Tire un voisin de `node` appartenant à la classe `c` (réservoir, sans allocation). 0 si aucun."
function pick_neighbor(g::GState, node::Int, c::Int)
    chosen = 0; cnt = 0
    @inbounds for w in neighbors(g.graph, node)
        if g.node_class[w] == c
            cnt += 1
            rand(g.rng) * cnt <= 1.0 && (chosen = w)
        end
    end
    return chosen
end

"Choix pondéré d'un index (accepte Tuple ou Vector de poids)."
function wchoose(w, rng)
    tot = sum(w); tot <= 0 && return rand(rng, 1:length(w))
    x = rand(rng) * tot; acc = 0.0
    @inbounds for i in eachindex(w)
        acc += w[i]; x <= acc && return i
    end
    return length(w)
end

# --- constructeurs de graphes ---
lattice_graph(L::Int) = Graphs.grid([L, L])                       # L×L nœuds, degré ~4
smallworld_graph(N::Int, k::Int, beta::Float64, seed::Int) = watts_strogatz(N, k, beta; rng = Xoshiro(seed))
empty_graph(N::Int) = SimpleGraph(N)                              # B (bien mélangé) : graphe inutilisé

function tailmean(x, tail)
    isempty(x) && return 0.0
    i0 = max(1, floor(Int, length(x) * (1 - tail)) + 1)
    return sum(@view x[i0:end]) / (length(x) - i0 + 1)
end

"""
    run_ssa(model, params, graph; wellmixed, tend, out_step, seed, ...)

Gillespie générique. Renvoie `(t, prev, snaps, gstate)`.
Garde-fou : arrêt anticipé si `max_events` ou `max_seconds` dépassés.
"""
function run_ssa(model, params, graph; wellmixed::Bool, tend, out_step, seed,
                 record_spatial::Bool = false,
                 max_events::Int = 20_000_000, max_seconds::Float64 = 20.0)
    classes = model.init_classes(params, nv(graph), Xoshiro(seed))
    g = GState(model.nclasses(params), classes, graph, wellmixed, params, seed + 1)
    prevc = model.prevalence_classes(params)
    prevalence() = sum(count_class(g, c) for c in prevc)

    rec_t = Float64[]; rec_prev = Float64[]; snaps = Vector{Vector{Int}}()
    nextout = 0.0; nev = 0; t0 = time(); truncated = false
    while g.t < tend
        rates = model.rates(g)
        tot = sum(rates); tot <= 0 && break
        g.t += -log(rand(g.rng)) / tot
        while nextout <= g.t && nextout <= tend
            push!(rec_t, nextout); push!(rec_prev, prevalence())
            record_spatial && push!(snaps, copy(g.node_class))
            nextout += out_step
        end
        g.t > tend && break
        model.execute!(g, wchoose(rates, g.rng))
        nev += 1
        if nev > max_events || time() - t0 > max_seconds
            @warn "run_ssa: budget calcul atteint → trajectoire tronquée (baisser ⟨k⟩ ou L)" nev t = round(g.t, digits = 2) tend
            truncated = true
            break
        end
    end
    return (t = rec_t, prev = rec_prev, snaps = snaps, gstate = g, truncated = truncated)
end

"Résolution A — champ moyen déterministe."
function run_ode(model, params; tend, saveat)
    u0 = model.init_u0(params)
    prob = ODEProblem(model.meanfield_rhs!, u0, (0.0, tend), params)
    sol = solve(prob, Rosenbrock23(); saveat = saveat, abstol = 1e-8, reltol = 1e-6)
    return (t = sol.t, prev = [model.u_prevalence(u, params) for u in sol.u], sol = sol)
end

"""
    frontier(model, params; N, degrees, reps, tend, out_step)

Frontière de validité : prévalence d'équilibre de C (réseau small-world de degré k)
rapportée à celle de A (champ moyen), en fonction de k.
"""
function frontier(model, params; N, degrees, ws_beta = 0.1, reps = 4,
                  tend, out_step, seed0 = 1000)
    outA = model.outcome_ode(run_ode(model, params; tend = tend, saveat = out_step), params)
    ks = Float64[]; ratios = Float64[]
    for k in degrees
        vals = Float64[]
        for r in 1:reps
            g = smallworld_graph(N, k, ws_beta, seed0 + 31k + r)
            c = run_ssa(model, params, g; wellmixed = false, tend = tend,
                        out_step = out_step, seed = seed0 + 31k + r)
            push!(vals, model.outcome_ssa(c, params))
        end
        push!(ks, float(k)); push!(ratios, (sum(vals) / length(vals)) / max(outA, 1e-9))
    end
    return (k = ks, ratio = ratios, outA = outA)
end

end # module
