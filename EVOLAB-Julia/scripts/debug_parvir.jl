using Random: Xoshiro
const SRC = normpath(joinpath(@__DIR__, "..", "src"))
include(joinpath(SRC, "framework", "Runners.jl"))
include(joinpath(SRC, "models", "Parvir.jl"))
using .Runners

L = 20
p = merge(Parvir.fixed_params(L), (b0 = 6.0, coef = 0.5, mu = 1.0, mut = 0.006, I0frac = 0.4))
classes = Parvir.init_classes(p, p.N, Xoshiro(1))
g = GState(Parvir.nclasses(p), classes, lattice_graph(L), false, p, 2)

Itot = sum(count_class(g, i + 1) for i in 1:p.ngeno)
println("S = ", count_class(g, 1), "   Itot = ", Itot, "   nclasses = ", Parvir.nclasses(p))
mid = cld(p.ngeno, 2)
println("pool du génotype médian (classe ", mid + 1, ") : ", count_class(g, mid + 1), " nœuds")
v = nodes_of(g, mid + 1)[1]
println("nœud infecté ex. : ", v, "  voisins S (nbr_count) = ", nbr_count(g, 1, v),
        "  betaf(mid) = ", Parvir.betaf(p, mid))
println("RATES init = ", Parvir.rates(g), "   (infection, death, mutation)")
