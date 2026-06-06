"""
    DrugRes — modèle DrugRes (S-I-S évolutif de Gandon) pour le moteur générique.

Réutilise le noyau de domaine `DrugResCore` (matrice VTC, recombinaison, mutation).
Génotypes : 4 (2 loci) × {traité, non traité}. Classes : 1=EMPTY, 2=SUSC,
3..10 = INF[génotype, traité].

⚠ Le plus lourd des modèles (10 classes, génotypes) → plutôt sur grille modeste.
Cross-check : en bien mélangé sur graphe complet, doit retrouver les ~640 de
l'instrument validé gelé (`drugres-reference/`, `ResolutionBC.run_B`).
"""
module DrugRes

using ..Runners: GState, set_class!, count_class, nodes_of, nbr_count, pick_neighbor, wchoose, tailmean
using ..DrugResCore: VIR, BETA, CLEAR, NGENO, recombine, mutate, recomb_dist, mut_dist, default_params
using Graphs: neighbors
using Random: randperm

# classes
@inline catid(treated, geno) = 3 + treated * 4 + geno      # INF[geno 0:3, treated 0:1] -> 3..10
@inline geno_of(c) = (c - 3) % 4
@inline treated_of(c) = (c - 3) ÷ 4
const EMPTY_C = 1
const SUSC_C = 2

nclasses(p) = 10
prevalence_classes(p) = (3, 4, 5, 6, 7, 8, 9, 10)
suggested_tend(p) = 1200.0
state_names() = ["EMPTY", "S", "I g0", "I g1", "I g2", "I g3", "T g0", "T g1", "T g2", "T g3"]
state_palette(p) = ["gray95", "steelblue", "#d62728", "#ff7f0e", "#bcbd22", "#9467bd",
                    "#fb9a99", "#fdbf6f", "#dbdb8d", "#cab2d6"]

fixed_params(L) = (N = L * L, vtc = default_params().vtc)
param_specs() = [(:theta, 1.0, 30.0, 10.0),
                 (:mu, 0.002, 0.05, 0.01),
                 (:tho, 0.0, 1.0, 0.8),
                 (:sigma, 0.0, 2.0, 1.0),
                 (:rp, 0.0, 1.0, 0.2),
                 (:mut, 0.0, 0.05, 0.01)]

@inline icount(g, geno, treated) = count_class(g, catid(treated, geno))
betaq(p, geno, treated) = p.vtc[BETA, geno + 1, treated + 1]
virq(p, geno, treated) = p.vtc[VIR, geno + 1, treated + 1]
clearq(p, geno, treated) = p.vtc[CLEAR, geno + 1, treated + 1]

# poids beta par génotype (sommé sur traité/non), et somme totale
function beta_weights(g)
    p = g.params; bw = zeros(4); bs = 0.0
    @inbounds for geno in 0:3, t in 0:1
        b = betaq(p, geno, t) * icount(g, geno, t)
        bw[geno + 1] += b; bs += b
    end
    return bw, bs
end

random_infected(g) = (n = sum(count_class(g, c) for c in 3:10); n == 0 ? 0 :
    (k = rand(g.rng, 1:n);
     for c in 3:10; m = count_class(g, c); k <= m ? (return nodes_of(g, c)[k]) : (k -= m); end; 0))

function infected_neighbor(g, node)   # voisin infecté au hasard (réservoir)
    chosen = 0; cnt = 0
    @inbounds for w in neighbors(g.graph, node)
        if g.node_class[w] >= 3
            cnt += 1; rand(g.rng) * cnt <= 1.0 && (chosen = w)
        end
    end
    return chosen
end

function rates(g::GState)
    p = g.params
    S = count_class(g, SUSC_C); E = count_class(g, EMPTY_C)
    bw, bs = beta_weights(g)
    Itot = 0; death = p.mu * S; clearance = 0.0
    @inbounds for geno in 0:3, t in 0:1
        c = icount(g, geno, t); c == 0 && continue
        Itot += c
        death += (p.mu + virq(p, geno, t)) * c
        clearance += clearq(p, geno, t) * c
    end
    birth = E > 0 ? p.theta : 0.0
    if g.wellmixed
        infection = bs * S
        superinf = p.sigma * bs * Itot
    else
        infection = 0.0; superinf = 0.0
        @inbounds for geno in 0:3, t in 0:1
            b = betaq(p, geno, t)
            for node in nodes_of(g, catid(t, geno))
                infection += b * nbr_count(g, SUSC_C, node)
                inb = 0; for c in 3:10; inb += nbr_count(g, c, node); end
                superinf += b * inb
            end
        end
        superinf *= p.sigma
    end
    return (birth, death, infection, clearance, superinf, p.mut * Itot)
