"""
    ResolutionBC

**Résolutions B et C — Gillespie stochastique individu-centré (Agents.jl).**

Même spécification microscopique que A, mais on suit chaque individu. Une seule
différence entre B et C : la **structure de contact**.

- **B (bien mélangé)** : infection/surinfection à partir des comptes globaux
  (`betaSum * S`, etc.). C'est la contrepartie stochastique exacte de A et la
  réplique de la version C++ d'origine → sert de **référence de validation**.
- **C (réseau)** : infection/surinfection le long des **arêtes** du graphe.
  En graphe complet, C se réduit exactement à B (cohérence par construction).

Algorithme : Gillespie en temps continu. À chaque pas on calcule les 6 taux de
canaux, on tire le temps d'attente `Exp(1/total)`, on choisit un canal ∝ taux,
on exécute. Implémentation **claire avant d'être optimisée** (réf. de validation).

⚠ Non testé : Julia absent de la machine au moment de l'écriture. À valider
(B doit reproduire les sorties du C++).
"""
module ResolutionBC

using ..DrugResModel
using Agents
using Graphs
using Random: shuffle!, Xoshiro

export run_B, run_C, build_model, run_abm!

@agent struct Site(GraphAgent)
    state::Int
    treated::Int
    geno::Int
end

# 10 catégories de pools : 1=EMPTY, 2=SUSC, 3..10 = INF[geno,treated]
@inline catid(state, treated, geno) =
    state == EMPTY ? 1 : state == SUSC ? 2 : (3 + treated * 4 + geno)

mutable struct SimState
    p::Params
    graph::SimpleGraph{Int}
    wellmixed::Bool
    t::Float64
    pools::Vector{Vector{Int}}   # id par catégorie
    cat::Vector{Int}             # catégorie courante par id
    posp::Vector{Int}            # position de l'id dans son pool
    nS::Vector{Int}              # # voisins susceptibles, par nœud (réseau)
    nI::Vector{Int}              # # voisins infectés, par nœud (réseau)
    # enregistreur
    rec_t::Vector{Float64}
    rec_S::Vector{Int}
    rec_Itot::Vector{Int}
    rec_Igt::Vector{Array{Int,2}}
    rec_si::Vector{Int}          # # arêtes S-I (réseau), -1 sinon
end

# --- gestion des pools (swap-remove O(1)) ---
function pool_add!(st::SimState, id::Int, c::Int)
    push!(st.pools[c], id)
    st.cat[id] = c
    st.posp[id] = length(st.pools[c])
end
function pool_del!(st::SimState, id::Int)
    c = st.cat[id]; pool = st.pools[c]; i = st.posp[id]
    last = pool[end]
    pool[i] = last; st.posp[last] = i
    pop!(pool)
    st.cat[id] = 0; st.posp[id] = 0
end

# --- comptes dérivés des pools ---
@inline Scount(st) = length(st.pools[2])
@inline Ecount(st) = length(st.pools[1])
@inline icount(st, g, t) = length(st.pools[catid(INF, t, g)])
function Itotal(st)
    n = 0
    @inbounds for c in 3:10
        n += length(st.pools[c])
    end
    return n
end

# --- changement d'état d'un site (met à jour pools + comptes de voisins) ---
function set_state!(model, st::SimState, id::Int, ns::Int, nt::Int, ng::Int)
    a = model[id]
    if !st.wellmixed
        dS = (ns == SUSC ? 1 : 0) - (a.state == SUSC ? 1 : 0)
        dI = (ns == INF ? 1 : 0) - (a.state == INF ? 1 : 0)
        if dS != 0 || dI != 0
            @inbounds for w in neighbors(st.graph, a.pos)
                st.nS[w] += dS; st.nI[w] += dI
            end
        end
    end
    a.state = ns; a.treated = nt; a.geno = ng
    newc = catid(ns, nt, ng)
    if st.cat[id] != newc
        pool_del!(st, id)
        pool_add!(st, id, newc)
    end
    return a
end

# tire un infecté uniformément parmi tous les infectés
function random_infected(st::SimState, rng)
    n = Itotal(st); n == 0 && return 0
    k = rand(rng, 1:n)
    @inbounds for c in 3:10
        pool = st.pools[c]
        if k <= length(pool); return pool[k]; else; k -= length(pool); end
    end
    return 0
end

random_member(pool, rng) = pool[rand(rng, 1:length(pool))]

# poids beta par génotype et somme totale
function beta_weights(st::SimState)
    vtc = st.p.vtc
    bw = zeros(Float64, NGENO); bs = 0.0
    @inbounds for g in 0:3, t in 0:1
        c = icount(st, g, t)
        b = vtc[BETA, g + 1, t + 1] * c
        bw[g + 1] += b; bs += b
    end
    return bw, bs
end

beta_of(st, g, t) = st.p.vtc[BETA, g + 1, t + 1]

