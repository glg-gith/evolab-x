"""
    DrugResCore

Noyau de domaine du modèle **DrugRes** (S-I-S évolutif de Sylvain Gandon) : génotypes,
matrice VTC, recombinaison, mutation. Préoccupation distincte du câblage A/B/C — le
modèle `DrugRes` (src/models/DrugRes.jl) l'importe pour ne pas dupliquer cette logique.

Traduction fidèle de `evolab-x/Commented-Sims/Sources/C++/Drugres/DrugRes.cpp`.
(L'archive gelée `drugres-reference/` garde sa propre copie sous le nom `DrugResModel`.)

Événements : BIRTH, DEATH, INFECTION, CLEARANCE, SUPERINFECTION, MUTATION.
Le pathogène a 4 génotypes (2 loci) : 0=ab, 1=Ab, 2=aB, 3=AB. Chaque infecté porte
aussi un statut `treated` (0/1). Virulence / transmission / clairance dépendent du
génotype et du traitement via la matrice VTC. Génotypes gardés en **0-based** (0..3)
pour préserver à l'identique la logique de masques de bits de la recombinaison (cf. C++).
"""
module DrugResCore

export Params, default_params, build_vtc
export recombine, mutate, recomb_dist, mut_dist, weighted_choice
export EMPTY, SUSC, INF, VIR, BETA, CLEAR, NGENO

# --- États d'un site ---
const EMPTY = 0
const SUSC  = 1
const INF   = 2

# --- Index des traits dans la matrice VTC ---
const VIR   = 1   # virulence (alpha) : surmortalité des infectés
const BETA  = 2   # transmission
const CLEAR = 3   # clairance (gamma) : retour à susceptible
const NGENO = 4

"""
Paramètres du modèle. `vtc[trait, geno(1:4), treated(1:2)]` (génotype indexé +1).
"""
struct Params
    N::Int          # nombre de sites (capacité de charge)
    theta::Float64  # taux de naissance global (constant)
    mu::Float64     # mortalité naturelle par capita
    tho::Float64    # probabilité d'être traité à l'infection
    sigma::Float64  # susceptibilité à la surinfection
    rp::Float64     # probabilité de recombinaison
    mut::Float64    # taux de mutation
    vtc::Array{Float64,3}
end

"""
    build_vtc(dm)

Construit la matrice VTC cumulée à partir de `dm[trait, col, treated]` où la
colonne vaut 1=Default, 2=effet A, 3=effet B, 4=épistasie.
Cf. `set_state_matrix()` (DrugRes.cpp:490-527) :
geno0=Default, geno1=+A, geno2=+B, geno3=+A+B+épistasie.
"""
function build_vtc(dm::Array{Float64,3})
    vtc = zeros(Float64, 3, NGENO, 2)
    for tr in 1:3, t in 1:2
        d0, dA, dB, dE = dm[tr, 1, t], dm[tr, 2, t], dm[tr, 3, t], dm[tr, 4, t]
        vtc[tr, 1, t] = d0                 # ab
        vtc[tr, 2, t] = d0 + dA            # Ab
        vtc[tr, 3, t] = d0 + dB            # aB
        vtc[tr, 4, t] = d0 + dA + dB + dE  # AB
    end
    return vtc
end

"""
    default_params(; N=1000)

Paramètres et matrice par défaut (DefaultInputs(), DrugRes.cpp:535-583).
Par défaut, seule la **transmission** dépend du génotype et le traitement la
réduit — c'est le moteur de la dynamique de résistance.
"""
function default_params(; N::Int = 1000)
    dm = zeros(Float64, 3, 4, 2)
    #                Default        A effect       B effect      Épistasie
    dm[VIR,   :, :] = [0.005 0.005; 0.0    0.0;    0.0    0.0;   0.0 0.0]
    dm[BETA,  :, :] = [0.0007 0.0003; 0.0001 0.0002; 0.0001 0.0002; 0.0 0.0]
    dm[CLEAR, :, :] = [0.005 0.005; 0.0    0.0;    0.0    0.0;   0.0 0.0]
    return Params(N, 10.0, 0.01, 0.8, 1.0, 0.2, 0.01, build_vtc(dm))
end

g_to_mask(g::Int) = 1 << g

"""
    recombine(g1, g2, rp, rng) -> Int

Recombinaison de surinfection : génotype résultant quand un résident `g1` est
surinfecté par un donneur `g2`. Fidèle à `RealizeSuperinfection`
(DrugRes.cpp:377-445), quirks d'origine inclus (ex. `geno` reste 0 si p≥0.5
pour les paires à un locus de distance).
"""
function recombine(g1::Int, g2::Int, rp::Float64, rng)
    g1 == g2 && return g1
    c = g_to_mask(g1) | g_to_mask(g2)
    geno = 0
    p2 = rand(rng)
    if c == 3 || c == 5 || c == 10 || c == 12        # un locus de distance
        if p2 < 0.5
            geno = g2
        end  # sinon geno reste 0 (comportement C++ tel quel)
    elseif c == 9 || c == 6                          # deux loci de distance
        rp2 = (c == 6) ? rp : (1 - rp)
        if     p2 < rp2 / 2;                 geno = 0
        elseif p2 < rp2 / 2 + (1 - rp2) / 2; geno = 1
        elseif p2 < rp2 / 2 + (1 - rp2);     geno = 2
        else                                 geno = 3
        end
    end
    return geno
end

"Distribution de recombinaison (pour l'ODE champ moyen). Vecteur de proba sur geno 0:3."
function recomb_dist(g1::Int, g2::Int, rp::Float64)
    d = zeros(Float64, NGENO)
    if g1 == g2
        d[g1 + 1] = 1.0
        return d
    end
    c = g_to_mask(g1) | g_to_mask(g2)
    if c == 3 || c == 5 || c == 10 || c == 12
        d[g2 + 1] += 0.5
        d[1]      += 0.5          # geno 0
    elseif c == 9 || c == 6
        rp2 = (c == 6) ? rp : (1 - rp)
        d[1] = rp2 / 2; d[2] = (1 - rp2) / 2; d[3] = (1 - rp2) / 2; d[4] = rp2 / 2
    end
    return d
end

"""
    mutate(g1, rng) -> Int

Mutation d'un locus. Fidèle à `RealizeMutation` (DrugRes.cpp:447-481) :
extrêmes (0,3) → un des intermédiaires (1,2) ; intermédiaires → un des extrêmes.
"""
function mutate(g1::Int, rng)
    p1 = rand(rng)
    if g1 == 0 || g1 == 3
        return p1 < 0.5 ? 1 : 2
    else
        return p1 < 0.5 ? 0 : 3
    end
end

"Distribution de mutation (pour l'ODE)."
function mut_dist(g1::Int)
    d = zeros(Float64, NGENO)
    if g1 == 0 || g1 == 3
        d[2] = 0.5; d[3] = 0.5
    else
        d[1] = 0.5; d[4] = 0.5
    end
    return d
end

"Choix pondéré : index 1:length(w) tiré ∝ w (évite une dépendance externe)."
function weighted_choice(w::AbstractVector{<:Real}, rng)
    tot = sum(w)
    tot <= 0 && return rand(rng, 1:length(w))
    x = rand(rng) * tot
    acc = 0.0
    @inbounds for i in eachindex(w)
        acc += w[i]
        x <= acc && return i
    end
    return length(w)
end

end # module
