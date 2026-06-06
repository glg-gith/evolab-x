# Valide les réglages des « cas limites » documentés dans cas-limites.md.
# Pour chaque scénario : issue A (ODE) / B (bien mélangé) / C (structuré).
using Printf, Statistics
const SRC = normpath(joinpath(@__DIR__, "..", "src"))
include(joinpath(SRC, "framework", "Runners.jl"))
include(joinpath(SRC, "models", "DrugResCore.jl"))
for m in ("SIR", "SEIR", "Emergence", "Parvir", "DrugRes")
    include(joinpath(SRC, "models", m * ".jl"))
end
using .Runners

runB(model, p, N; tend, out, seed, ms = 25.0) =
    run_ssa(model, p, empty_graph(N); wellmixed = true, tend = tend, out_step = out, seed = seed, max_seconds = ms)
runC(model, p, graph; tend, out, seed, ms = 25.0) =
    run_ssa(model, p, graph; wellmixed = false, tend = tend, out_step = out, seed = seed, max_seconds = ms)

sec(t) = (println("\n========== ", t, " =========="))

# ---------- SIR ----------
sec("SIR")
L = 30; N = L*L; p = merge(SIR.fixed_params(L), (beta = 0.05, gamma = 0.1, I0 = 6.0)); T = 120.0; O = 3.0
A = run_ode(SIR, p; tend = T, saveat = O)
B = runB(SIR, p, N; tend = T, out = O, seed = 1)
Cl = runC(SIR, p, lattice_graph(L); tend = T, out = O, seed = 1)
Cs = runC(SIR, p, smallworld_graph(N, 96, 0.1, 1); tend = T, out = O, seed = 1)
@printf("[spatial: lattice]      A=%.0f  B=%.0f  C=%.0f  (taille finale R∞)\n", SIR.outcome_ode(A,p), SIR.outcome_ssa(B,p), SIR.outcome_ssa(Cl,p))
@printf("[champ moyen: SW k=96]  A=%.0f  B=%.0f  C=%.0f\n", SIR.outcome_ode(A,p), SIR.outcome_ssa(B,p), SIR.outcome_ssa(Cs,p))

# ---------- SEIR ----------
sec("SEIR")
p = merge(SEIR.fixed_params(L), (beta = 0.05, sigma = 0.2, gamma = 0.1, I0 = 6.0))
A = run_ode(SEIR, p; tend = T, saveat = O)
B = runB(SEIR, p, N; tend = T, out = O, seed = 1)
Cl = runC(SEIR, p, lattice_graph(L); tend = T, out = O, seed = 1)
@printf("[spatial: lattice]      A=%.0f  B=%.0f  C=%.0f\n", SEIR.outcome_ode(A,p), SEIR.outcome_ssa(B,p), SEIR.outcome_ssa(Cl,p))

# ---------- Emergence : écart A→B ----------
sec("Emergence")
p = merge(Emergence.fixed_params(L), (beta = 0.14, gamma = 0.10, I0 = 2.0)); T = 300.0; O = 7.5
A = run_ode(Emergence, p; tend = T, saveat = O); finalA = Emergence.outcome_ode(A, p)
maj = 0; vals = Float64[]
for s in 1:12
    b = runB(Emergence, p, N; tend = T, out = O, seed = s); v = Emergence.outcome_ssa(b, p)
    push!(vals, v); global maj += (v > 0.2N)
end
@printf("[seuil R0=1.4, I0=2]    A=%.0f (toujours)  |  B sur 12 graines : %d majeures, %d avortées  (R∞ de 2 à %.0f)\n",
        finalA, maj, 12 - maj, maximum(vals))
psup = merge(Emergence.fixed_params(L), (beta = 0.30, gamma = 0.10, I0 = 2.0))
As = run_ode(Emergence, psup; tend = T, saveat = O); majs = 0
for s in 1:12
    b = runB(Emergence, psup, N; tend = T, out = O, seed = s); global majs += (Emergence.outcome_ssa(b,p) > 0.2N)
end
@printf("[super-seuil R0=3.0]    A=%.0f          |  B : %d/12 majeures (A≈B retrouvé)\n", Emergence.outcome_ode(As, psup), majs)

# ---------- Parvir : évolution de virulence B vs C ----------
sec("Parvir")
L = 24; N = L*L; T = 150.0; O = 3.0
p = merge(Parvir.fixed_params(L), (b0 = 6.0, coef = 0.5, mu = 1.0, mut = 0.008, I0frac = 0.4))
meanvir(g) = (n = sum(count_class(g, i+1) for i in 1:p.ngeno); n == 0 ? 0.0 :
              sum(Parvir.vir(p, i) * count_class(g, i+1) for i in 1:p.ngeno) / n)
A = run_ode(Parvir, p; tend = T, saveat = O)
B = runB(Parvir, p, N; tend = T, out = O, seed = 1)
Cl = runC(Parvir, p, lattice_graph(L); tend = T, out = O, seed = 1)
@printf("[lattice]  prévalence  A=%.0f  B=%.0f  C=%.0f\n", Parvir.outcome_ode(A,p), Parvir.outcome_ssa(B,p), Parvir.outcome_ssa(Cl,p))
@printf("           VIRULENCE évoluée (axe 0..6) :  départ=%.2f  B=%.2f  C=%.2f\n", Parvir.vir(p, cld(p.ngeno,2)), meanvir(B.gstate), meanvir(Cl.gstate))

# ---------- DrugRes : B→C (lourd, budgété) ----------
sec("DrugRes")
L = 24; N = L*L; T = 600.0; O = 25.0
p = merge(DrugRes.fixed_params(L), (theta = 10.0, mu = 0.01, tho = 0.8, sigma = 1.0, rp = 0.2, mut = 0.01))
A = run_ode(DrugRes, p; tend = T, saveat = O)
B = runB(DrugRes, p, N; tend = T, out = O, seed = 1, ms = 30.0)
Cl = runC(DrugRes, p, lattice_graph(L); tend = T, out = O, seed = 1, ms = 30.0)
@printf("[lattice]  prévalence  A=%.0f  B=%.0f  C=%.0f   (B tronqué=%s, C tronqué=%s)\n",
        DrugRes.outcome_ode(A,p), DrugRes.outcome_ssa(B,p), DrugRes.outcome_ssa(Cl,p), B.truncated, Cl.truncated)
println("\n(fin)")