# --- taux des 6 canaux ---
function channel_rates(model, st::SimState)
    p = st.p; vtc = p.vtc
    S = Scount(st); E = Ecount(st); Itot = Itotal(st)

    birth = E > 0 ? p.theta : 0.0
    death = p.mu * S
    clearance = 0.0
    @inbounds for g in 0:3, t in 0:1
        c = icount(st, g, t)
        death += (p.mu + vtc[VIR, g + 1, t + 1]) * c
        clearance += vtc[CLEAR, g + 1, t + 1] * c
    end
    mutation = p.mut * Itot

    if st.wellmixed
        _, bs = beta_weights(st)
        infection = bs * S
        superinf = p.sigma * bs * Itot
    else
        infection = 0.0; superinf = 0.0
        @inbounds for g in 0:3, t in 0:1
            b = beta_of(st, g, t)
            for id in st.pools[catid(INF, t, g)]
                v = model[id].pos
                infection += b * st.nS[v]
                superinf  += b * st.nI[v]
            end
        end
        superinf *= p.sigma
    end
    return (birth, death, infection, clearance, superinf, mutation)
end

# --- exécution de chaque canal ---
function do_birth!(model, st, rng)
    Ecount(st) == 0 && return
    id = random_member(st.pools[1], rng)
    set_state!(model, st, id, SUSC, -1, -1)
end

function do_death!(model, st, rng)
    p = st.p; vtc = p.vtc
    w = zeros(Float64, 9); ids_cat = Vector{Int}(undef, 9)   # 1=S puis 8 = INF[g,t]
    w[1] = p.mu * Scount(st); ids_cat[1] = 2
    k = 1
    @inbounds for t in 0:1, g in 0:3
        k += 1
        c = catid(INF, t, g)
        w[k] = (p.mu + vtc[VIR, g + 1, t + 1]) * length(st.pools[c])
        ids_cat[k] = c
    end
    sel = weighted_choice(w, rng)
    pool = st.pools[ids_cat[sel]]
    isempty(pool) && return
    id = random_member(pool, rng)
    set_state!(model, st, id, EMPTY, -1, -1)
end

function do_clearance!(model, st, rng)
    p = st.p; vtc = p.vtc
    w = zeros(Float64, 8); cats = Vector{Int}(undef, 8)
    k = 0
    @inbounds for t in 0:1, g in 0:3
        k += 1
        c = catid(INF, t, g)
        w[k] = vtc[CLEAR, g + 1, t + 1] * length(st.pools[c])
        cats[k] = c
    end
    sel = weighted_choice(w, rng)
    pool = st.pools[cats[sel]]
    isempty(pool) && return
    id = random_member(pool, rng)
    set_state!(model, st, id, SUSC, -1, -1)
end

function do_infection!(model, st, rng)
    p = st.p
    if st.wellmixed
        bw, _ = beta_weights(st)
        g = weighted_choice(bw, rng) - 1
        isempty(st.pools[2]) && return
        target = random_member(st.pools[2], rng)
    else
        # source infectée ∝ beta * (# voisins S), puis un voisin S au hasard
        Itot = Itotal(st); Itot == 0 && return
        ws = Float64[]; src = Int[]
        @inbounds for gg in 0:3, tt in 0:1
            b = beta_of(st, gg, tt)
            for id in st.pools[catid(INF, tt, gg)]
                v = model[id].pos
                if st.nS[v] > 0
                    push!(ws, b * st.nS[v]); push!(src, id)
                end
            end
        end
        isempty(src) && return
        source = src[weighted_choice(ws, rng)]
        g = model[source].geno
        sneigh = [w for w in neighbors(st.graph, model[source].pos) if model[w].state == SUSC]
        isempty(sneigh) && return
        target = sneigh[rand(rng, 1:length(sneigh))]
    end
    treated = rand(rng) < p.tho ? 1 : 0
    set_state!(model, st, target, INF, treated, g)
end

function do_superinfection!(model, st, rng)
    p = st.p
    if st.wellmixed
        resident = random_infected(st, rng); resident == 0 && return
        bw, _ = beta_weights(st)
        g2 = weighted_choice(bw, rng) - 1
        a = model[resident]
        geno = recombine(a.geno, g2, p.rp, rng)
        geno != a.geno && set_state!(model, st, resident, INF, a.treated, geno)
    else
        # donneur ∝ beta * (# voisins I), résident = un voisin infecté au hasard
        ws = Float64[]; don = Int[]
        @inbounds for gg in 0:3, tt in 0:1
            b = beta_of(st, gg, tt)
            for id in st.pools[catid(INF, tt, gg)]
                v = model[id].pos
                if st.nI[v] > 0
                    push!(ws, b * st.nI[v]); push!(don, id)
                end
            end
        end
        isempty(don) && return
        donor = don[weighted_choice(ws, rng)]
        ineigh = [w for w in neighbors(st.graph, model[donor].pos) if model[w].state == INF]
        isempty(ineigh) && return
        resident = ineigh[rand(rng, 1:length(ineigh))]
        ar = model[resident]
        geno = recombine(ar.geno, model[donor].geno, p.rp, rng)
        geno != ar.geno && set_state!(model, st, resident, INF, ar.treated, geno)
    end
end

