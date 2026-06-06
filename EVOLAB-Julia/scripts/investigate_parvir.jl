# Investigation : Parvir, virulence évoluée A vs B vs C — version durcie.
# Question : le « C>B » à T fixe est-il un vrai effet spatial, ou un artefact
# transitoire (C équilibre plus lentement) ?
# Corrections vs v1 :
#   - imprime truncated + t final réel  → détecte la contamination par le budget
#   - n=10 graines + erreur-type        → la décimale n'est plus décorative
#   - référence = A (ODE) à l'équilibre, MÊME estimateur (moyenne pond. prévalence)
#     au lieu d'un argmax théorique naïf.
#
# Sortie compacte : scripts/parvir_investigation.txt (run détaché → on lit ce fichier).
using Printf, Statistics
const SRC = normpath(joinpath(@__DIR__, "..", "src"))
include(joinpath(SRC, "framework", "Runners.jl"))
include(joinpath(SRC, "models", "Parvir.jl"))
using .Runners

const RESULT = joinpath(@__DIR__, "parvir_investigation.txt")
const IO_OUT = open(RESULT, "w")
emit(s) = (println(s); println(IO_OUT, s); flush(IO_OUT))
emitf(fmt, args...) = (s = Printf.format(Printf.Format(fmt), args...); print(s); print(IO_OUT, s); flush(IO_OUT))

# virulence moyenne pondérée par prévalence — estimateur commun A/B/C
meanvir_counts(counts, p) = (n = sum(counts); n == 0 ? NaN :
    sum(Parvir.vir(p, i) * counts[i] for i in 1:p.ngeno) / n)
meanvir_ssa(g, p) = meanvir_counts([count_class(g, i + 1) for i in 1:p.ngeno], p)
meanvir_ode(u, p) = meanvir_counts([u[i + 1] for i in 1:p.ngeno], p)   # u = [S, g1..gng]
sem(v) = std(v) / sqrt(length(v))

L = 24; N = L * L; O = 3.0
p = merge(Parvir.fixed_params(L), (b0 = 6.0, coef = 0.5, mu = 1.0, mut = 0.008, I0frac = 0.4))
const BUDGET = 300.0   # large : on VEUT qu'aucun run ne tronque ; sinon on le verra

runBC(graph, wm; tend, seed) =
    run_ssa(Parvir, p, graph; wellmixed = wm, tend = tend, out_step = O, seed = seed, max_seconds = BUDGET)

# référence A : ODE poussée loin, virulence à l'équilibre
Aeq = run_ode(Parvir, p; tend = 4000.0, saveat = 50.0)
virA = meanvir_ode(Aeq.sol.u[end], p)
emitf("RÉFÉRENCE  départ = %.2f   |   A (ODE, T=4000) virulence équilibre = %.2f\n\n",
      Parvir.vir(p, cld(p.ngeno, 2)), virA)

# ---- (1) ensemble à T=150 : le +0.32 est-il significatif ? ----
emit("(1) ENSEMBLE à T=150 — 16 graines (effet apparent)")
T = 150.0; nseed = 16
vb = Float64[]; vc = Float64[]
for s in 1:nseed
    push!(vb, meanvir_ssa(runBC(empty_graph(N), true;  tend = T, seed = s).gstate, p))
    push!(vc, meanvir_ssa(runBC(lattice_graph(L), false; tend = T, seed = s).gstate, p))
end
d = vc .- vb
emitf("    B = %.2f ± %.2f   |   C = %.2f ± %.2f\n", mean(vb), std(vb), mean(vc), std(vc))
emitf("    C−B apparié = %.3f ± %.3f (SEM)   |   t ≈ %.2f   → %s\n\n",
      mean(d), sem(d), mean(d) / sem(d),
      abs(mean(d)) > 1.96 * sem(d) ? "SIGNIFICATIF" : "DANS le bruit")

# ---- (2) convergence : n=10, avec truncated + t final réel ----
emit("(2) CONVERGENCE — n=10 graines, vs A=$(round(virA, digits=2)). 'tr' = #runs tronqués / t̄ final")
emitf("    %6s | %-18s | %-18s | %8s | %s\n", "T", "B (±SEM)", "C (±SEM)", "C−B", "intégrité")
for T in (150.0, 400.0, 900.0, 1500.0)
    bs = [runBC(empty_graph(N), true;  tend = T, seed = s) for s in 1:10]
    cs = [runBC(lattice_graph(L), false; tend = T, seed = s) for s in 1:10]
    vb = [meanvir_ssa(r.gstate, p) for r in bs]; vc = [meanvir_ssa(r.gstate, p) for r in cs]
    trB = count(r -> r.truncated, bs); trC = count(r -> r.truncated, cs)
    tB = mean(r.t[end] for r in bs);   tC = mean(r.t[end] for r in cs)
    emitf("    %6.0f | %6.2f ± %.2f      | %6.2f ± %.2f      | %+7.2f | trB=%d trC=%d  t̄B=%.0f t̄C=%.0f\n",
          T, mean(vb), sem(vb), mean(vc), sem(vc), mean(vc) - mean(vb), trB, trC, tB, tC)
end
emit("\n(fin)")
close(IO_OUT)
