"""
    Émergence — SIR à incidence standard, calé au SEUIL épidémique.

But pédagogique : isoler l'écart **A→B** (stochasticité / fade-out), celui que les
autres modèles ne montrent pas (chez eux B≈A car franchement super-seuil). Ici
R0 ≈ β/γ est volontairement proche de 1 et le nombre de cas-index `I0` est petit :
- **A** (ODE) part *toujours* en épidémie (déterministe, ignore l'extinction) ;
- **B** (stochastique bien mélangé) avorte *souvent* — les rares cas-index guérissent
  avant d'avoir contaminé assez (fade-out par extinction). Bouge la **graine** pour
  voir basculer entre épidémie majeure et avortée : c'est l'écart A→B, en direct.
- **C** (structure) ne fait qu'aggraver.

Incidence normalisée par le nombre de contacts (N en champ moyen, ⟨k⟩ en réseau) →
R0 ≈ β/γ indépendant de la taille ET de la structure. L'écart A→B est donc d'origine
purement stochastique, pas un artefact de R0 qui changerait avec N. Voir aussi le
« piège du R0 » du plan : ici on l'a neutralisé exprès pour isoler la stochasticité.

Classes : 1=S, 2=I, 3=R.
"""
module Emergence

using ..Runners: GState, set_class!, count_class, nodes_of, nbr_count, pick_neighbor
using Graphs: ne, nv
using Random: randperm

const S = 1; const I = 2; const R = 3

nclasses(p) = 3
prevalence_classes(p) = (I,)
state_names() = ["S", "I", "R"]
state_palette(p) = [:steelblue, :crimson, :gray75]
fixed_params(L) = (N = L * L,)
suggested_tend(p) = 300.0

# R0 = β/γ ; défauts calés juste au-dessus du seuil pour maximiser le fade-out
param_specs() = [(:beta, 0.08, 0.40, 0.14),    # R0 ≈ 0.8 .. 4.0 (défaut 1.4)
                 (:gamma, 0.05, 0.30, 0.10),
                 (:I0, 1, 10, 2)]              # peu de cas-index → extinction fréquente

function init_classes(p, N, rng)
    cl = fill(S, N)
    for i in randperm(rng, N)[1:min(round(Int, p.I0), N)]
        cl[i] = I
    end
    return cl
end

# taux de transmission par contact S-I, normalisé : β/N (champ moyen) ou β/⟨k⟩ (réseau)
@inline contact_norm(g::GState) = g.wellmixed ? float(g.params.N) : 2 * ne(g.graph) / nv(g.graph)

function rates(g::GState)
    p = g.params
    Sc = count_class(g, S); Ic = count_class(g, I)
    rate = p.beta / contact_norm(g)
    if g.wellmixed
        inf = rate * Ic * Sc
    else
        s = 0
        @inbounds for v in nodes_of(g, I); s += nbr_count(g, S, v); end
        inf = rate * s
    end
    return (inf, p.gamma * Ic)
end

function execute!(g::GState, k::Int)
    if k == 1                       # infection : un S → I
        if g.wellmixed
            count_class(g, S) == 0 && return
            target = nodes_of(g, S)[rand(g.rng, 1:count_class(g, S))]
        else
            inf = nodes_of(g, I); total = 0
            @inbounds for v in inf; total += nbr_count(g, S, v); end
            total == 0 && return
            x = rand(g.rng) * total; acc = 0; source = inf[1]
            @inbounds for v in inf
                acc += nbr_count(g, S, v)
                if x <= acc; source = v; break; end
            end
            target = pick_neighbor(g, source, S); target == 0 && return
        end
        set_class!(g, target, I)
    else                            # guérison : un I → R
        count_class(g, I) == 0 && return
        set_class!(g, nodes_of(g, I)[rand(g.rng, 1:count_class(g, I))], R)
    end
end

init_u0(p) = [float(p.N - round(Int, p.I0)), float(round(Int, p.I0)), 0.0]
function meanfield_rhs!(du, u, p, t)
    Sf, If, _ = u
    rate = p.beta / p.N
    du[1] = -rate * Sf * If
    du[2] =  rate * Sf * If - p.gamma * If
    du[3] =  p.gamma * If
    return nothing
end
u_prevalence(u, p) = u[2]

outcome_ssa(res, p) = p.N - count_class(res.gstate, S)   # taille finale R∞
outcome_ode(res, p) = res.sol.u[end][3]

end # module
