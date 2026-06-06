# =============================================================================
#  validate_vs_eee.jl  —  VALIDATION EXTERNE (temporaire, n'altère rien)
# =============================================================================
#
#  But : comparer le moteur Julia au programme C++ d'origine recompilé
#  (EEE-MISC/drugres_stoch_ode, GSL-only), pour vérifier que les CHIFFRES
#  coïncident — pas seulement la cohérence interne.
#
#  Le modèle canonique (evolab-x) est « parking » : N=1000 sites finis.
#  Le C++ EEE-MISC est « ville ouverte » : population libre.
#  ASTUCE : on aligne TEMPORAIREMENT Julia sur la ville ouverte en mettant un
#  N gigantesque (20000) → le plafond ne mord jamais → la naissance n'est jamais
#  bridée = immigration constante. Le code canonique src/ n'est PAS modifié ;
#  on l'appelle juste avec N=20000 ici.
#
#  Pour le mode « bien mélangé » (B), le graphe n'est jamais consulté → on passe
#  un graphe SANS arêtes (SimpleGraph(N)) au lieu d'un graphe complet (qui
#  exploserait la mémoire à 20000 nœuds).
#
#  Matrice VTC : IDENTIQUE à celle passée au C++ (valeurs finales par génotype) :
#     virulence  : 0.005 partout
#     transmission : g0=.0007/.0003  g1=.0008/.0005  g2=.0008/.0005  g3=.0009/.0007
#     clairance  : 0.005 partout
#
#  Référence C++ (matrice ci-dessus, mêmes scalaires) à l'équilibre :
#     ODE   : S≈23.4  Iut≈19.6  It≈78.2  (Itot≈98)
#     stoch : S≈23    Iut≈49    It≈195   (Itot≈245)
# =============================================================================

using Printf, Graphs, Statistics, Random
using Agents: abmproperties

const SRC = @__DIR__   # archive gelée : les modules sont dans ce même dossier
include(joinpath(SRC, "DrugResModel.jl"))
include(joinpath(SRC, "ResolutionA_ODE.jl"))
include(joinpath(SRC, "ResolutionBC_ABM.jl"))
include(joinpath(SRC, "Indices.jl"))
using .DrugResModel
using .ResolutionA
using .ResolutionBC

# --- matrice VTC identique au C++ ---
vtc = zeros(Float64, 3, 4, 2)
vtc[VIR,   :, 1] = [0.005, 0.005, 0.005, 0.005]
vtc[VIR,   :, 2] = [0.005, 0.005, 0.005, 0.005]
vtc[BETA,  :, 1] = [0.0007, 0.0008, 0.0008, 0.0009]
vtc[BETA,  :, 2] = [0.0003, 0.0005, 0.0005, 0.0007]
vtc[CLEAR, :, 1] = [0.005, 0.005, 0.005, 0.005]
vtc[CLEAR, :, 2] = [0.005, 0.005, 0.005, 0.005]

const N_OPEN = 20000   # << très grand : "ville ouverte"
p = Params(N_OPEN, 10.0, 0.01, 0.8, 1.0, 0.2, 0.01, vtc)

const S0, IUT0, IT0, GENO0, TEND = 270, 20, 80, 0, 3000.0

# ------------------------------------------------------------------ A (ODE) ---
sol = run_ode(p; S0 = S0, Iut0 = IUT0, It0 = IT0, geno0 = GENO0, tend = TEND, saveat = 100.0)
sA = census_series_ode(sol)
function row_at(s, t)
    k = argmin(abs.(s.t .- t))
    Iut = sum(@view s.Igt[k, :, 1]); It = sum(@view s.Igt[k, :, 2])
    g = [s.Igt[k, gg, 1] + s.Igt[k, gg, 2] for gg in 1:4]
    return (S = s.S[k], Iut = Iut, It = It, g = g)
end
println("=== A (Julia ODE, N=$N_OPEN) — comparer à C++ ODE (S≈23.4 Iut≈19.6 It≈78.2) ===")
@printf("%8s %8s %8s %8s | %8s %8s %8s %8s\n", "t", "S", "Iut", "It", "g0", "g1", "g2", "g3")
for t in (100.0, 1000.0, 2000.0, 3000.0)
    r = row_at(sA, t)
    @printf("%8.0f %8.2f %8.2f %8.2f | %8.3f %8.3f %8.3f %8.3f\n", t, r.S, r.Iut, r.It, r.g...)
end

# ------------------------------------------------- B (stochastique, ouvert) ---
println("\n=== B (Julia stochastique ouvert) — comparer à C++ stoch (S≈23 Iut≈49 It≈195) ===")
@printf("%6s %8s %8s %8s %8s\n", "rep", "S", "Iut", "It", "Itot")
for rep in 1:5
    model = build_model(p, SimpleGraph(N_OPEN); wellmixed = true, seed = 4000 + rep)
    ResolutionBC.init_pop!(model, abmproperties(model); S0 = S0, Iut0 = IUT0, It0 = IT0, geno0 = GENO0)
    st = run_abm!(model; tend = TEND, out_step = 100.0)
    s = ResolutionBC._series(st, "B")
    idx = findall(s.t .>= 2000.0)
    Smean = mean(s.S[idx])
    Iut = mean(sum(@view s.Igt[k, :, 1]) for k in idx)
    It  = mean(sum(@view s.Igt[k, :, 2]) for k in idx)
    @printf("%6d %8.0f %8.0f %8.0f %8.0f\n", rep, Smean, Iut, It, Iut + It)
end