end

function execute!(g::GState, k::Int)
    p = g.params
    if k == 1                                   # naissance : EMPTY -> S
        count_class(g, EMPTY_C) == 0 && return
        set_class!(g, nodes_of(g, EMPTY_C)[rand(g.rng, 1:count_class(g, EMPTY_C))], SUSC_C)

    elseif k == 2                               # mort : -> EMPTY (∝ taux de mort par classe)
        w = zeros(9); cats = Vector{Int}(undef, 9)
        w[1] = p.mu * count_class(g, SUSC_C); cats[1] = SUSC_C
        i = 1
        @inbounds for geno in 0:3, t in 0:1
            i += 1; c = catid(t, geno)
            w[i] = (p.mu + virq(p, geno, t)) * count_class(g, c); cats[i] = c
        end
        c = cats[wchoose(w, g.rng)]; count_class(g, c) == 0 && return
        set_class!(g, nodes_of(g, c)[rand(g.rng, 1:count_class(g, c))], EMPTY_C)

    elseif k == 3                               # infection : S -> I
        if g.wellmixed
            bw, _ = beta_weights(g); geno = wchoose(bw, g.rng) - 1
            count_class(g, SUSC_C) == 0 && return
            target = nodes_of(g, SUSC_C)[rand(g.rng, 1:count_class(g, SUSC_C))]
        else
            total = 0.0
            @inbounds for geno in 0:3, t in 0:1
                b = betaq(p, geno, t)
                for node in nodes_of(g, catid(t, geno)); total += b * nbr_count(g, SUSC_C, node); end
            end
            total <= 0 && return
            x = rand(g.rng) * total; acc = 0.0; source = 0
            @inbounds for geno in 0:3, t in 0:1
                b = betaq(p, geno, t)
                for node in nodes_of(g, catid(t, geno))
                    acc += b * nbr_count(g, SUSC_C, node)
                    if x <= acc; source = node; break; end
                end
                source != 0 && break
            end
            source == 0 && return
            geno = geno_of(g.node_class[source])
            target = pick_neighbor(g, source, SUSC_C); target == 0 && return
        end
        treated = rand(g.rng) < p.tho ? 1 : 0
        set_class!(g, target, catid(treated, geno))

    elseif k == 4                               # clairance : I -> S (∝ taux de clairance)
        w = zeros(8); cats = Vector{Int}(undef, 8); i = 0
        @inbounds for geno in 0:3, t in 0:1
            i += 1; c = catid(t, geno)
            w[i] = clearq(p, geno, t) * count_class(g, c); cats[i] = c
        end
        c = cats[wchoose(w, g.rng)]; count_class(g, c) == 0 && return
        set_class!(g, nodes_of(g, c)[rand(g.rng, 1:count_class(g, c))], SUSC_C)

    elseif k == 5                               # surinfection : résident change de génotype
        if g.wellmixed
            resident = random_infected(g); resident == 0 && return
            bw, _ = beta_weights(g); g2 = wchoose(bw, g.rng) - 1
        else
            total = 0.0
            @inbounds for geno in 0:3, t in 0:1
                b = betaq(p, geno, t)
                for node in nodes_of(g, catid(t, geno))
                    inb = 0; for c in 3:10; inb += nbr_count(g, c, node); end
                    total += b * inb
                end
            end
            total <= 0 && return
            x = rand(g.rng) * total; acc = 0.0; donor = 0
            @inbounds for geno in 0:3, t in 0:1
                b = betaq(p, geno, t)
                for node in nodes_of(g, catid(t, geno))
                    inb = 0; for c in 3:10; inb += nbr_count(g, c, node); end
                    acc += b * inb
                    if x <= acc; donor = node; break; end
                end
                donor != 0 && break
            end
            donor == 0 && return
            resident = infected_neighbor(g, donor); resident == 0 && return
            g2 = geno_of(g.node_class[donor])
        end
        rc = g.node_class[resident]; g1 = geno_of(rc); tr = treated_of(rc)
        geno = recombine(g1, g2, p.rp, g.rng)
        geno != g1 && set_class!(g, resident, catid(tr, geno))

    else                                        # mutation
        resident = random_infected(g); resident == 0 && return
        rc = g.node_class[resident]; g1 = geno_of(rc); tr = treated_of(rc)
        g2 = mutate(g1, g.rng)
        g2 != g1 && set_class!(g, resident, catid(tr, g2))
    end
