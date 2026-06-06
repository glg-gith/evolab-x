# EVOLAB-Julia — démo « individu vs champ moyen »

Démo interactive qui montre **quand le champ moyen (ODE) ment**, en comparant trois
résolutions du même modèle microscopique :

- **A** — champ moyen déterministe (ODE)
- **B** — stochastique **bien mélangé** (Gillespie, contacts au hasard)
- **C** — stochastique **structuré** (Gillespie sur grille / réseau)

Écart **A→B** = stochasticité (extinction, fade-out). Écart **B→C** = corrélations
spatiales. On ne compare jamais A à C directement : les trois ensemble *décomposent*
l'erreur.

## Prérequis

Julia 1.12 via juliaup. Le binaire s'appelle `julialauncher` (pas de symlink `julia`).
Première fois — installer et précompiler les dépendances :

```bash
cd EVOLAB-Julia
~/.juliaup/bin/julialauncher --project=. -e 'using Pkg; Pkg.instantiate(); Pkg.precompile()'
```

## Lancer la démo interactive (le mode normal)

```bash
~/.juliaup/bin/julialauncher --project=. -e 'using Pluto; Pluto.run(notebook="dashboard.jl")'
```

Pluto démarre et ouvre le notebook dans le navigateur (1re fois : ~1 min de
compilation). Chaque mouvement de slider recalcule A, B et C en direct.
*(Variante : `Pluto.run()` sans argument, puis ouvrir `dashboard.jl` depuis la page
d'accueil Pluto.)*

## Ce que tu peux régler

| Contrôle | Effet |
|---|---|
| **Modèle** | SIR · SEIR · Émergence · Parvir (virulence) · DrugRes (résistance) |
| **Sliders de paramètres** | propres à chaque modèle (β, γ, virulence…) |
| **Grille L** | taille de la population N = L×L |
| **Graine** | le tirage aléatoire (change l'issue stochastique de B et C) |
| **Structure de contact de C** | *Grille 2D* (spatial) ou *Small-world* avec ⟨k⟩ et recâblage β réglables |
| **Bouton « frontière de validité »** | trace l'écart C/A en fonction du degré ⟨k⟩ |

Panneaux affichés : carte spatiale de C · trajectoires A/B/C · barres d'issue ·
(sur bouton) courbe de frontière.

## Trois expériences à faire (chacune isole un écart)

1. **L'effet spatial (B→C).** Modèle **SIR**, défauts. La courbe C (grille) traîne
   loin sous A et B : l'épidémie avance en tache d'encre au lieu de flamber. Puis
   passe la structure en *Small-world* et **monte ⟨k⟩** : C remonte vers A. À ⟨k⟩→N,
   les trois courbes se rejoignent — *c'est la frontière de validité du champ moyen,
   en direct*. (Convergence la plus nette à petite grille, L=20.)

2. **L'effet stochastique (A→B).** Modèle **Émergence** (calé au seuil). A part
   *toujours* en épidémie. **Bouge la graine** : B (bien mélangé, sans aucune
   structure) bascule entre épidémie majeure et avortement — l'ODE ne voit jamais ce
   fade-out. Ici l'écart est purement stochastique, pas spatial.

3. **L'effet évolutif.** Modèle **Parvir** ou **DrugRes**. La structure ne change pas
   que la prévalence : elle change *quelle souche gagne*. En C (spatial), la souche la
   plus transmissible s'auto-étouffe localement → virulence / résistance évoluée plus
   basse qu'en B. Le champ moyen rate ça qualitativement.

## Sans interaction

- **Figure statique** (PNG multi-panneaux) :
  `~/.juliaup/bin/julialauncher --project=. scripts/make_figure.jl` → `dashboard_preview.png`
- **Smoke test** (vérifie que tout charge et tourne) :
  `~/.juliaup/bin/julialauncher --project=. dashboard.jl`
- **Tests par modèle** : `scripts/test_{framework,parvir,emergence,drugres}.jl`

> ⚠️ Ne pas lancer deux compilations Julia lourdes **en parallèle** (Makie +
> OrdinaryDiffEq) : Julia 1.12.6 segfaulte dans son GC. Une à la fois.

## Aller plus loin

- Ajouter un modèle : voir la convention dans `CLAUDE.md` (table des fonctions à
  exposer) et `src/models/SIR.jl` comme gabarit minimal.
- `drugres-reference/` est l'**instrument DrugRes validé, gelé** (référence de
  validation, reproductible par script) — voir son propre README.
