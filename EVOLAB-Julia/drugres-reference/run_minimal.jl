# Expérience minimale (« Figure 1 ») : A vs B vs C sur le modèle DrugRes.
#
# Usage :  julia --project=. scripts/run_minimal.jl
#
# - A : champ moyen déterministe (ODE)
# - B : stochastique bien mélangé (réplique de la version C++ d'origine)
# - C : stochastique sur réseau (small-world)
#
# Mêmes taux microscopiques pour les trois (matching au niveau MICRO, pas macro).
# On NE re-calibre PAS l'ODE sur l'ABM : la divergence EST le signal.

using Printf
using Graphs
using Statistics
using Random

const SRC = @__DIR__   # archive gelée : les modules sont dans ce même dossier
include(joinpath(SRC, "DrugResModel.jl"))
include(joinpath(SRC, "ResolutionA_ODE.jl"))
include(joinpath(SRC, "ResolutionBC_ABM.jl"))
include(joinpath(SRC, "Indices.jl"))

using .DrugResModel
using .ResolutionA
using .ResolutionBC
using .Indices

# ----- paramètres (defaults DrugRes ; tailles modestes pour un pilote) -----
N        = 1000
tend     = 3000.0
out_step = 10.0
reps     = 20            # nombre de réplications stochastiques
mean_deg = 6             # degré moyen du réseau small-world
ws_beta  = 0.1           # proba de recâblage (0 = treillis, 1 = aléatoire)

p = default_params(; N = N)

# Conditions initiales (cf. InitPopulation, DrugRes.cpp:535-540, 883-916)
S0   = 270
I0   = 100
It0  = round(Int, I0 * p.tho)
Iut0 = I0 - It0
geno0 = 0                # tous démarrent en ab : la résistance doit ÉMERGER

@info "Résolution A (ODE champ moyen)…"
solA = run_ode(p; S0 = S0, Iut0 = Iut0, It0 = It0, geno0 = geno0,
               tend = tend, saveat = out_step)
sA = census_series_ode(solA)

@info "Résolution B (bien mélangé), $reps réplications…"
B = [run_B(p; seed = 1000 + r, S0 = S0, Iut0 = Iut0, It0 = It0,
           geno0 = geno0, tend = tend, out_step = out_step) for r in 1:reps]

@info "Résolution C (réseau small-world k=$mean_deg, β=$ws_beta), $reps réplications…"
C = [begin
        g = watts_strogatz(N, mean_deg, ws_beta; rng = Random.Xoshiro(7000 + r))
        run_C(p, g; seed = 7000 + r, S0 = S0, Iut0 = Iut0, It0 = It0,
              geno0 = geno0, tend = tend, out_step = out_step)
     end for r in 1:reps]

# ----- indices -----
a1 = family1(sA); a2 = family2(sA)
eb = ensemble_summary(B); ec = ensemble_summary(C)
c3 = mean(family3(s; mean_degree = mean_deg, N = N).si_correlation for s in C)

println("\n", "="^64)
println("  INDICES  (régime quasi-stationnaire, fin de trajectoire)")
println("="^64)
@printf("%-26s %12s %14s %14s\n", "", "A (ODE)", "B (mass-act.)", "C (réseau)")
@printf("%-26s %12.2f %8.1f±%-5.1f %8.1f±%-5.1f\n",
        "F1 · prévalence eq. |I|", a1.eq_prevalence,
        eb.eq_prevalence[1], eb.eq_prevalence[2],
        ec.eq_prevalence[1], ec.eq_prevalence[2])
@printf("%-26s %12.3f %8.3f±%-5.3f %8.3f±%-5.3f\n",
        "F2 · fréquence génotype AB", a2.freq_AB,
        eb.freq_AB[1], eb.freq_AB[2], ec.freq_AB[1], ec.freq_AB[2])
@printf("%-26s %12s %12.0f%% %12.0f%%\n",
        "F2 · persistance", "—", 100eb.persistence, 100ec.persistence)
@printf("%-26s %12s %12s %12.2f\n",
        "F3 · corrélation S-I (C)", "1.00*", "~1.0", c3)
println("-"^64)
println("* En champ moyen, la corrélation S-I vaut 1 par construction (mélange parfait).")
println("  L'écart de C à 1.00 chiffre la CAUSE structurelle de la divergence A/B vs C.")
println("="^64)
