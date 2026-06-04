# =============================================================================
#  LÉGENDE DES INDICES  —  F1, F2, F3
# =============================================================================
#
#  Les sorties des scripts (run_minimal, sweep_validity, evolution_endemic)
#  étiquettent leurs lignes F1 / F2 / F3. Voici ce que c'est :
#
#    F1  —  SYMPTÔME ÉPIDÉMIQUE   (sur la COURBE de l'épidémie)
#           prévalence d'équilibre |I|, hauteur et timing du pic.
#           → « de combien l'agrégat se trompe sur la TRANSMISSION »
#
#    F2  —  SYMPTÔME ÉVOLUTIF     (sur l'ÉVOLUTION du pathogène)
#           fréquences des 4 génotypes, fréquence de la souche AB,
#           fraction traitée, variance du trait, persistance.
#           → « de combien l'agrégat se trompe sur la SÉLECTION »
#
#    F3  —  CAUSE STRUCTURELLE    (sur l'ORGANISATION de la population)
#           corrélation S-I (couples voisins S-I vs hasard), clustering.
#           → ce n'est PAS un résultat : c'est l'EXPLICATION du décrochage.
#
#  Logique d'ensemble :  F1 et F2 = SYMPTÔMES (l'écart de résultats)
#                        F3        = CAUSE     (la structure ignorée par le champ moyen)
#  Le but : montrer que l'écart F1/F2 grandit EXACTEMENT quand F3 s'éloigne de 1.
# =============================================================================

"""
    Indices

Les trois familles d'indices qui chiffrent l'écart entre résolutions
(voir la légende F1/F2/F3 en tête de fichier).

Une « série » est le NamedTuple renvoyé par les résolutions :
`(t, S, Itot, Igt[n,4,2], si, label)`.
"""
module Indices

using Statistics
using ..DrugResModel

export family1, family2, family3, summarize, ensemble_summary

"Indice moyen sur la dernière fraction `tail` de la trajectoire (régime quasi-stationnaire)."
_tailmean(x, tail) = mean(@view x[max(1, floor(Int, length(x) * (1 - tail)) + 1):end])

"Famille 1 — symptômes épidémiques."
function family1(s; tail = 0.3)
    eq_prev = _tailmean(s.Itot, tail)
    pk, idx = findmax(s.Itot)
    return (eq_prevalence = eq_prev, peak = pk, peak_time = s.t[idx],
            final_S = _tailmean(s.S, tail))
end

"Fréquence des 4 génotypes (moyennée sur traités+non-traités) au régime quasi-stationnaire."
function _geno_freqs(s; tail = 0.3)
    n = length(s.t)
    rng = max(1, floor(Int, n * (1 - tail)) + 1):n
    freqs = zeros(Float64, NGENO)
    cnt = 0
    for k in rng
        tot = sum(@view s.Igt[k, :, :])
        tot == 0 && continue
        for g in 1:NGENO
            freqs[g] += (s.Igt[k, g, 1] + s.Igt[k, g, 2]) / tot
        end
        cnt += 1
    end
    cnt > 0 && (freqs ./= cnt)
    return freqs
end

"Famille 2 — symptômes évolutifs."
function family2(s; tail = 0.3)
    f = _geno_freqs(s; tail = tail)
    # trait = index de génotype 0:3 ; moyenne et variance pondérées
    vals = 0:3
    mean_g = sum(f[i + 1] * vals[i + 1] for i in 0:3)
    var_g = sum(f[i + 1] * (vals[i + 1] - mean_g)^2 for i in 0:3)
    n = length(s.t)
    rngk = max(1, floor(Int, n * (1 - tail)) + 1):n
    treated_frac = 0.0; cnt = 0
    for k in rngk
        tot = sum(@view s.Igt[k, :, :])
        tot == 0 && continue
        treated_frac += sum(@view s.Igt[k, :, 2]) / tot; cnt += 1
    end
    cnt > 0 && (treated_frac /= cnt)
    return (geno_freqs = f, freq_AB = f[4], mean_geno = mean_g,
            var_geno = var_g, treated_frac = treated_frac)
end

"Famille 3 — cause structurelle (réseau). Corrélation S-I normalisée par l'attendu sous mélange aléatoire."
function family3(s; tail = 0.3, mean_degree = nothing, N = nothing)
    s.si === nothing && return (si_edges = nothing, si_correlation = nothing)
    si = _tailmean(s.si, tail)
    corr = nothing
    if mean_degree !== nothing && N !== nothing
        Smean = _tailmean(s.S, tail); Imean = _tailmean(s.Itot, tail)
        # arêtes S-I attendues sous mélange aléatoire : (deg*N/2) * 2*(S/N)*(I/N)
        expected = mean_degree * Smean * Imean / N
        corr = expected > 0 ? si / expected : nothing
    end
    return (si_edges = si, si_correlation = corr)
end

"Résumé complet d'une série unique."
function summarize(s; kwargs...)
    return (label = s.label, f1 = family1(s; kwargs...),
            f2 = family2(s), f3 = family3(s; kwargs...))
end

"Résumé d'un ensemble de séries stochastiques : moyenne ± écart-type des indices clés + persistance."
function ensemble_summary(series_vec; tail = 0.3)
    prev = [family1(s; tail = tail).eq_prevalence for s in series_vec]
    fAB  = [family2(s; tail = tail).freq_AB for s in series_vec]
    persist = mean(s.Itot[end] > 0 for s in series_vec)
    return (label = series_vec[1].label,
            eq_prevalence = (mean(prev), std(prev)),
            freq_AB = (mean(fAB), std(fAB)),
            persistence = persist, n = length(series_vec))
end

end # module
