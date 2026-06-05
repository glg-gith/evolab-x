using Printf
const SRC = normpath(joinpath(@__DIR__, "..", "src"))
include(joinpath(SRC, "framework", "Runners.jl"))
include(joinpath(SRC, "models", "Parvir.jl"))
using .Runners

meanvir(g, p) = (n = sum(count_class(g, i + 1) for i in 1:p.ngeno);
                 n == 0 ? 0.0 : sum(Parvir.vir(p, i) * count_class(g, i + 1) for i in 1:p.ngeno) / n)

L = 24
p = merge(Parvir.fixed_params(L), (b0 = 6.0, coef = 0.5, mu = 1.0, mut = 0.008, I0frac = 0.4))
TEND = 150.0; OUT = 3.0
A = run_ode(Parvir, p; tend = TEND, saveat = OUT)
B = run_ssa(Parvir, p, empty_graph(p.N); wellmixed = true,  tend = TEND, out_step = OUT, seed = 1)
C = run_ssa(Parvir, p, lattice_graph(L); wellmixed = false, tend = TEND, out_step = OUT, seed = 1)

@printf("départ : virulence = %.2f (sur axe 0..6)\n", Parvir.vir(p, cld(p.ngeno, 2)))
@printf("prévalence eq : A=%.0f  B=%.0f  C=%.0f  (N=%d)\n",
        Runners.tailmean(A.prev, .3), Runners.tailmean(B.prev, .3), Runners.tailmean(C.prev, .3), p.N)
@printf("VIRULENCE évoluée : B (bien mélangé) = %.2f   |   C (grille) = %.2f\n",
        meanvir(B.gstate, p), meanvir(C.gstate, p))
println("→ attendu : C (spatial) évolue une virulence PLUS BASSE que B (auto-étouffement)")
