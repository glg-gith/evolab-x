"""
    Viz — couche de visualisation (CairoMakie), partagée entre le PNG headless
    et le notebook Pluto. Indépendante du modèle (utilise la convention L2).
"""
module Viz

using CairoMakie
using Graphs: nv
using ..Runners

export run_main, run_frontier, plot_spatial, plot_trajectories, plot_frontier, plot_bars, dashboard_figure

"Palette catégorielle d'un modèle (1 couleur par classe), via `state_palette(p)`."
palette(model, p) = cgrad(model.state_palette(p), model.nclasses(p), categorical = true)

"""
    run_main(model, params, L; tend, out, seed)

Calcule les 3 résolutions « live » : A (ODE), B (bien mélangé) et C (grille L×L,
avec snapshots spatiaux). Léger → adapté à un slider.
"""
function run_main(model, params, L; tend, out, seed)
    N = L * L
    A = run_ode(model, params; tend = tend, saveat = out)
    B = run_ssa(model, params, empty_graph(N); wellmixed = true,
                tend = tend, out_step = out, seed = seed)
    C = run_ssa(model, params, lattice_graph(L); wellmixed = false,
                tend = tend, out_step = out, seed = seed, record_spatial = true)
    return A, B, C
end

run_frontier(model, params; N, degrees, reps, tend, out) =
    frontier(model, params; N = N, degrees = degrees, reps = reps, tend = tend, out_step = out)

# --- panneaux individuels (chacun renvoie une Figure, pour Pluto) ---

function plot_spatial(model, params, C, L, frame)
    fig = Figure(size = (430, 430))
    ax = Axis(fig[1, 1], aspect = DataAspect(),
              title = "Propagation spatiale — grille $(L)×$(L)" *
                      (isempty(C.t) ? "" : ", t≈$(round(C.t[clamp(frame,1,length(C.t))],digits=0))"))
    if !isempty(C.snaps)
        snap = reshape(C.snaps[clamp(frame, 1, length(C.snaps))], L, L)
        heatmap!(ax, snap; colormap = palette(model, params), colorrange = (1, model.nclasses(params)))
    end
    hidedecorations!(ax); hidespines!(ax)
    return fig
end

function plot_trajectories(A, B, C)
    fig = Figure(size = (560, 360))
    ax = Axis(fig[1, 1], xlabel = "temps", ylabel = "nb infectés (prévalence)",
              title = "A (champ moyen) vs B (bien mélangé) vs C (grille)")
    lines!(ax, A.t, A.prev, label = "A — champ moyen", linewidth = 3, color = :gray25)
    lines!(ax, B.t, B.prev, label = "B — bien mélangé", linewidth = 2, linestyle = :dash, color = :dodgerblue)
    lines!(ax, C.t, C.prev, label = "C — grille (spatial)", linewidth = 3, color = :crimson)
    axislegend(ax, position = :rt)
    return fig
end

function plot_frontier(fr)
    fig = Figure(size = (430, 360))
    ax = Axis(fig[1, 1], xlabel = "degré moyen k", ylabel = "issue  C / A  (%)",
              title = "Frontière de validité du champ moyen", xscale = log2)
    lines!(ax, fr.k, 100 .* fr.ratio, color = :black)
    scatter!(ax, fr.k, 100 .* fr.ratio, markersize = 12, color = :black)
    hlines!(ax, [95.0], color = :seagreen, linestyle = :dot)
    return fig
end

function plot_bars(model, params, A, B, C)
    fig = Figure(size = (430, 360))
    ax = Axis(fig[1, 1], xticks = (1:3, ["A", "B", "C"]),
              ylabel = "issue (taille finale / prévalence eq.)",
              title = "Issue par résolution")
    vals = [model.outcome_ode(A, params), model.outcome_ssa(B, params), model.outcome_ssa(C, params)]
    barplot!(ax, 1:3, vals, color = [:gray40, :dodgerblue, :crimson])
    return fig
end

"Figure combinée (pour export PNG headless)."
function dashboard_figure(model, params, L, A, B, C, frame; fr = nothing)
    fig = Figure(size = (1150, 850))
    ax1 = Axis(fig[1, 1], aspect = DataAspect(),
               title = "Propagation spatiale (grille $(L)×$(L), t≈$(round(C.t[clamp(frame,1,length(C.t))],digits=0)))")
    if !isempty(C.snaps)
        heatmap!(ax1, reshape(C.snaps[clamp(frame,1,length(C.snaps))], L, L);
                 colormap = palette(model, params), colorrange = (1, model.nclasses(params)))
    end
    hidedecorations!(ax1); hidespines!(ax1)

    ax2 = Axis(fig[1, 2], xlabel = "temps", ylabel = "nb infectés",
               title = "Prévalence I(t) : A vs B vs C")
    lines!(ax2, A.t, A.prev, label = "A — champ moyen", linewidth = 3, color = :gray25)
    lines!(ax2, B.t, B.prev, label = "B — bien mélangé", linewidth = 2, linestyle = :dash, color = :dodgerblue)
    lines!(ax2, C.t, C.prev, label = "C — grille", linewidth = 3, color = :crimson)
    axislegend(ax2, position = :rt)

    if fr !== nothing
        ax3 = Axis(fig[2, 1], xlabel = "degré moyen k", ylabel = "issue C / A (%)",
                   title = "Frontière de validité", xscale = log2)
        lines!(ax3, fr.k, 100 .* fr.ratio, color = :black)
        scatter!(ax3, fr.k, 100 .* fr.ratio, markersize = 12, color = :black)
        hlines!(ax3, [95.0], color = :seagreen, linestyle = :dot)
    end

    ax4 = Axis(fig[2, 2], xticks = (1:3, ["A", "B", "C"]), title = "Issue d'épidémie",
               ylabel = "taille finale / prév. eq.")
    barplot!(ax4, 1:3, [model.outcome_ode(A, params), model.outcome_ssa(B, params), model.outcome_ssa(C, params)],
             color = [:gray40, :dodgerblue, :crimson])

    Label(fig[0, :], "EVOLAB — Individu vs champ moyen", fontsize = 20, font = :bold)
    return fig
end

end # module
