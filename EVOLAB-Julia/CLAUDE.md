# EVOLAB-Julia — guide du dépôt

Expérience « individu vs champ moyen » en épidémiologie spatiale et évolutive.
On compare **trois résolutions** du même modèle microscopique et on regarde *où*
et *de combien* le champ moyen ment :

- **A** — champ moyen déterministe (ODE).
- **B** — stochastique **bien mélangé** (Gillespie, graphe complet implicite).
- **C** — stochastique **structuré** (Gillespie sur réseau/grille).

Écart A→B = stochasticité / fade-out. Écart B→C = corrélations spatiales. Le but
est de *décomposer* l'erreur, donc on garde les trois, jamais A vs C seul.

## Structure

- **`src/` + `scripts/` + `dashboard.jl` — la démo (code vivant).** Moteur générique
  `src/framework/Runners.jl` (mécanique A/B/C, indépendante du modèle), modèles
  `src/models/*.jl`, visualisation `src/Viz.jl`, notebook Pluto `dashboard.jl`.
  Multi-modèles, interactif. **Pas un livrable reproductible** — un effet vu en
  bougeant un slider se refait en script avant de devenir un résultat.
- **`drugres-reference/` — instrument DrugRes validé, GELÉ.** L'implémentation
  DrugRes-spécifique d'origine (`ResolutionA_ODE.jl` = A/ODE, `ResolutionBC_ABM.jl` =
  B&C/Agents.jl, `Indices.jl`, + ses scripts). Sert de **référence de validation** :
  `src/models/DrugRes.jl` doit retrouver ses chiffres. Ne pas modifier (cf. son README).

Le noyau de domaine DrugRes (génotypes, matrice VTC, `recombine`, `mutate`,
distributions, traduit du C++) est dans `src/models/DrugResCore.jl` — vivant, importé
par `src/models/DrugRes.jl`. L'archive gelée en garde une **copie figée**
(`drugres-reference/DrugResModel.jl`), donc **aucune dépendance vivant→gelé**.

## Écrire un nouveau modèle

Un modèle = un module exposant cette convention (le moteur l'appelle par duck typing ;
voir l'en-tête de `src/framework/Runners.jl` et `src/models/SIR.jl` comme gabarit
minimal). Fonctions requises :

| fonction | rôle |
|---|---|
| `nclasses(p)` | nombre de classes (états) d'un nœud |
| `prevalence_classes(p)` | classes comptées comme « infectés » (tuple) |
| `init_classes(p, N, rng)` | vecteur classe-par-nœud initial (SSA) |
| `rates(g)::Tuple` | taux de chaque canal d'événement, dans l'ordre |
| `execute!(g, k)` | applique l'événement `k` (= index du canal tiré) |
| `init_u0(p)` / `meanfield_rhs!(du,u,p,t)` / `u_prevalence(u,p)` | champ moyen (A) |
| `outcome_ssa(res,p)` / `outcome_ode(res,p)` | observable de sortie (barres/frontière) |
| `param_specs()` | sliders : `(:nom, min, max, défaut)` |
| `fixed_params(L)` | params non-réglables (au moins `N = L*L`) |
| `suggested_tend(p)` | horizon temporel par défaut |
| `state_names()` / `state_palette(p)` | étiquettes + couleurs des classes |

Puis l'ajouter à `dashboard.jl` : `include` + entrée dans `MODELS`.

**Cohérence A/B/C** : la transmission doit être **par contact** et donner les mêmes
taons dans les trois résolutions (cf. `SIR.jl` : C = β·#arêtes S-I, B = β·I·S,
A = βSI). Pour fixer R0 indépendamment de la structure (isoler la stochasticité),
normaliser le taux par contact par `N` (bien mélangé) ou `⟨k⟩ = 2·ne/nv` (réseau) —
voir `Emergence.jl` et `Parvir.jl`.

## Lancer

Julia 1.12 via juliaup. **Pas de symlink `julia` dans le PATH** — le binaire est
`~/.juliaup/bin/julialauncher`.

```bash
~/.juliaup/bin/julialauncher --project=. -e 'using Pkg; Pkg.precompile()'   # 1re fois
~/.juliaup/bin/julialauncher --project=. dashboard.jl                       # smoke headless
~/.juliaup/bin/julialauncher --project=. scripts/test_emergence.jl          # valide A→B + C→A
~/.juliaup/bin/julialauncher --project=. scripts/make_figure.jl             # régénère le PNG
```

Le dashboard se lance interactif avec Pluto (`using Pluto; Pluto.run()` puis ouvrir
`dashboard.jl`). En headless il s'exécute comme un script (les `@bind` prennent leur
valeur par défaut) — c'est notre smoke test.

## Pièges (vécus)

- **Ne jamais lancer deux compilations Julia lourdes en parallèle.** Julia 1.12.6
  segfaulte dans son GC (`gc_mark_outrefs`, signal 11) sous compilation concurrente de
  Makie/OrdinaryDiffEq. Un segfault *dans le compilateur* (pas un `ERROR:` Julia) →
  relancer SEUL avant de suspecter le code.
- `Manifest.toml` est **commité** (reproductibilité). Si un cache `.ji` est corrompu
  par un crash, `Pkg.precompile()` le régénère.
- `Select` (PlutoUI) lie la **clé** d'une paire `valeur => "label"`, pas le label.
- `watts_strogatz` exige un degré **pair** et `< N` (`Viz.run_main` le borne déjà).
