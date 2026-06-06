"""
    ResolutionA

**Résolution A — champ moyen déterministe (ODE).**

Limite « bien mélangée, grand N » du modèle DrugRes. C'est la baseline naïve :
on suit des *comptes* (S, et I[génotype, traité]), pas des individus. Les
événements deviennent des flux continus dérivés EXACTEMENT des mêmes taux
microscopiques que les versions stochastiques (B, C).

État `u` : `u[1] = S`, puis `I[g,t]` aplati. `E = N - S - ΣI` (implicite).
"""
module ResolutionA

using ..DrugResModel
using OrdinaryDiffEq

export run_ode, census_series_ode

# index dans u de I[g,t], g∈0:3, t∈0:1
iI(g, t) = 2 + t * 4 + g

function drugres_rhs!(du, u, p::Params, _t)
    vtc = p.vtc
    S = u[1]

    T = eltype(u)                          # compat. ForwardDiff (Dual lors du calcul de Jacobienne)
    Itot = zero(T)
    betaSum = zero(T)
    betaw = zeros(T, NGENO)                 # Σ_t beta[g,t] I[g,t]
    @inbounds for g in 0:3, t in 0:1
        I = u[iI(g, t)]
        Itot += I
        b = vtc[BETA, g + 1, t + 1] * I
        betaw[g + 1] += b
        betaSum += b
    end
    E = p.N - S - Itot
    birth = (E > 1e-9) ? p.theta : 0.0

    clear_to_S = 0.0
    @inbounds for g in 0:3, t in 0:1
        clear_to_S += vtc[CLEAR, g + 1, t + 1] * u[iI(g, t)]
    end

    # Susceptibles : naissance, mort, sortie par infection, retour par clairance
    du[1] = birth - p.mu * S - betaSum * S + clear_to_S

    @inbounds for g in 0:3, t in 0:1
        du[iI(g, t)] = 0.0
    end

    # Infection : la souche g entre, traitée avec proba tho
    @inbounds for g in 0:3
        inflow = betaw[g + 1] * S
        du[iI(g, 1)] += p.tho * inflow
        du[iI(g, 0)] += (1 - p.tho) * inflow
    end

    # Clairance (→ S) et mort (→ E)
    @inbounds for g in 0:3, t in 0:1
        I = u[iI(g, t)]
        du[iI(g, t)] -= vtc[CLEAR, g + 1, t + 1] * I
        du[iI(g, t)] -= (p.mu + vtc[VIR, g + 1, t + 1]) * I
    end

    # Mutation : flux génotypique (traitement préservé), total mut*Itot
    @inbounds for t in 0:1, g1 in 0:3
        I1 = u[iI(g1, t)]
        I1 <= 0 && continue
        md = mut_dist(g1)
        du[iI(g1, t)] -= p.mut * I1          # mutate change toujours de génotype
        for g2 in 0:3
            du[iI(g2, t)] += p.mut * I1 * md[g2 + 1]
        end
    end

    # Surinfection : résident uniforme, donneur ∝ beta. Total sigma*betaSum*Itot.
    # Taux (g1,t)->(geno,t) = sigma * I[g1,t] * Σ_{g2} betaw[g2] * recomb_dist(g1,g2)[geno]
    if Itot > 0
        @inbounds for t in 0:1, g1 in 0:3
            I1 = u[iI(g1, t)]
            I1 <= 0 && continue
            for g2 in 0:3
                bw = betaw[g2 + 1]
                bw <= 0 && continue
                rd = recomb_dist(g1, g2, p.rp)
                for geno in 0:3
                    geno == g1 && continue
                    rate = p.sigma * I1 * bw * rd[geno + 1]
                    du[iI(g1, t)]    -= rate
                    du[iI(geno, t)]  += rate
                end
            end
        end
    end
    return nothing
end

"""
    run_ode(p; S0, Iut0, It0, geno0=0, tend=3000.0, saveat=10.0)

Intègre le modèle champ moyen. Renvoie la solution OrdinaryDiffEq.
Le solveur `Rosenbrock23` est choisi pour sa robustesse (système potentiellement raide).
"""
function run_ode(p::Params; S0::Real, Iut0::Real, It0::Real,
                 geno0::Int = 0, tend = 3000.0, saveat = 10.0)
    u0 = zeros(Float64, 9)
    u0[1] = S0
    u0[iI(geno0, 0)] = Iut0
    u0[iI(geno0, 1)] = It0
    prob = ODEProblem(drugres_rhs!, u0, (0.0, tend), p)
    return solve(prob, Rosenbrock23(); saveat = saveat, abstol = 1e-8, reltol = 1e-6)
end

"Convertit une solution ODE en séries agrégées comparables aux versions stochastiques."
function census_series_ode(sol)
    ts = sol.t
    n = length(ts)
    S = zeros(Float64, n)
    Itot = zeros(Float64, n)
    Igt = zeros(Float64, n, NGENO, 2)
    for (k, u) in enumerate(sol.u)
        S[k] = u[1]
        for g in 0:3, t in 0:1
            Igt[k, g + 1, t + 1] = u[iI(g, t)]
        end
        Itot[k] = sum(@view Igt[k, :, :])
    end
    return (t = ts, S = S, Itot = Itot, Igt = Igt, si = nothing, label = "A (ODE champ moyen)")
end

end # module
