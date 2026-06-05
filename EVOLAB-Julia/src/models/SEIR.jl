"""
    SEIR — modèle SEIR (sans démographie), convention de modèle L2.
Classes : 1=S, 2=E (exposé, pas encore infectieux), 3=I (infectieux), 4=R.
Seuls les I transmettent. Transmission par contact (comme SIR).
"""
module SEIR

using ..Runners: GState, set_class!, count_class, nodes_of, nbr_count, pick_neighbor, wchoose
using Random: randperm

const S = 1; const E = 2; const I = 3; const R = 4

nclasses(p) = 4
prevalence_classes(p) = (I,)
state_names() = ["S", "E", "I", "R"]
state_palette(p) = [:steelblue, :orange, :crimson, :gray75]
fixed_params(L) = (N = L * L,)
suggested_tend(p) = 120.0

param_specs() = [(:beta, 0.005, 0.06, 0.05),
                 (:sigma, 0.05, 1.0, 0.2),
                 (:gamma, 0.02, 0.5, 0.1),
                 (:I0, 1, 40, 6)]

function init_classes(p, N, rng)
    cl = fill(S, N)
    for i in randperm(rng, N)[1:min(round(Int, p.I0), N)]
        cl[i] = I
    end
    return cl
end

function rates(g::GState)
    p = g.params
    Sc = count_class(g, S); Ec = count_class(g, E); Ic = count_class(g, I)
    if g.wellmixed
        inf = p.beta * Ic * Sc
    else
        s = 0
        @inbounds for v in nodes_of(g, I); s += nbr_count(g, S, v); end
        inf = p.beta * s
    end
    return (inf, p.sigma * Ec, p.gamma * Ic)   # infection, progression E→I, guérison
end

function execute!(g::GState, k::Int)
    if k == 1                       # infection : S → E
        if g.wellmixed
            count_class(g, S) == 0 && return
            target = nodes_of(g, S)[rand(g.rng, 1:count_class(g, S))]
        else
            inf = nodes_of(g, I)
            total = 0
            @inbounds for v in inf; total += nbr_count(g, S, v); end
            total == 0 && return
            x = rand(g.rng) * total; acc = 0; source = inf[1]
            @inbounds for v in inf
                acc += nbr_count(g, S, v)
                if x <= acc; source = v; break; end
            end
            target = pick_neighbor(g, source, S)
            target == 0 && return
        end
        set_class!(g, target, E)
    elseif k == 2                   # progression : E → I
        count_class(g, E) == 0 && return
        set_class!(g, nodes_of(g, E)[rand(g.rng, 1:count_class(g, E))], I)
    else                            # guérison : I → R
        count_class(g, I) == 0 && return
        set_class!(g, nodes_of(g, I)[rand(g.rng, 1:count_class(g, I))], R)
    end
end

init_u0(p) = [float(p.N - round(Int, p.I0)), 0.0, float(round(Int, p.I0)), 0.0]
function meanfield_rhs!(du, u, p, t)
    Sf, Ef, If, _ = u
    du[1] = -p.beta * Sf * If
    du[2] =  p.beta * Sf * If - p.sigma * Ef
    du[3] =  p.sigma * Ef - p.gamma * If
    du[4] =  p.gamma * If
    return nothing
end
u_prevalence(u, p) = u[3]
outcome_ssa(res, p) = p.N - count_class(res.gstate, S)
outcome_ode(res, p) = res.sol.u[end][4]

end # module
