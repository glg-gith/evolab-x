### A Pluto.jl notebook ###
# v0.20.4

using Markdown
using InteractiveUtils

# This Pluto notebook uses @bind for interactivity. When running this notebook outside of Pluto, the following 'mock version' of @bind gives bound variables a default value (instead of an error).
macro bind(def, element)
    #= EVOLAB dashboard =#
    quote
        local iv = try Base.loaded_modules[Base.PkgId(Base.UUID("6e696c72-6542-2067-7265-42206c756150"), "AbstractPlutoDingetjes")].Bonds.initial_value catch; b -> missing; end
        local el = $(esc(element))
        global $(esc(def)) = Core.applicable(Base.get, el) ? Base.get(el) : iv(el)
        el
    end
end

# ╔═╡ 11111111-1111-1111-1111-111111111101
md"""
# EVOLAB — Individu vs champ moyen
Compare **A** (champ moyen, ODE), **B** (bien mélangé, stochastique) et **C** (grille
spatiale, stochastique), et trace la **frontière de validité**. Choisis un modèle, bouge
les sliders. *(Parvir et DrugRes sont génotypés → plus lents : compte quelques secondes.)*
"""

# ╔═╡ 11111111-1111-1111-1111-111111111102
begin
    using Pkg
    Pkg.activate(@__DIR__)
    using PlutoUI
    import CairoMakie
    CairoMakie.activate!()
    include(joinpath(@__DIR__, "src", "framework", "Runners.jl"))
    include(joinpath(@__DIR__, "src", "DrugResModel.jl"))
    include(joinpath(@__DIR__, "src", "models", "SIR.jl"))
    include(joinpath(@__DIR__, "src", "models", "SEIR.jl"))
    include(joinpath(@__DIR__, "src", "models", "Parvir.jl"))
    include(joinpath(@__DIR__, "src", "models", "DrugResL2.jl"))
    include(joinpath(@__DIR__, "src", "Viz.jl"))
    import .Runners, .Viz, .SIR, .SEIR, .Parvir, .DrugResL2
    MODELS = ["SIR" => SIR, "SEIR" => SEIR, "Parvir (virulence)" => Parvir, "DrugRes" => DrugResL2]
    md"*(moteur chargé — $(length(MODELS)) modèles)*"
end

# ╔═╡ 11111111-1111-1111-1111-111111111103
md"""**Modèle** : $(@bind modelname Select([k for (k, v) in MODELS]))"""

# ╔═╡ 11111111-1111-1111-1111-111111111104
model = Dict(MODELS)[modelname]

# ╔═╡ 11111111-1111-1111-1111-111111111105
@bind P PlutoUI.combine() do Child
    frags = [md""" **$(string(s[1]))** : $(Child(string(s[1]), Slider(range(s[2], s[3], length = 60), default = s[4], show_value = true))) """ for s in model.param_specs()]
    md""" $(frags) """
end

# ╔═╡ 11111111-1111-1111-1111-111111111106
md"""Grille L = $(@bind L Slider(20:5:50, default = 30, show_value = true))   ·   graine = $(@bind seed Slider(1:50, default = 1, show_value = true))"""

# ╔═╡ 11111111-1111-1111-1111-111111111107
params = merge(model.fixed_params(L), P)

# ╔═╡ 11111111-1111-1111-1111-111111111108
begin
    TEND = model.suggested_tend(params)
    OUT = TEND / 40
end

# ╔═╡ 11111111-1111-1111-1111-111111111109
A, B, C = Viz.run_main(model, params, L; tend = TEND, out = OUT, seed = seed)

# ╔═╡ 11111111-1111-1111-1111-111111111110
md"""Instant affiché : $(@bind frame Slider(1:max(1, length(C.snaps)), default = max(1, length(C.snaps) ÷ 2), show_value = true))"""

# ╔═╡ 11111111-1111-1111-1111-111111111111
Viz.plot_spatial(model, params, C, L, frame)

# ╔═╡ 11111111-1111-1111-1111-111111111112
Viz.plot_trajectories(A, B, C)

# ╔═╡ 11111111-1111-1111-1111-111111111113
Viz.plot_bars(model, params, A, B, C)

# ╔═╡ 11111111-1111-1111-1111-111111111114
@bind go_frontier Button("Calculer la frontière de validité")

# ╔═╡ 11111111-1111-1111-1111-111111111115
begin
    go_frontier
    fr = Viz.run_frontier(model, params; N = L * L, degrees = [4, 8, 16, 32, 64],
                          reps = 2, tend = TEND, out = OUT)
    Viz.plot_frontier(fr)
end

# ╔═╡ Cell order:
# ╟─11111111-1111-1111-1111-111111111101
# ╟─11111111-1111-1111-1111-111111111102
# ╟─11111111-1111-1111-1111-111111111103
# ╟─11111111-1111-1111-1111-111111111104
# ╟─11111111-1111-1111-1111-111111111105
# ╟─11111111-1111-1111-1111-111111111106
# ╟─11111111-1111-1111-1111-111111111107
# ╟─11111111-1111-1111-1111-111111111108
# ╟─11111111-1111-1111-1111-111111111109
# ╟─11111111-1111-1111-1111-111111111110
# ╟─11111111-1111-1111-1111-111111111111
# ╟─11111111-1111-1111-1111-111111111112
# ╟─11111111-1111-1111-1111-111111111113
# ╟─11111111-1111-1111-1111-111111111114
# ╟─11111111-1111-1111-1111-111111111115
