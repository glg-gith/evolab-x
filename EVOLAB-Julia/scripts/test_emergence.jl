# Valide le modèle Émergence : (1) l'écart A→B (fade-out stochastique) et
# (2) la convergence C→A quand le degré moyen ⟨k⟩ → N (bouton d'interpolation).
using Printf
const SRC = normpath(joinpath(@__DIR__, "..", "src"))
include(joinpath(SRC, "framework", "Runners.jl"))
include(joinpath(SRC, "models", "Emergence.jl"))
using .Runners

L = 30; N = L * L
p = merge(Emergence.fixed_params(L), (beta = 0.14, gamma = 0.10, I0 = 2.0))
TEND = 300.0; OUT = 5.0

A = run_ode(Emergence, p; tend = TEND, saveat = OUT)
finalA = Emergence.outcome_ode(A, p)
@printf("R0 = β/γ = %.2f   |   A (ODE) : taille finale R∞ = %.0f  (%.0f%% de N=%d)\n",
        p.beta / p.gamma, finalA, 100 * finalA / N, N)

println("\n--- écart A→B : B bien mélangé sur 12 graines (fade-out attendu) ---")
nmajor = 0
for s in 1:12
    B = run_ssa(Emergence, p, empty_graph(N); wellmixed = true, tend = TEND, out_step = OUT, seed = s)
    fin = Emergence.outcome_ssa(B, p)
    major = fin > 0.2 * N
    global nmajor += major
    @printf("  graine %2d : R∞ = %4.0f   %s\n", s, fin, major ? "MAJEURE" : "avortée (fade-out)")
end
@printf("→ %d/12 majeures. L'ODE prédit TOUJOURS une épidémie, le stochastique non : écart A→B isolé.\n", nmajor)

println("\n--- convergence C→A via le degré (small-world, ⟨k⟩ → N) ---")
for k in (4, 8, 16, 32, 64, 128)
    vals = Float64[]
    for r in 1:4
        g = smallworld_graph(N, k, 0.1, 100 + r)
        C = run_ssa(Emergence, p, g; wellmixed = false, tend = TEND, out_step = OUT, seed = 100 + r)
        push!(vals, Emergence.outcome_ssa(C, p))
    end
    m = sum(vals) / length(vals)
    @printf("  ⟨k⟩=%3d : C moyen = %5.0f   (%.0f%% de A)\n", k, m, 100 * m / max(finalA, 1))
end
println("→ attendu : C croît avec ⟨k⟩ et rejoint A quand ⟨k⟩→N (frontière de validité).")
