# Installe les dépendances dans un environnement local au dossier EVOLAB-Julia.
# Usage :  julia scripts/setup.jl
#
# (On ajoute par NOM plutôt que par UUID : Pkg résout les versions et génère
#  Project.toml + Manifest.toml. Robuste sans connaître les UUID à l'avance.)

import Pkg

projectdir = normpath(joinpath(@__DIR__, ".."))
Pkg.activate(projectdir)

deps = ["Agents", "Graphs", "OrdinaryDiffEq"]   # Random/Statistics sont dans la stdlib

println("Activation de l'environnement : ", projectdir)
for d in deps
    println("  + ", d)
    Pkg.add(d)
end
Pkg.precompile()
println("\nOK. Lance ensuite :  julia --project=. scripts/run_minimal.jl")