function do_mutation!(model, st, rng)
    resident = random_infected(st, rng); resident == 0 && return
    a = model[resident]
    g2 = mutate(a.geno, rng)
    g2 != a.geno && set_state!(model, st, resident, INF, a.treated, g2)
end

# --- enregistrement d'un instantané ---
function record!(model, st::SimState, t::Float64)
    Igt = zeros(Int, NGENO, 2)
    @inbounds for g in 0:3, tt in 0:1
        Igt[g + 1, tt + 1] = icount(st, g, tt)
    end
    si = -1
    if !st.wellmixed
        s = 0
        @inbounds for c in 3:10, id in st.pools[c]
            s += st.nS[model[id].pos]
        end
        si = s
    end
    push!(st.rec_t, t); push!(st.rec_S, Scount(st))
    push!(st.rec_Itot, Itotal(st)); push!(st.rec_Igt, Igt); push!(st.rec_si, si)
end

# --- construction du modèle ---
function build_model(p::Params, graph::SimpleGraph; wellmixed::Bool, seed::Int)
    @assert nv(graph) == p.N "Le graphe doit avoir exactement N=$(p.N) nœuds."
    N = p.N
    st = SimState(p, graph, wellmixed, 0.0,
                  [Int[] for _ in 1:10], zeros(Int, N), zeros(Int, N),
                  zeros(Int, N), zeros(Int, N),
                  Float64[], Int[], Int[], Array{Int,2}[], Int[])
    model = StandardABM(Site, GraphSpace(graph);
                        properties = st, rng = Xoshiro(seed),
                        container = Vector, model_step! = (m) -> nothing)
    for v in 1:N
        add_agent!(v, model; state = EMPTY, treated = -1, geno = -1)
    end
    # tous EMPTY au départ
    for id in 1:N
        pool_add!(st, id, 1)
    end
    return model
end

function init_pop!(model, st::SimState; S0, Iut0, It0, geno0 = 0)
    rng = abmrng(model)
    ids = collect(1:st.p.N); shuffle!(rng, ids)
    k = 1
    for _ in 1:S0;   set_state!(model, st, ids[k], SUSC, -1, -1); k += 1; end
    for _ in 1:Iut0; set_state!(model, st, ids[k], INF, 0, geno0); k += 1; end
    for _ in 1:It0;  set_state!(model, st, ids[k], INF, 1, geno0); k += 1; end
end

"""
    run_abm!(model; tend, out_step)

Boucle de Gillespie. Enregistre un instantané sur une grille temporelle
régulière (`out_step`). S'arrête à `tend`, à l'extinction (Itot=0) ou si le
taux total s'annule.
"""
function run_abm!(model; tend, out_step)
    st = abmproperties(model); rng = abmrng(model)
    next_out = 0.0
    while st.t < tend
        Itotal(st) == 0 && break
        rates = channel_rates(model, st)
        total = sum(rates)
        total <= 0 && break
        st.t += -log(rand(rng)) / total
        while next_out <= st.t && next_out <= tend
            record!(model, st, next_out); next_out += out_step
        end
        st.t > tend && break
        ch = weighted_choice(collect(rates), rng)
        if     ch == 1; do_birth!(model, st, rng)
        elseif ch == 2; do_death!(model, st, rng)
        elseif ch == 3; do_infection!(model, st, rng)
        elseif ch == 4; do_clearance!(model, st, rng)
        elseif ch == 5; do_superinfection!(model, st, rng)
        else            do_mutation!(model, st, rng)
        end
    end
    return st
end

function _series(st::SimState, label)
    n = length(st.rec_t)
    Igt = zeros(Float64, n, NGENO, 2)
    for k in 1:n
        Igt[k, :, :] .= st.rec_Igt[k]
    end
    si = st.wellmixed ? nothing : Float64.(st.rec_si)
    return (t = Float64.(st.rec_t), S = Float64.(st.rec_S),
            Itot = Float64.(st.rec_Itot), Igt = Igt, si = si, label = label)
end

"Résolution B : bien mélangé (graphe complet). Renvoie des séries agrégées."
function run_B(p::Params; seed, S0, Iut0, It0, geno0 = 0, tend = 3000.0, out_step = 10.0)
    model = build_model(p, complete_graph(p.N); wellmixed = true, seed = seed)
    init_pop!(model, abmproperties(model); S0 = S0, Iut0 = Iut0, It0 = It0, geno0 = geno0)
    st = run_abm!(model; tend = tend, out_step = out_step)
    return _series(st, "B (bien mélangé)")
end

"Résolution C : réseau de contacts. `graph` doit avoir N nœuds."
function run_C(p::Params, graph::SimpleGraph; seed, S0, Iut0, It0, geno0 = 0,
               tend = 3000.0, out_step = 10.0)
    model = build_model(p, graph; wellmixed = false, seed = seed)
    init_pop!(model, abmproperties(model); S0 = S0, Iut0 = Iut0, It0 = It0, geno0 = geno0)
    st = run_abm!(model; tend = tend, out_step = out_step)
    return _series(st, "C (réseau)")
end

end # module
