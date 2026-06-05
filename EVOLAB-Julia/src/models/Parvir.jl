"""
    Parvir — évolution de la virulence (d'après parvir-syl, EEE-MISC).

SIS sur une population de taille N constante (la « mort par infection » libère
l'hôte → redevient sain). Le pathogène a `ngeno` génotypes le long d'un **axe de
virulence** `vir[i]`, avec un **trade-off** transmission–virulence `β[i] = b0·vir[i]^coef / N`.
La **mutation** déplace le génotype de ±1 sur l'axe. La sélection (R0 = β/(μ+vir))
fait évoluer la virulence vers son optimum — et la **structure spatiale** déplace
cet optimum (auto-étouffement local → virulence évoluée plus faible). C'est le modèle
le plus parlant pour l'axe A/B/C côté évolution.

Classes : 1 = S (sain) ; 2..ngeno+1 = infecté de génotype 1..ngeno.
"""
module Parvir

using ..Runners: GState, set_class!, count_class, nodes_of, nbr_count, pick_neighbor, wchoose, tailmean
using Graphs: ne, nv
using Random: randperm
using Colors

fixed_params(L) = (N = L * L, ngeno = 21, virmin = 0.0, virmax = 6.0)
param_specs() = [(:b0, 1.0, 12.0, 6.0),
                 (:coef, 0.2, 1.5, 0.5),
                 (:mu, 0.2, 3.0, 1.0),
                 (:mut, 0.0005, 0.02, 0.005),
                 (:I0frac, 0.05, 0.8, 0.4)]

nclasses(p) = p.ngeno + 1
prevalence_classes(p) = Tuple(2:(p.ngeno + 1))
suggested_tend(p) = 150.0
state_names() = ["S"; ["g$i" for i in 1:21]]

vir(p, i) = p.virmin + (p.virmax - p.virmin) * (i - 1) / (p.ngeno - 1)
# transmission par contact, normalisée par le nb de contacts (N en champ moyen, degré en réseau)
betaf(p, i, norm) = p.b0 * vir(p, i)^p.coef / norm

# palette : S en gris clair ; génotypes en gradient virulence (or → rouge foncé)
state_palette(p) = vcat(colorant"gray85",
                        collect(range(colorant"gold", stop = colorant"darkred", length = p.ngeno)))

function init_classes(p, N, rng)
    cl = fill(1, N)                       # tous sains
    ninf = min(round(Int, p.I0frac * N), N)
    mid = cld(p.ngeno, 2)                 # on démarre au génotype médian
    for i in randperm(rng, N)[1:ninf]
        cl[i] = mid + 1
    end
    return cl
end

mutate_geno(i, ng, rng) = i == 1 ? 2 : i == ng ? ng - 1 : (rand(rng) < 0.5 ? i - 1 : i + 1)

function rand_infected(g, p)
    ng = p.ngeno
    n = sum(count_class(g, i + 1) for i in 1:ng); n == 0 && return 0
    k = rand(g.rng, 1:n)
    for i in 1:ng
        m = count_class(g, i + 1)
        if k <= m; return nodes_of(g, i + 1)[k]; else; k -= m; end
    end
    return 0
end

function rates(g::GState)
    p = g.params; ng = p.ngeno
    Sc = count_class(g, 1)
    norm = g.wellmixed ? float(p.N) : 2 * ne(g.graph) / nv(g.graph)
    deathw = 0.0; Itot = 0
    @inbounds for i in 1:ng
        ci = count_class(g, i + 1); ci == 0 && continue
        Itot += ci
        deathw += (p.mu + vir(p, i)) * ci
    end
    if g.wellmixed
        infw = 0.0
        @inbounds for i in 1:ng; infw += betaf(p, i, norm) * count_class(g, i + 1); end
        infection = infw * Sc
    else
        infection = 0.0
        @inbounds for i in 1:ng
            b = betaf(p, i, norm)
            for node in nodes_of(g, i + 1)
                infection += b * nbr_count(g, 1, node)
            end
        end
    end
    return (infection, deathw, p.mut * Itot)
end

function execute!(g::GState, k::Int)
    p = g.params; ng = p.ngeno
    norm = g.wellmixed ? float(p.N) : 2 * ne(g.graph) / nv(g.graph)
    if k == 1                              # infection : S → I(génotype du transmetteur)
        if g.wellmixed
            count_class(g, 1) == 0 && return
            w = [betaf(p, i, norm) * count_class(g, i + 1) for i in 1:ng]
            geno = wchoose(w, g.rng)
            target = nodes_of(g, 1)[rand(g.rng, 1:count_class(g, 1))]
        else
            total = 0.0
            @inbounds for i in 1:ng
                b = betaf(p, i, norm)
                for node in nodes_of(g, i + 1); total += b * nbr_count(g, 1, node); end
            end
            total <= 0 && return
            x = rand(g.rng) * total; acc = 0.0; source = 0
            @inbounds for i in 1:ng
                b = betaf(p, i, norm)
                for node in nodes_of(g, i + 1)
                    acc += b * nbr_count(g, 1, node)
                    if x <= acc; source = node; break; end
                end
                source != 0 && break
            end
            source == 0 && return
            geno = g.node_class[source] - 1
            target = pick_neighbor(g, source, 1)
            target == 0 && return
        end
        set_class!(g, target, geno + 1)
    elseif k == 2                          # mort par infection : I → S (hôte libéré)
        w = [(p.mu + vir(p, i)) * count_class(g, i + 1) for i in 1:ng]
        geno = wchoose(w, g.rng)
        count_class(g, geno + 1) == 0 && return
        id = nodes_of(g, geno + 1)[rand(g.rng, 1:count_class(g, geno + 1))]
        set_class!(g, id, 1)
    else                                   # mutation : génotype ±1
        id = rand_infected(g, p); id == 0 && return
        gi = g.node_class[id] - 1
        set_class!(g, id, mutate_geno(gi, ng, g.rng) + 1)
    end
end

# --- champ moyen : u = [S, g_1, ..., g_ngeno] ---
function init_u0(p)
    ng = p.ngeno; ninf = round(Int, p.I0frac * p.N); mid = cld(ng, 2)
    u = zeros(ng + 1); u[1] = float(p.N - ninf); u[mid + 1] = float(ninf)
    return u
end

function meanfield_rhs!(du, u, p, t)
    ng = p.ngeno; S = u[1]
    dS = 0.0
    @inbounds for i in 1:ng
        gi = u[i + 1]
        infl = betaf(p, i, p.N) * gi * S
        dth = (p.mu + vir(p, i)) * gi
        du[i + 1] = infl - dth - p.mut * gi      # infection - mort - mutation sortante
        dS += dth - infl
    end
    # mutation entrante (noyau ±1, réfléchissant aux bords)
    @inbounds for i in 1:ng
        infl = 0.0
        if i > 1
            j = i - 1; pj = (j == 1) ? 1.0 : 0.5
            infl += p.mut * u[j + 1] * pj
        end
        if i < ng
            j = i + 1; pj = (j == ng) ? 1.0 : 0.5
            infl += p.mut * u[j + 1] * pj
        end
        du[i + 1] += infl
    end
    du[1] = dS
    return nothing
end

u_prevalence(u, p) = sum(@view u[2:end])
outcome_ssa(res, p) = tailmean(res.prev, 0.3)
outcome_ode(res, p) = tailmean(res.prev, 0.3)

end # module
