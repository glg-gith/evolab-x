# Cross-check du modèle DrugRes (moteur générique) vs valeur validée (~640 à N~1000).
using Printf
const SRC = normpath(joinpath(@__DIR__, "..", "src"))
include(joinpath(SRC, "models", "DrugResCore.jl"))
include(joinpath(SRC, "framework", "Runners.jl"))
include(joinpath(SRC, "models", "DrugRes.jl"))
using .Runners

L = 32; N = L * L    # ≈ 1024 ~ nb_sites 1000 du modèle d'origine
p = merge(DrugRes.fixed_params(L), (theta = 10.0, mu = 0.01, tho = 0.8, sigma = 1.0, rp = 0.2, mut = 0.01))
TEND = 3000.0; OUT = 25.0

t0 = time()
A = run_ode(DrugRes, p; tend = TEND, saveat = OUT)
B = run_ssa(DrugRes, p, empty_graph(N); wellmixed = true, tend = TEND, out_step = OUT, seed = 1,
            max_seconds = 40.0)
@printf("N=%d  (réf validée ≈ 640)\n", N)
@printf("  A (ODE champ moyen)  prévalence eq = %.0f\n", Runners.tailmean(A.prev, 0.3))
@printf("  B (bien mélangé)     prévalence eq = %.0f   [t atteint=%.0f]\n",
        Runners.tailmean(B.prev, 0.3), isempty(B.t) ? -1 : B.t[end])
@printf("  temps total : %.1fs\n", time() - t0)
