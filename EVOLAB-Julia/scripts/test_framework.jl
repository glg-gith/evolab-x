# Test headless du framework L2 : A/B/C sur SIR, divergence champ-moyen vs grille.
using Printf
const SRC = normpath(joinpath(@__DIR__, "..", "src"))
include(joinpath(SRC, "framework", "Runners.jl"))
include(joinpath(SRC, "models", "SIR.jl"))
using .Runners

L = 30; N = L * L
p = (N = N, beta = 0.03, gamma = 0.1, I0 = 5)
TEND = 120.0; OUT = 2.0

peak(r) = isempty(r.prev) ? 0.0 : maximum(r.prev)

A = run_ode(SIR, p; tend = TEND, saveat = OUT)
B = run_ssa(SIR, p, empty_graph(N); wellmixed = true,  tend = TEND, out_step = OUT, seed = 1)
C = run_ssa(SIR, p, lattice_graph(L); wellmixed = false, tend = TEND, out_step = OUT, seed = 1,
            record_spatial = true)

@printf("%-22s %12s %12s\n", "", "pic I", "taille finale")
@printf("%-22s %12.1f %12.1f\n", "A (champ moyen, ODE)", peak(A), SIR.outcome_ode(A, p))
@printf("%-22s %12.1f %12.1f\n", "B (bien mélangé)",      peak(B), SIR.outcome_ssa(B, p))
@printf("%-22s %12.1f %12.1f\n", "C (grille $(L)x$(L), deg~4)", peak(C), SIR.outcome_ssa(C, p))
println("snapshots spatiaux capturés (C) : ", length(C.snaps), " frames de ", N, " nœuds")

println("\n=== frontière de validité (small-world, degré k croissant) ===")
fr = frontier(SIR, p; N = N, degrees = [4, 8, 16, 32, 64], reps = 3, tend = TEND, out_step = OUT)
@printf("  taille finale champ moyen (réf) = %.1f\n", fr.outA)
for (k, ratio) in zip(fr.k, fr.ratio)
    @printf("  k=%-4.0f  ratio C/A = %5.2f%%\n", k, 100 * ratio)
end
println("\n(attendu : C grille s'effondre vs A ; le ratio remonte quand k augmente)")