end

# --- champ moyen (repris de ResolutionA) : u = [S, I[g,t]] (9 dim) ---
iI(geno, t) = 2 + t * 4 + geno
function init_u0(p)
    N = p.N; S0 = round(Int, 0.3 * N); I0 = round(Int, 0.1 * N)
    It0 = round(Int, I0 * p.tho); Iut0 = I0 - It0
    u = zeros(9); u[1] = S0; u[iI(0, 0)] = Iut0; u[iI(0, 1)] = It0
    return u
end

function meanfield_rhs!(du, u, p, t)
    vtc = p.vtc; S = u[1]; T = eltype(u)
    Itot = zero(T); betaSum = zero(T); betaw = zeros(T, 4)
    @inbounds for geno in 0:3, tt in 0:1
        I = u[iI(geno, tt)]; Itot += I
        b = vtc[BETA, geno + 1, tt + 1] * I; betaw[geno + 1] += b; betaSum += b
    end
    E = p.N - S - Itot
    birth = E > 1e-9 ? p.theta : 0.0
    clear_to_S = zero(T)
    @inbounds for geno in 0:3, tt in 0:1
        clear_to_S += vtc[CLEAR, geno + 1, tt + 1] * u[iI(geno, tt)]
    end
    du[1] = birth - p.mu * S - betaSum * S + clear_to_S
    @inbounds for geno in 0:3, tt in 0:1; du[iI(geno, tt)] = zero(T); end
    @inbounds for geno in 0:3
        inflow = betaw[geno + 1] * S
        du[iI(geno, 1)] += p.tho * inflow
        du[iI(geno, 0)] += (1 - p.tho) * inflow
    end
    @inbounds for geno in 0:3, tt in 0:1
        I = u[iI(geno, tt)]
        du[iI(geno, tt)] -= vtc[CLEAR, geno + 1, tt + 1] * I
        du[iI(geno, tt)] -= (p.mu + vtc[VIR, geno + 1, tt + 1]) * I
    end
    @inbounds for tt in 0:1, g1 in 0:3        # mutation
        I1 = u[iI(g1, tt)]; I1 <= 0 && continue
        md = mut_dist(g1); du[iI(g1, tt)] -= p.mut * I1
        for g2 in 0:3; du[iI(g2, tt)] += p.mut * I1 * md[g2 + 1]; end
    end
    if Itot > 0                                # surinfection
        @inbounds for tt in 0:1, g1 in 0:3
            I1 = u[iI(g1, tt)]; I1 <= 0 && continue
            for g2 in 0:3
                bw = betaw[g2 + 1]; bw <= 0 && continue
                rd = recomb_dist(g1, g2, p.rp)
                for geno in 0:3
                    geno == g1 && continue
                    rate = p.sigma * I1 * bw * rd[geno + 1]
                    du[iI(g1, tt)] -= rate; du[iI(geno, tt)] += rate
                end
            end
        end
    end
    return nothing
end

u_prevalence(u, p) = sum(@view u[2:9])

function init_classes(p, N, rng)
    cl = fill(EMPTY_C, N)
    S0 = round(Int, 0.3 * N); I0 = round(Int, 0.1 * N)
    It0 = round(Int, I0 * p.tho); Iut0 = I0 - It0
    idx = randperm(rng, N); k = 1
    for _ in 1:S0;   cl[idx[k]] = SUSC_C;          k += 1; end
    for _ in 1:Iut0; cl[idx[k]] = catid(0, 0);     k += 1; end
    for _ in 1:It0;  cl[idx[k]] = catid(1, 0);     k += 1; end
    return cl
end

outcome_ssa(res, p) = tailmean(res.prev, 0.3)
outcome_ode(res, p) = tailmean(res.prev, 0.3)

end # module
