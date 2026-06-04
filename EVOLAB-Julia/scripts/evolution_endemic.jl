# =============================================================================
#  evolution_endemic.jl  —  OPTION 2 : l'AXE ÉVOLUTIF en régime endémique
# =============================================================================
#
#  QUESTION SCIENTIFIQUE
#  ---------------------
#  « La structure spatiale change-t-elle l'ÉVOLUTION du pathogène (la sélection
#    sur les génotypes / la résistance), au-delà de son effet sur la prévalence ? »
#
#  POURQUOI UN SCRIPT SÉPARÉ
#  -------------------------
#  Avec les paramètres par défaut, le réseau clairsemé effondre l'épidémie
#  (prévalence ~2). À |I|≈2, la fréquence des génotypes n'est plus que de la
#  DÉRIVE sur deux individus : la sélection ne peut pas s'exprimer, et l'axe
#  évolutif est illisible (cf. run_minimal.jl, F2 de C avec un écart-type
#  supérieur à la moyenne).
#
#  Pour ÉTUDIER l'évolution, il faut un régime où même le réseau reste
#  franchement endémique. On y arrive en :
#    (a) augmentant la transmissibilité (facteur BETAFAC sur toute la matrice
#        de transmission — cela préserve les DIFFÉRENCES relatives entre
#        génotypes, donc la structure de sélection), et
#    (b) prenant un degré moyen modéré mais suffisant (DEG) : assez de structure
#        pour que ça compte, assez de connectivité pour rester endémique.
#
#  RÈGLE D'OR CONSERVÉE : A, B et C partagent EXACTEMENT les mêmes paramètres
#  microscopiques. On ne re-calibre rien entre résolutions. On compare la
#  COMPOSITION génotypique à l'équilibre.
#
#  RAPPEL DU MODÈLE (defaults) : seule la TRANSMISSION dépend du génotype, et
#  le traitement la réduit. Le génotype AB (g3) a la transmission la plus forte
#  → la sélection devrait le favoriser. La question : le réseau modifie-t-il
#  l'équilibre des fréquences de génotypes et la fraction traitée par rapport au
#  champ moyen ?
#
#  SORTIE
#  ------
#  Pour A, B, C : la prévalence d'équilibre, la distribution complète des 4
#  génotypes (ab, Ab, aB, AB), la fréquence du génotype AB (proxy de « souche
#  sélectionnée »), et la fraction d'infectés traités.
# =============================================================================

using Printf
using Graphs
using Statistics
using Random

const SRC = normpath(joinpath(@__DIR__, "..", "src"))
include(joinpath(SRC, "DrugResModel.jl"))
include(joinpath(SRC, "ResolutionA_ODE.jl"))
include(joinpath(SRC, "ResolutionBC_ABM.jl"))
include(joinpath(SRC, "Indices.jl"))
using .DrugResModel
using .ResolutionA
using .ResolutionBC
using .Indices

# --- paramètres de l'expérience ---
const N        = 1000
const TEND     = 3000.0
const OUT_STEP = 20.0
const REPS     = 10
const BETAFAC  = 12.0    # facteur d'amplification de la transmission (garde C endémique)
const DEG      = 30      # degré moyen du réseau (structure modérée mais endémique)
const WS_BETA  = 0.1

# -----------------------------------------------------------------------------
#  Construit des paramètres avec transmission amplifiée.
#  On scale la matrice de transmission DÉJÀ construite : l'opération est linéaire,
#  donc les rapports entre génotypes (la structure de sélection) sont préservés.
# -----------------------------------------------------------------------------
function scaled_params(; N, betafac)
    p0 = default_params(; N = N)
    vtc = copy(p0.vtc)
    vtc[BETA, :, :] .*= betafac
    return Params(p0.N, p0.theta, p0.mu, p0.tho, p0.sigma, p0.rp, p0.mut, vtc)
end

p = scaled_params(; N = N, betafac = BETAFAC)

const S0    = 270
const I0    = 100
const IT0   = round(Int, I0 * p.tho)
const IUT0  = I0 - IT0
const GENO0 = 0

