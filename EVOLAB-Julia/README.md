# EVOLAB-Julia — DrugRes : individu vs champ moyen

Réimplémentation **propre** du modèle `DrugRes` (S-I-S évolutif de S. Gandon)
issu d'EVOLAB-X, en Julia, dans le but de **chiffrer l'écart** entre trois
résolutions d'un *même* modèle microscopique :

| | Résolution | Outil | Ce qu'elle ajoute |
|---|---|---|---|
| **A** | champ moyen déterministe (ODE) | OrdinaryDiffEq | baseline naïve |
| **B** | stochastique **bien mélangé** | Agents.jl | stochasticité / minorité rare |
| **C** | stochastique **réseau** | Agents.jl + Graphs | corrélation spatiale |

L'écart **A→B** isole l'effet de la stochasticité ; **B→C** isole l'effet de la
structure. La version C++ d'origine correspond à **B** (référence de validation).

> ⚠️ **Code non testé.** Julia n'était pas installé sur la machine au moment de
> l'écriture. Le code est écrit pour être correct et lisible, mais **n'a pas été
> exécuté**. Voir la checklist de validation en bas.

## Principe méthodologique (à ne pas casser)

- **Matching au niveau microscopique** : A, B et C partagent EXACTEMENT les mêmes
  taux par événement (β par contact, γ, mutation, matrice VTC). On laisse les
  observables macro (R0, prévalence) **diverger** — la divergence est le signal.
- **Ne jamais re-calibrer l'ODE sur l'ABM** : cela effacerait le phénomène étudié.
- En **graphe complet**, C se réduit exactement à B (cohérence par construction).

## Structure

```
EVOLAB-Julia/
├── src/
│   ├── DrugResModel.jl      # cœur partagé : params, matrice VTC, recombinaison, mutation
│   ├── ResolutionA_ODE.jl   # A : champ moyen (ODE)
│   ├── ResolutionBC_ABM.jl  # B & C : Gillespie individu-centré (contact en interrupteur)
│   └── Indices.jl           # les 3 familles d'indices
├── scripts/
│   ├── setup.jl             # installe les dépendances dans un env local
│   └── run_minimal.jl       # expérience minimale A vs B vs C (« Figure 1 »)
└── README.md
```

## Installation de Julia

- **Recommandé (multiplateforme)** : `juliaup`
  ```bash
  curl -fsSL https://install.julialang.org | sh
  # puis, nouveau shell :
  juliaup add release && juliaup default release
  ```
- **Manjaro/Arch** : `sudo pacman -S julia` (ou `yay -S juliaup`).
- Vérifier : `julia --version` (≥ 1.10 conseillé).

## Lancer

```bash
cd EVOLAB-Julia
julia scripts/setup.jl              # installe Agents, Graphs, OrdinaryDiffEq
julia --project=. scripts/run_minimal.jl
```

Le script imprime un tableau comparant les indices A / B / C :
- **F1** prévalence d'équilibre (symptôme épidémique),
- **F2** fréquence du génotype AB + persistance (symptôme évolutif),
- **F3** corrélation S-I (cause structurelle, réseau).

## Faire varier le bouton d'interpolation (version « papier »)

Dans `run_minimal.jl`, `ws_beta` est la proba de recâblage small-world :
`0` = treillis (très structuré) → `1` = aléatoire (≈ bien mélangé).
Balayer `ws_beta` (ou `mean_deg`, ou `N`) et tracer un indice vs le bouton
donne la **frontière de validité** du champ moyen.

## Checklist de validation (avant d'exploiter les résultats)

1. **B ≈ C++** : comparer la trajectoire de B aux sorties de `DrugRes.cpp`
   (mêmes paramètres) — doit coïncider en distribution.
2. **C(graphe complet) ≈ B** : remplacer le graphe de C par `complete_graph(N)`
   doit reproduire B.
3. **A ≈ B à grand N** : augmenter N ; B doit converger vers A.
4. **Conservation** : `S + I + E == N` à tout instant (sites).
5. Surinfection/recombinaison : la partie la plus délicate — vérifier les
   distributions de génotypes contre la logique de `RealizeSuperinfection`.
