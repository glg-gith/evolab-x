# Génère la figure multi-panneaux (headless) via Viz — valide toute la visu.
using CairoMakie
const SRC = normpath(joinpath(@__DIR__, "..", "src"))
include(joinpath(SRC, "framework", "Runners.jl"))
include(joinpath(SRC, "models", "SIR.jl"))
include(joinpath(SRC, "Viz.jl"))
using .Runners, .Viz

L = 45; N = L * L
p = (N = N, beta = 0.08, gamma = 0.1, I0 = 4)    # super-seuil sur grille -> vague visible
TEND = 260.0; OUT = 4.0

A, B, C = run_main(SIR, p, L; tend = TEND, out = OUT, seed = 7)
fr = run_frontier(SIR, p; N = N, degrees = [4, 8, 16, 32, 64], reps = 2, tend = TEND, out = OUT)
frame = clamp(argmax(C.prev), 1, length(C.snaps))

fig = dashboard_figure(SIR, p, L, A, B, C, frame; fr = fr)
out = normpath(joinpath(@__DIR__, "..", "dashboard_preview.png"))
save(out, fig)
println("Figure : ", out)
println("A final=", round(SIR.outcome_ode(A,p)), " | B final=", round(SIR.outcome_ssa(B,p)),
        " | C final=", round(SIR.outcome_ssa(C,p)), " | frame=", frame, "/", length(C.snaps))
