"""
    SIR — modèle SIR (sans démographie), implémentant la convention de modèle (L2).

Classes : 1=S, 2=I, 3=R.
Transmission **par contact** (densité-dépendante), cohérente entre les 3 résolutions :
- réseau (C)   : un infecté infecte chaque voisin susceptible au taux β  → taux total = β · (#arêtes S-I).
- bien mélangé (B) : graphe complet implicite → taux = β · I · S.
- champ moyen (A)  : dS=-βSI, dI=βSI-γI, dR=γI  (= limite de B).
C'est le SIR le plus simple, et c'est lui qui montre le mieux la divergence
champ-moyen / spatial (vague qui se propage sur une grille).
"""
module SIR

using ..Runners: GState, set_class!, count_class, nodes_of, nbr_count, pick_neighbor, wchoose
using Random: randperm

const S = 1; const I = 2; const R = 3

nclasses(p) = 3
prevalence_classes(p) = (I,)
state_names() = ["S", "I", "R"]
state_palette(p) = [:steelblue, :crimson, :gray75]
fixed_params(L) = (N = L * L,)
suggested_tend(p) = 120.0

# specs de sliders : (nom, min, max, défaut)
param_specs() = [(:beta, 0.005, 0.06, 0.05),
                 (:gamma, 0.02, 0.5, 0.1),
                 (:I0, 1, 40, 6)]

function init_classes(p, N, rng)
    cl = fill(S, N)
    ni = min(round(Int, p.I0), N)
    for i in randperm(rng, N)[1:ni]
        cl[i] = I
    end
    return cl
end

function rates(g::GState)
    p = g.params
    Sc = count_class(g, S); Ic = count_class(g, I)
    if g.wellmixed
        inf = p.beta * Ic * Sc
    else
        s = 0
        @inbounds for v in nodes_of(g, I); s += nbr_count(g, S, v); end
        inf = p.beta * s
    end
    rec = p.gamma * Ic
    return (inf, rec)
end

function execute!(g::GState, k::Int)
    if k == 1                       # infection : un S → I
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
        set_class!(g, target, I)
    else                            # guérison : un I → R
        count_class(g, I) == 0 && return
        id = nodes_of(g, I)[rand(g.rng, 1:count_class(g, I))]
        set_class!(g, id, R)
    end
end

# --- champ moyen (counts S,I,R) ---
init_u0(p) = [float(p.N - round(Int, p.I0)), float(round(Int, p.I0)), 0.0]
function meanfield_rhs!(du, u, p, t)
    Sf, If, _ = u
    du[1] = -p.beta * Sf * If
    du[2] =  p.beta * Sf * If - p.gamma * If
    du[3] =  p.gamma * If
    return nothing
end
u_prevalence(u, p) = u[2]

# observable de sortie = TAILLE FINALE de l'épidémie (R∞), car le SIR s'éteint
outcome_ssa(res, p) = p.N - count_class(res.gstate, S)
outcome_ode(res, p) = res.sol.u[end][3]

end # module
