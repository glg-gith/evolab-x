# =============================================================================
#  sweep_validity.jl  —  OPTION 1 : la FRONTIÈRE DE VALIDITÉ du champ moyen
# =============================================================================
#
#  QUESTION SCIENTIFIQUE
#  ---------------------
#  « À partir de quand le modèle agrégé (champ moyen / mass-action) redevient-il
#    une approximation fidèle de la réalité individu-centrée sur réseau ? »
#
#  On ne mesure pas UN écart, on trace la COURBE de l'écart en fonction d'un
#  bouton qui fait passer continûment du monde « structuré » au monde « bien
#  mélangé ». La valeur du bouton où l'écart franchit un seuil acceptable = la
#  frontière de validité.
#
#  QUEL BOUTON ?  ---  POINT MÉTHODOLOGIQUE CRUCIAL
#  ------------------------------------------------
#  Intuition naïve : « augmenter le recâblage small-world (ws_beta) de 0 à 1
#  rapproche du bien-mélangé ». C'EST FAUX. Le recâblage Watts-Strogatz garde le
#  DEGRÉ MOYEN constant : il transforme un treillis local en graphe aléatoire,
#  mais chaque nœud garde ~k voisins. Il change donc la STRUCTURE (clustering,
#  corrélations) sans changer la CONNECTIVITÉ. Or ce qui pilote la convergence
#  vers le mass-action, c'est le nombre de contacts : il faut k → N.
#
#  → Le vrai bouton vers le champ moyen est le DEGRÉ MOYEN k.
#    (On garde un petit recâblage fixe pour éviter les artefacts purement
#     géométriques du treillis régulier.)
#
#  Ce que B(bien mélangé) ≈ A(ODE) a déjà été validé, et C(graphe complet) ≈ B
#  aussi. Ici on remplit l'espace ENTRE C(clairsemé) et C(complet).
#
#  SORTIE
#  ------
#  Un tableau : pour chaque degré k, la prévalence d'équilibre de C (moyenne ±
#  écart-type sur plusieurs réplications), son ratio à la baseline mass-action B,
#  et la corrélation S-I (famille 3 = la CAUSE structurelle de l'écart).
#  On s'attend à voir la prévalence de C grimper de ~quelques individus (k=6)
#  vers la valeur mass-action (~640) à mesure que k augmente, ET la corrélation
#  S-I se rapprocher de 1 (mélange parfait).
# =============================================================================

using Printf
using Graphs
using Statistics
using Random

# --- chargement des modules du projet ---
const SRC = normpath(joinpath(@__DIR__, "..", "src"))
include(joinpath(SRC, "DrugResModel.jl"))
include(joinpath(SRC, "ResolutionBC_ABM.jl"))
include(joinpath(SRC, "Indices.jl"))
using .DrugResModel
using .ResolutionBC
using .Indices

# --- paramètres de l'expérience (pilote : tailles modestes pour rester rapide) ---
const N        = 1000          # nombre de sites
const TEND     = 1500.0        # horizon temporel
const OUT_STEP = 20.0          # pas d'enregistrement
const REPS     = 6             # réplications stochastiques par point
const WS_BETA  = 0.1           # recâblage FIXE (isole l'effet du degré)
const DEGREES  = [6, 12, 24, 48, 96, 192]   # le bouton : degré moyen k

p = default_params(; N = N)

# Conditions initiales identiques partout (cf. DrugRes.cpp:535-540)
const S0   = 270
const I0   = 100
const IT0  = round(Int, I0 * p.tho)   # initialement traités
const IUT0 = I0 - IT0                  # initialement non traités
const GENO0 = 0                        # tous démarrent en génotype ab

# -----------------------------------------------------------------------------
#  1) Baseline : B (bien mélangé) = limite k → N (référence mass-action)
# -----------------------------------------------------------------------------
@info "Baseline B (bien mélangé), $REPS réplications…"
B = [run_B(p; seed = 1000 + r, S0 = S0, Iut0 = IUT0, It0 = IT0,
           geno0 = GENO0, tend = TEND, out_step = OUT_STEP) for r in 1:REPS]
eb = ensemble_summary(B)
prev_B = eb.eq_prevalence[1]

# -----------------------------------------------------------------------------
#  2) Balayage du degré : C sur réseau small-world de degré k croissant
# -----------------------------------------------------------------------------
results = Vector{NamedTuple}()
for k in DEGREES
    @info "C : réseau small-world degré k=$k (β=$WS_BETA), $REPS réplications…"
    Ck = map(1:REPS) do r
        # watts_strogatz(N, k, β) : k voisins par nœud, recâblés avec proba β.
        # rng dédié par réplication pour la reproductibilité.
        g = watts_strogatz(N, k, WS_BETA; rng = Xoshiro(7000 + 31 * k + r))
        run_C(p, g; seed = 7000 + 31 * k + r, S0 = S0, Iut0 = IUT0, It0 = IT0,
              geno0 = GENO0, tend = TEND, out_step = OUT_STEP)
    end
    ec = ensemble_summary(Ck)
    # Famille 3 : corrélation S-I moyenne (cause structurelle).
    corr = mean(family3(s; mean_degree = k, N = N).si_correlation for s in Ck)
    push!(results, (k = k,
                    prev = ec.eq_prevalence[1], prev_sd = ec.eq_prevalence[2],
                    ratio = ec.eq_prevalence[1] / prev_B,
                    corr = corr))
end

# -----------------------------------------------------------------------------
#  3) Tableau de la frontière de validité
# -----------------------------------------------------------------------------
println("\n", "="^70)
println("  FRONTIÈRE DE VALIDITÉ  —  prévalence de C vs degré moyen k")
println("  (baseline mass-action B = ", @sprintf("%.1f", prev_B), ")")
println("="^70)
@printf("%6s | %16s | %10s | %16s\n", "k", "prévalence C", "ratio/B", "corr. S-I (F3)")
println("-"^70)
for r in results
    @printf("%6d | %8.1f ± %-5.1f | %9.2f%% | %16.3f\n",
            r.k, r.prev, r.prev_sd, 100 * r.ratio, r.corr)
end
println("-"^70)
println("Lecture : à β par contact fixe, plus k augmente, plus C rejoint le")
println("mass-action (ratio → 100 %) et plus la corrélation S-I → 1.")
println("La frontière de validité ≈ le k où le ratio dépasse un seuil (ex. 95 %).")
println("Rappel : à degré fixe, recâbler (ws_beta) ne suffit PAS à atteindre B.")
println("="^70)