# -----------------------------------------------------------------------------
#  A — champ moyen (ODE) ; une seule trajectoire (déterministe)
# -----------------------------------------------------------------------------
@info "A (ODE champ moyen)…"
solA = run_ode(p; S0 = S0, Iut0 = IUT0, It0 = IT0, geno0 = GENO0,
               tend = TEND, saveat = OUT_STEP)
sA = census_series_ode(solA)
a1 = family1(sA); a2 = family2(sA)

# -----------------------------------------------------------------------------
#  B — bien mélangé (ensemble)
# -----------------------------------------------------------------------------
@info "B (bien mélangé), $REPS réplications…"
B = [run_B(p; seed = 2000 + r, S0 = S0, Iut0 = IUT0, It0 = IT0,
           geno0 = GENO0, tend = TEND, out_step = OUT_STEP) for r in 1:REPS]

# -----------------------------------------------------------------------------
#  C — réseau (ensemble), degré DEG
# -----------------------------------------------------------------------------
@info "C (réseau small-world k=$DEG), $REPS réplications…"
C = map(1:REPS) do r
    g = watts_strogatz(N, DEG, WS_BETA; rng = Xoshiro(9000 + r))
    run_C(p, g; seed = 9000 + r, S0 = S0, Iut0 = IUT0, It0 = IT0,
          geno0 = GENO0, tend = TEND, out_step = OUT_STEP)
end

# --- agrégation des distributions génotypiques sur l'ensemble ---
# Renvoie (moyenne, écart-type) du vecteur de fréquences des 4 génotypes.
function geno_distribution(series_vec; tail = 0.3)
    fs = [family2(s; tail = tail).geno_freqs for s in series_vec]   # vecteurs longueur 4
    M = reduce(hcat, fs)                                            # 4 × reps
    return vec(mean(M; dims = 2)), vec(std(M; dims = 2))
end
geno_distribution_single(s; tail = 0.3) = family2(s; tail = tail).geno_freqs

fB, sdB = geno_distribution(B)
fC, sdC = geno_distribution(C)
fA = geno_distribution_single(sA)

prevB = ensemble_summary(B).eq_prevalence
prevC = ensemble_summary(C).eq_prevalence
trtB  = mean(family2(s).treated_frac for s in B)
trtC  = mean(family2(s).treated_frac for s in C)

# -----------------------------------------------------------------------------
#  Tableaux de résultats
# -----------------------------------------------------------------------------
genonames = ["ab (g0)", "Ab (g1)", "aB (g2)", "AB (g3)"]

println("\n", "="^72)
println("  AXE ÉVOLUTIF EN RÉGIME ENDÉMIQUE  (betafac=$BETAFAC, degré=$DEG)")
println("="^72)
@printf("%-22s %12s %16s %16s\n", "", "A (ODE)", "B (mass-act.)", "C (réseau)")
@printf("%-22s %12.1f %8.1f±%-7.1f %8.1f±%-7.1f\n",
        "Prévalence eq. |I|", a1.eq_prevalence,
        prevB[1], prevB[2], prevC[1], prevC[2])
@printf("%-22s %12.3f %16.3f %16.3f\n",
        "Fraction traitée", a2.treated_frac, trtB, trtC)
println("-"^72)
println("  Distribution des génotypes à l'équilibre (fréquence parmi les infectés)")
println("-"^72)
@printf("%-22s %12s %16s %16s\n", "génotype", "A (ODE)", "B (mass-act.)", "C (réseau)")
for g in 1:4
    @printf("%-22s %12.3f %8.3f±%-7.3f %8.3f±%-7.3f\n",
            genonames[g], fA[g], fB[g], sdB[g], fC[g], sdC[g])
end
println("-"^72)
println("Lecture : si la colonne C diffère significativement de A/B sur la")
println("distribution des génotypes (et pas seulement sur la prévalence), alors")
println("la structure spatiale modifie la SÉLECTION — c'est le résultat cherché.")
println("Si C ≈ A ≈ B, le champ moyen capture l'évolution malgré la structure.")
println("="^72)
