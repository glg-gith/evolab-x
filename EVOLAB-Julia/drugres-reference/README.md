# drugres-reference — instrument DrugRes validé (GELÉ)

Implémentation **DrugRes-spécifique** d'origine de l'expérience A/B/C, validée en
interne. Elle a servi de premier instrument de recherche ; elle est maintenant
**gelée** et conservée comme **référence de validation** du modèle générique
`src/models/DrugRes.jl` (qui doit retrouver ses chiffres).

> ⚠️ **Ne pas modifier.** Code figé. Pour faire évoluer la démo, travailler dans
> `src/` (moteur générique + modèles). Cette archive est auto-contenue : elle a sa
> propre copie du noyau (`DrugResModel.jl`) et ne dépend de rien dans `src/`.

## Contenu

- `DrugResModel.jl` — noyau de domaine (génotypes, matrice VTC, recombinaison,
  mutation), traduit du C++ d'origine. *(La démo vivante en a une copie maintenue,
  `src/models/DrugResCore.jl`.)*
- `ResolutionA_ODE.jl` — A : champ moyen déterministe (OrdinaryDiffEq).
- `ResolutionBC_ABM.jl` — B & C : Gillespie individu-centré (Agents.jl).
- `Indices.jl` — les 3 familles d'indices (F1 épidémique, F2 évolutif, F3 structurel).

## Lancer (depuis `EVOLAB-Julia/`)

```bash
~/.juliaup/bin/julialauncher --project=. drugres-reference/run_minimal.jl        # Figure 1 : A/B/C
~/.juliaup/bin/julialauncher --project=. drugres-reference/sweep_validity.jl     # frontière de validité vs degré
~/.juliaup/bin/julialauncher --project=. drugres-reference/evolution_endemic.jl  # axe évolutif (endémique)
~/.juliaup/bin/julialauncher --project=. drugres-reference/validate_vs_eee.jl    # validation vs C++ d'origine
```

Résultats validés (defaults, N=1000) : prévalence d'équilibre A=641, B=639±9,
C(small-world k=6)=2.4±1.1 ; B≈A, C(graphe complet)≈B.
