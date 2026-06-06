# Cas limites par modèle — réglages du dashboard

Réglages concrets pour **voir** chaque écart entre les trois résolutions. Tous les
chiffres ci-dessous sont **mesurés** (`scripts/limit_cases.jl`), pas théoriques.

Rappel des écarts :
- **A→B** = effet de la stochasticité (extinction, fade-out). Sans structure.
- **B→C** = effet de la structure spatiale (corrélations de voisinage).
- **A≈B≈C** = le champ moyen est valide (rien à voir, et c'est le point : il n'a *pas*
  toujours tort).

## Quel modèle montre quel cas ? (résumé)

| Modèle | A→B (stochastique) | B→C (prévalence s'effondre) | A≈B≈C (validité) | Divergence évolutive |
|---|:---:|:---:|:---:|:---:|
| **SIR** | — (super-seuil, A≈B) | ✅ net (C=58 vs 900) | ✅ (small-world ⟨k⟩↑) | — |
| **SEIR** | — | ✅ net (C=28 vs 900) | ✅ (⟨k⟩↑) | — |
| **Émergence** | ✅ **son but** | ✅ (s'effondre aussi) | ✅ (β élevé) | — |
| **Parvir** | — | ❌ reste endémique | ≈ d'emblée | ✗ aucune à l'équilibre (le « C>B » est un transitoire) |
| **DrugRes** | — | ✅ net (C→0) | ✅ (⟨k⟩↑) | ⚠ hors dashboard |

Pour voir **A→B**, c'est Émergence. Pour voir **B→C**, c'est SIR/SEIR/DrugRes.
Parvir ne s'effondre pas (population à taille constante) → il sert d'exemple où le
spatial **ne change rien à l'équilibre** (l'écart de virulence apparent n'est qu'un
transitoire, cf. plus bas).

---

## SIR

Défauts : `β=0.05, γ=0.1, I0=6`, `L=30`. (Observable = taille finale R∞ ; N=900.)

| Cas | Réglages | Résultat mesuré |
|---|---|---|
| **B→C (l'épidémie spatiale s'étouffe)** | Structure = **Grille 2D** | A=900 · B=900 · **C=58** |
| **A≈B≈C (champ moyen valide)** | Structure = **Small-world**, **⟨k⟩=96** | A=900 · B=900 · **C=900** |

→ Passe la structure de *Grille 2D* à *Small-world* et monte ⟨k⟩ : C remonte de 58
vers 900. C'est la frontière de validité, en direct. (A→B n'apparaît pas : SIR par
défaut est très super-seuil, B colle à A.)

## SEIR

Défauts : `β=0.05, σ=0.2, γ=0.1, I0=6`, `L=30`. Même histoire que SIR, le compartiment
*exposé* retarde la vague.

| Cas | Réglages | Résultat mesuré |
|---|---|---|
| **B→C** | Structure = **Grille 2D** | A=900 · B=900 · **C=28** |
| **A≈B≈C** | **Small-world**, **⟨k⟩** élevé (≥96) | C rejoint A (comme SIR) |

## Émergence — *le* modèle de l'écart A→B

Calé au seuil exprès. Observable = taille finale R∞ (N=900).

| Cas | Réglages | Résultat mesuré |
|---|---|---|
| **A→B (fade-out stochastique)** | `β=0.14, γ=0.10, I0=2` (défauts) — puis **bouge la graine** | A=**462 toujours** ; B = **4 graines sur 12 en épidémie majeure, 8 avortées** (R∞ de 2 à 551) |
| **A≈B (au-dessus du seuil)** | `β=0.30` (R0=3) | A=847 ; B = **11/12 majeures** → B colle à A |

→ Le geste clé : laisse `β=0.14` et **fais défiler la graine**. A part toujours en
épidémie ; B (sans aucune structure) bascule entre flambée et avortement. L'écart est
purement stochastique. *(En Grille 2D, C s'effondre encore plus — sous-seuil spatial.)*

## Parvir — le « cas-limite » qui n'en est pas (artefact transitoire)

Défauts : `b0=6, coef=0.5, mu=1, mut=0.008, I0frac=0.4`, `L=24`. SIS à taille constante :
**ne s'effondre pas**, donc pas de cas B→C de prévalence.

Observation brute (graine 1, T=150) : virulence évoluée **B=2.38 · C=2.70** (axe 0–6,
départ 3.0) → on dirait que la structure *augmente* la virulence, **à rebours de la
théorie** (la dispersion locale devrait la *baisser* : auto-étouffement). **C'est un faux
positif**, vérifié sur ensembles (`scripts/investigate_parvir.jl`) :

- **Non significatif entre graines** : sur 16 graines à T=150, C−B = **+0.25 ± 0.15** (SEM),
  t≈1.7 → dans le bruit.
- **Pur transitoire** : A (ODE, T=4000) évolue vers une virulence d'équilibre de **1.00**
  (départ 3.0). B et C y descendent tous deux ; C juste **plus lentement** (moins de contacts,
  prévalence plus basse) → à temps fixe C reste plus près du départ → C>B *apparent*. La
  convergence (n=10 graines, aucun run tronqué, `t̄ = T`) :

| T | B (±SEM) | C (±SEM) | C−B |
|---:|:---:|:---:|:---:|
| 150 | 2.12 ± 0.16 | 2.37 ± 0.10 | +0.25 |
| 400 | 1.13 ± 0.06 | 1.40 ± 0.06 | +0.26 |
| 900 | 1.02 ± 0.04 | 1.06 ± 0.05 | +0.05 |
| 1500 | 1.04 ± 0.03 | 1.01 ± 0.03 | −0.03 |

→ ⚠ **Ni cas-limite, ni divergence évolutive spatiale ici.** Dès que les deux résolutions
sont équilibrées (T≳900), B et C rejoignent l'optimum champ moyen (≈1.0) et l'écart
s'annule. Le « C plus virulent » était le **piège du matching** : comparer à temps fixe
deux dynamiques de vitesses différentes. *(Portée : établi pour ce point de paramètres,
trade-off concave `coef=0.5`. La réduction spatiale de virulence dépend de la courbure du
trade-off et de la portée de dispersion — non balayées ici.)*

## DrugRes — B→C maximal, mais évolution hors-portée du dashboard

Défauts : `θ=10, μ=0.01, tho=0.8, σ=1, rp=0.2, mut=0.01`, `L=24`.

| Cas | Réglages | Résultat mesuré |
|---|---|---|
| **B→C (le pathogène n'envahit pas le réseau)** | Structure = **Grille 2D** | A=515 · B=526 · **C=0** (extinction spatiale) |

→ C s'effondre totalement (R0 réseau < 1). A≈B (~500–640 selon N). Monter ⟨k⟩
ramènerait C vers A (frontière, cf. l'archive : k=48→25 %, k=96→68 %, k=192→86 %).

⚠ **La divergence évolutive de DrugRes (composition génotypique) n'est pas atteignable
depuis le dashboard** : la transmission β est dans la matrice VTC (fixe), pas un slider.
Pour ce cas, utiliser l'archive gelée :
`drugres-reference/evolution_endemic.jl` (régime endémique forcé → génotype AB :
A=0.385, B=0.397, **C=0.250**).

---

*Reproduire ces chiffres :* `~/.juliaup/bin/julialauncher --project=. scripts/limit_cases.jl`
(une seule graine par cas dans le script ; les valeurs stochastiques B/C bougent un
peu d'une graine à l'autre — c'est le sujet).
