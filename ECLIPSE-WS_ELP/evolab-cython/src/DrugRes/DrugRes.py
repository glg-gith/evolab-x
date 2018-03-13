import sys

from xml.dom.minidom import parse

import cyelp

from cyelp import PyLabSimulatorTimeArray

from cyelp import PyLabRng
from cyelp import PyLabTimer
from cyelp import PyLabOutputerLine


from cyelp import Py_ofstream
from cyelp import Py_ifstream
from cyelp import PyLabBinaryUtils

from cyelp import PyLabEventStack
from cyelp import PyLabCallBack

from cyelp import PyDoubleArg
from cyelp import PyIntArg
from cyelp import PyStringArg
from cyelp import PySwitchArg
from cyelp import PyCmdLine

from cyelp import PyLabSiteSetsCalculator


from cyelp import PyLabSiteBase


import numpy as np

'''
# GLOBALS
'''

# BASICS

run_id      = "0"
nb_rep      = 1
nb_sites    = 1000 #100000;//100;//

tt_out      = 1000 #/ 30                   # Output by steps of one 1 time unit
tt_end      = 100000 #/ 30                 # When simulation ends

popsize = None                            # Total pop size

output_files_prefix = None



'''
# INPUTS
'''

# Individuals
# States amounts (int)
global E, S, I  
# Individuals models (PyLabSiteBase)
global E_site, S_site, I_UT_site, I_T_site #, S_UT_site, S_T_site

# General rates (double)
global theta        # Global birth rate
global mu           # Natural per capita death rate
global tho          # Probability to be treated
global sigma        # Superinfection susceptibility
global rp           # Recombination probability
global mut          # Mutation rate

# Loci-specific rates
# State matrix
global VTCmtx_input, VTCmtx



'''
MISC
'''

# Mean transmission, virulence and clearance
global virMOY, betaMOY, clearMOY
global virMOY_I, virMOY_T, betaMOY_I, betaMOY_T, clearMOY_I, clearMOY_T

# Pathogen genotype can be ab=0=>0001, Ab=1=>0010, aB=2=>0100, AB=3=>1000
ngeno = 4;

global killedI, killedT, clearedI, clearedT
killedI = killedT = clearedI = clearedT = 0

global nb_fired
nb_fired = 0


## STATES
#typedef enum { EMPTY, SUSCEPTIBLE, INFECTED } t_state;
global EMPTY, SUSCEPTIBLE, INFECTED
EMPTY       = 0
SUSCEPTIBLE = 1
INFECTED    = 2

## ATTRIBUTES
#typedef enum { TREATED, GENO } t_site_attr;
global TREATED, GENO 
TREATED = 0
GENO    = 1

## GENERAL PROPERTIES (referenced in the VTCmtx)
#typedef enum { alpha = 0, beta = 1, gamma = 2 } t_attr_id;        // virulence, transmission, clearance
global alpha, beta, gamma
alpha = 0
beta  = 1
gamma = 2


# Random Number Generator
global rng
# Timer
global timer
# Sets Theory Calculator
global _C



'''
RATES CALLBACKS
'''

#@callback
def BirthRate(param):
    return theta

#@callback
def DeathRate(param):
    global virMOY_I, virMOY_T
    global _C
    
    # Susceptible             \           
    # Infected / not treated  \ 
    # Infected / treated       
    return (mu * _C.GS("S").Cardinality())                \
         + (mu + virMOY_I) * (_C.GS("T0").Cardinality())  \
         + (mu + virMOY_T) * (_C.GS("T1").Cardinality())   

#@callback
def InfectionRate(param):
    global betaMOY
    global _C
    return betaMOY * (_C.GS("I").Cardinality()) * _C.GS("S").Cardinality()

#@callback
def ClearanceRate(param):
    global clearMOY_I, clearMOY_T
    global _C
    return clearMOY_I * (_C.GS("T0").Cardinality())  +  clearMOY_T * (_C.GS("T1").Cardinality())

#@callback
def SuperinfectionRate(param):
    global betaMOY
    global _C
    return sigma * _C.GS("I").Cardinality() * ( betaMOY * (_C.GS("I").Cardinality()) )

#@callback
def MutationRate(param):
    global _C
    return mut * _C.GS("I").Cardinality()


'''
REALIZES CALLBACKS
'''

#@callback
def RealizeBirth(param):
#    print "BIRTH fired !"
    
    global _C
    global popsize
    
    global EMPTY, SUSCEPTIBLE, INFECTED
    global TREATED, GENO 

    site = _C.GI(_C.GS("EMPTY").GetFirst())
    if (site != None):
#        site.Copy(S_site, cyelp.IND_DEP);
        site.SetState(SUSCEPTIBLE, 0.0);
        site.SetAttrDouble(cyelp.IND_DEP, TREATED, -1);
        site.SetAttrDouble(cyelp.IND_DEP, GENO, -1);

        popsize += 1

    return 0.0


#@callback
def RealizeDeath(param):
#    print "DEATH fired !"
    
    global _C
    global rng
    global popsize
    global virMOY_I, virMOY_T
    
    global EMPTY, SUSCEPTIBLE, INFECTED
    global TREATED, GENO 
    global alpha, beta, gamma
    
    global VTCmtx
    
    global killedI, killedT

    site = None
    p1 = rng.GetNextDouble()
    fgeno = 0.0;
    geno = -1;

    natural_death = (mu * _C.GS("S").Cardinality())
    max = mu + ((mu + virMOY_I) * _C.GS("T0").Cardinality()) + ((mu + virMOY_T) * _C.GS("T1").Cardinality())

    fgeno += natural_death / max;

    # Try to kill a S
    if (p1 < fgeno):
        site = _C.GI(_C.GS("S").ChooseOne(rng))
    else:

        # Try to kill a I_UT
        for i in xrange(ngeno):
            # Build geno group name
            s = "g" + str(i)

            #fgeno += (vir[i]*gI[i]) / (virMOY_I*(y[1]+y[2]));
            fgeno += ((mu + VTCmtx[alpha][i][0]) * _C.GS("T0").CI(_C.GS(s))) / max
            if (p1 < (fgeno)):
                geno = i
                site = _C.GI((_C.GS("T0")).GetFirst())
                if (site != None): killedI += 1
                break 

        # Try to kill a I_T
        if (geno == -1):             # No match for untreated ones
            for i in xrange(ngeno):
                # Build geno group name
                s = "g" + str(i)

                #fgeno += (virT[i]*gT[i]) / (virMOY_T*(y[1]+y[2]));
                fgeno += ((mu + VTCmtx[alpha][i][1]) * _C.GS("T1").CI(_C.GS(s))) / max
                if (p1 < (fgeno)):
                    geno = i
                    site = _C.GI((_C.GS("T1")).GetFirst())
                    if (site != None): killedT += 1
                    break

    # Return to emptiness
    if (site != None):
        #site.Copy(E_site);
        site.SetState(EMPTY, 0.0)
        site.SetAttrDouble(cyelp.IND_DEP, TREATED, -1)
        site.SetAttrDouble(cyelp.IND_DEP, GENO, -1)
        popsize -= 1


    return 0.0
    

#@callback
def RealizeInfection(param):
#    print "INFECTION fired !"

    global _C
    global rng
    global betaMOY
    
    global EMPTY, SUSCEPTIBLE, INFECTED
    global TREATED, GENO 
    global alpha, beta, gamma

    global tho
    global VTCmtx

    p1 = rng.GetNextDouble()
    fgeno = 0.0
    geno = -1

    max = (betaMOY * _C.GS("I").Cardinality())

    # Choose infecting strain
    for i in xrange(ngeno):
        s = "g" + str(i)
        fgeno += (VTCmtx[beta][i][0] * _C.GS("T0").CI(_C.GS(s)) + \
                  VTCmtx[beta][i][1] * _C.GS("T1").CI(_C.GS(s)) ) / max
        if (p1 < (fgeno)):
            geno = i
            break

    # Goto infectious state
    site = _C.GI(_C.GS("S").ChooseOne(rng))
    if (site != None):
        site.SetState(INFECTED, 0.0)

#        if (RND(site.GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED)) == 0) {    # Not treated
#            site.Copy(I_UT_site)
#        } else {
#            site.Copy(I_T_site)
#        }

#        if (RND(site.GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED)) == -1)        # ????
#        {
            # Choose to treat or not
        p1 = rng.GetNextDouble()
        if (p1 < tho):
            #site.Copy(I_T_site)        # Treated
            site.SetAttrDouble(cyelp.IND_DEP, TREATED, 1)
        else:
            #site.Copy(I_UT_site)        # Not treated
            site.SetAttrDouble(cyelp.IND_DEP, TREATED, 0)
#        } else {
#            if (RND(site.GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED)) == 1) # ????
#                site.Copy(I_T_site)        # Treated
#            else
#                site.Copy(I_UT_site)        # Not treated
#        }

        site.SetAttrDouble(cyelp.IND_DEP, GENO, geno)


    return 0.0


#@callback
def RealizeClearance(param):
#    print "CLEARANCE fired !"

    global _C
    global rng
    
    global clearMOY_I, clearMOY_T
    global clearedI, clearedT
    
    global EMPTY, SUSCEPTIBLE, INFECTED
    global TREATED, GENO 
    
    global VTCmtx


    site = None

    p1 = rng.GetNextDouble()
    fgeno = 0.0
    geno = -1
    
    max = (clearMOY_I * _C.GS("T0").Cardinality()) + (clearMOY_T * _C.GS("T1").Cardinality())
    

    # Try to clear a I_UT
    for i in xrange(ngeno):
        # Build geno group name
        s = "g" + str(i)

        #fgeno += (clear[i]*gI[i]) / (clearMOY_I*(y[1]))
        fgeno += (VTCmtx[gamma][i][0] * _C.GS("T0").CI(_C.GS(s))) / max
        if (p1 < (fgeno)):
            geno = i
            site = _C.GI( (_C.GS("T0") * _C.GS(s)).GetFirst() )
            if (site != None): clearedI += 1
            break

    # Try to clear a I_T
    if (geno == -1):             # No match for untreated ones
        for i in xrange(ngeno):
            # Build geno group name
            s = "g" + str(i)

            #fgeno += (clearT[i]*gT[i]) / (clearMOY_T*(y[2]))
            fgeno += (VTCmtx[gamma][i][1] * _C.GS("T1").CI(_C.GS(s))) / max
            if (p1 < (fgeno)):
                geno = i
                site = _C.GI( (_C.GS("T1") * _C.GS(s)).GetFirst() )
                if (site != None): clearedT += 1
                break

    # Return to susceptible state
    if (site != None):
        #site.Copy(S_site)
        site.SetState(SUSCEPTIBLE, 0.0)
        site.SetAttrDouble(cyelp.IND_DEP, TREATED, -1)
        site.SetAttrDouble(cyelp.IND_DEP, GENO, -1)
    
    return 0.0



# Full mask = 1111 = 2^3+2^2+2^1+2^0 = 8+4+2+1 = 15
# Ex : g=1 => 0010 = 2^1 = 2
def g_to_mask(g):
    return 1 << g

#@callback
def RealizeSuperinfection(param):
#    print "SUPERINFECTION fired !"

    global _C
    global rng

    global alpha, beta, gamma
    
    global VTCmtx


    global betaMOY

    site = None

    p1 = rng.GetNextDouble()
    fgeno = 0.0

    g1 = g2 = 0
    # Choose the resident strain
    site = _C.GI(_C.GS("I").ChooseOne(rng))
    if (site != None):                                     # TODO : else this is EXTINCTION and should be checked in the main loop
        ##g1 = RND(site.GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::GENO))
        g1 = site.GetAttrInt(cyelp.IND_DEP, GENO)

        # Choose the superinfecting strain (superinfection is beta-dependent)
        # TODO : ici il faudrait faire -1 sur le group (gI ou gT) quand c'est le group de l'individu choisi ci-dessus
        #        car un individu ne peut pas se superinfecter lui-meme...
        for i in xrange(ngeno):
            # Build geno group name
            s = "g" + str(i)
            fgeno += ( VTCmtx[beta][i][0] * _C.GS("T0").CI(_C.GS(s)) +      \
                       VTCmtx[beta][i][1] * _C.GS("T1").CI(_C.GS(s)) ) / (betaMOY * _C.GS("I").Cardinality())
            if (p1 < (fgeno)):
                g2 = i
                break


        if (g1 != g2):
            # Combine g1 and g2
            # Ex : g1=1=0010 and g2=3=1000 => c=1010=2^3+2^1=10
                        
            c = g_to_mask(g1) | g_to_mask(g2)
            geno = g1

            # Choose the winner strain
            p2 = rng.GetNextDouble()
            rp2 = 0.0
            if ((c == 3) or (c == 5) or (c == 10) or (c == 12)):                # c=0011=3 or c=0101=5 or c=1010=10 or c=1100=12
                if (p2 < 0.5): geno = g2
            else:
                if (c == 9 or c == 6):                                          # c=1001=9 or c=0110=6
                    if (c == 6): rp2 = rp 
                    else: rp2 = (1-rp)                    # Switch rp for the extremes (c=1001)
                    
                    if (p2 < rp2/2.0):                         geno = 0
                    elif (p2 < rp2/2.0 + (1-rp2)/2.0):         geno = 1
                    elif (p2 < rp2/2.0 + (1-rp2)):             geno = 2
                    else:                                      geno = 3

            # Ancient and new strains are different : do replacement
            if ((g1 != geno) and (site != None)):
                site.SetAttrDouble(cyelp.IND_DEP, GENO, geno)


    return 0.0


#@callback
def RealizeMutation(param):
#    print "MUTATION fired !"

    global _C
    global rng
    site = None

    p1 = rng.GetNextDouble()

    g1 = g2 = 0
    # Choose the resident strain
    site = _C.GI(_C.GS("I").ChooseOne(rng))
    if (site != None):                                     # TODO : else this is EXTINCTION and should be checked in the main loop

        g1 = site.GetAttrInt(cyelp.IND_DEP, GENO)

        if ((g1 == 0) or (g1 == 3)):      # Extremes : choose one among the middles
            if (p1 < 0.5): g2 = 1
            else: g2 = 2
        elif ((g1 == 1) or (g1 == 2)):    # Middles : choose one among the extremes
            if (p1 < 0.5): g2 = 0
            else: g2 = 3

        # Ancient and new strains are different : do replacement
        if (g1 != g2):
            site.SetAttrDouble(cyelp.IND_DEP, GENO, g2)

    return 0.0


'''
Misc
'''


def set_state_matrix(mtx):
    
    global VTCmtx
    
    # VTCmtx gets the same structure as 'mtx' (shape=(3, 4, 2), dtype=np.float)
    VTCmtx = np.empty_like(mtx) 

    # Virulence                [ex : treatment lowers virulence]
    # Not-Treated / vir[Iab, IAb, IaB, IAB]
    VTCmtx[0][0][0] = mtx[0][0][0]
    VTCmtx[0][1][0] = mtx[0][0][0] + mtx[0][1][0]
    VTCmtx[0][2][0] = mtx[0][0][0] + mtx[0][2][0]
    VTCmtx[0][3][0] = mtx[0][0][0] + mtx[0][1][0] + mtx[0][2][0] + mtx[0][3][0]
    # Treated / virT[Tab, TAb, TaB, TAB]
    VTCmtx[0][0][1] = mtx[0][0][1]
    VTCmtx[0][1][1] = mtx[0][0][1] + mtx[0][1][1]
    VTCmtx[0][2][1] = mtx[0][0][1] + mtx[0][2][1]
    VTCmtx[0][3][1] = mtx[0][0][1] + mtx[0][1][1] + mtx[0][2][1] + mtx[0][3][1]

    # Transmission                [ex : treatment lowers transmission]
    # Not-Treated / beta[Iab, IAb, IaB, IAB]
    VTCmtx[1][0][0] = mtx[1][0][0]
    VTCmtx[1][1][0] = mtx[1][0][0] + mtx[1][1][0]
    VTCmtx[1][2][0] = mtx[1][0][0] + mtx[1][2][0]
    VTCmtx[1][3][0] = mtx[1][0][0] + mtx[1][1][0] + mtx[1][2][0] + mtx[1][3][0]
    # Treated / betaT[Tab, TAb, TaB, TAB]
    VTCmtx[1][0][1] = mtx[1][0][1]
    VTCmtx[1][1][1] = mtx[1][0][1] + mtx[1][1][1]
    VTCmtx[1][2][1] = mtx[1][0][1] + mtx[1][2][1]
    VTCmtx[1][3][1] = mtx[1][0][1] + mtx[1][1][1] + mtx[1][2][1] + mtx[1][3][1]

    # Clearance                [ex : treatment uppers clearance]
    # Not-Treated / clear[Iab, IAb, IaB, IAB]
    VTCmtx[2][0][0] = mtx[2][0][0]
    VTCmtx[2][1][0] = mtx[2][0][0] + mtx[2][1][0]
    VTCmtx[2][2][0] = mtx[2][0][0] + mtx[2][2][0]
    VTCmtx[2][3][0] = mtx[2][0][0] + mtx[2][1][0] + mtx[2][2][0] + mtx[2][3][0]
    # Treated / clearT[Tab, TAb, TaB, TAB]
    VTCmtx[2][0][1] = mtx[2][0][1]
    VTCmtx[2][1][1] = mtx[2][0][1] + mtx[2][1][1]
    VTCmtx[2][2][1] = mtx[2][0][1] + mtx[2][2][1]
    VTCmtx[2][3][1] = mtx[2][0][1] + mtx[2][1][1] + mtx[2][2][1] + mtx[2][3][1]



'''
# SIM CLASS : The main simulation definition class to be run

The class "PyLabSimulatorTimeArray" inherits from "PyLabSimulatorTime" + handles an object of type 
"PyLabArrayBinding" for dealing with the a list of sites (See : PyLabSimulatorTimeArray.arrayBinding).
'''
class DrugRes(PyLabSimulatorTimeArray):
    
    # Dump file
    outputer = None
    outputFilename = None
    
    eventStack = None
    
    # Binary streams
    ofs = None
    ifs = None
    
    def __init__(self):
        global output_files_prefix
        output_files_prefix = "output"
        
        self.eventStack = PyLabEventStack()

    
    def DefaultInputs(self):
        
        global theta, mu, tho, sigma, rp, mut
        
        global E, S, I
        
        global VTCmtx_input
        
        # Individuals
        S = 270            # TODO : Replace it by endemic equilibrium law
        I = 100            # 80
        E = nb_sites - (S + I)
    
        ##T = 20                # 20
    
        # General rates
        theta = 10.0
        mu = .01#.01
        tho = 0.8#0.8
        sigma = 1
        rp  = 0.2
        mut = 0.01#0.001
    
        # Loci-specific rates
        # Default state matrix
        #
        #                               Default     A effect     B effect     Epistasis
        #                               ___________________________________________________
        #     Virulence         [I,T]    | [a, aT]     [daA, daAT]  [daB, daBT]  [dae, daeT]
        #     Transmission    [I,T]    | [b, bT]     [dbA, dbAT]  [dbB, dbBT]  [dbe, dbeT]
        #     Clearance        [I,T]    | [c, cT]     [dcA, dcAT]  [dcB, dcBT]  [dce, dceT]
        #
        #
        #-b [[.00007,.00003],[-.000001,.000001],[-.000001,.000001],[0.0,0.0]]
        
        # Numpy way of doing
        VTCmtx_input = np.array([
                [ [0.005,     0.005],         [0.00,         0.00],         [0.00,         0.00],         [0.0,    0.0] ],
                ##[ [.0007,     .0003],         [.0001,     .0002],     [.0001,     .0002],     [0.0,    0.0] ],
                [ [.00007,     .00003],         [-.00001,     .00002],     [-.00001,     .00002],     [0.0,    0.0] ],
                #[ [.000003,     .000002],         [-.0000001,     .0000001],     [-.0000001,     .0000001],     [0.0,    0.0] ],
                #/[ [mu + .01,     mu + .01],     [0,         0.00],         [0,         0.00],         [0.0,    0.0] ]
                [ [0.005,     0.005],     [0.0,         0.00],         [0.0,         0.00],         [0.0,    0.0] ]
        ], dtype=np.float)
        
        #    VTCmtx_input = [
        #            [ [0.005,     0.005],         [0.00,         0.00],         [0.00,         0.00],         [0.0,    0.0] ],
        #            [ [.0007,     .0003],         [-.0001,     .0002],     [-.0001,     .0002],     [0.001,    0.0] ],
        #            #[ [.000003,     .000002],         [-.0000001,     .0000001],     [-.0000001,     .0000001],     [0.0,    0.0] ],
        #            #/[ [mu + .01,     mu + .01],     [0,         0.00],         [0,         0.00],         [0.0,    0.0] ]
        #            [ [0.005,     0.005],     [0,         0.00],         [0,         0.00],         [0.0,    0.0] ]
        #    ]

        ###/
        set_state_matrix(VTCmtx_input)
        ###/
    
        self.SetNbRepetitions(nb_rep)
    
        self.SetTimeDumpStep(1)
        self.SetDoDump(False)
        self.SetDoStartFromDump(False)



    
#    def ParseCL(self, argv):
#        global d, b, mu, mo, x1, x2, L
#        global nb_rep, run_id
#        global output_files_prefix, xml_input_filename
#
#    
#        # If XML input was set : skip CL parsing
#        argc = len(argv)
#        for i in xrange(argc):
#            if (argc-1 > i and argv[i] == "-i"):
#                return argv[i+1]
#    
#        # Wrap everything in a try block.  Do this every time,
#        # because exceptions will be thrown for problems.
#        try:
#            
#            cmd = PyCmdLine("BdmLsim4-python simulation (using EVOLAB-X1 project)", ord(' '), "0.4", True)
#            
#            dr_arg = PyDoubleArg("d", "death-rate", "Constant individual death factor.", True, 0.01, "double")
#            cmd.add( dr_arg )
#            
#            br_arg = PyDoubleArg("b", "birth-rate", "Constant individual birth factor.", True, 0.01, "double")
#            cmd.add( br_arg )
#            
#            mu_arg = PyDoubleArg("m","mut-rate", "Constant individual mutation factor.", True, 0.01, "double")
#            cmd.add( mu_arg )
#    
#            mo_arg = PyDoubleArg("M","mov-rate", "Constant individual movement factor.", True, 0.01, "double")
#            cmd.add( mo_arg )
#    
#            x1_arg = PyDoubleArg("x","x1-trait", "Constant value for resident's trait.", True, 0.01, "double")
#            cmd.add( x1_arg )
#    
#            x2_arg = PyDoubleArg("X","x2-trait", "Constant value for mutant's trait.", True, 0.02, "double")
#            cmd.add( x2_arg )
#    
#            Lr_arg = PyDoubleArg("c","child-rate", "Constant individual reproduction factor.", True, 0.01, "double")
#            cmd.add( Lr_arg )
#    
#            ri_arg = PyStringArg("r","run-id", "String identifier for this whole simulation.", True, "run-0", "string")
#            cmd.add( ri_arg )
#    
#            nr_arg = PyIntArg("n","nb-rep", "Number of repetitions.", True, 1, "int")
#            cmd.add( nr_arg )
#              
#                
#            ds_arg = PyDoubleArg("s","dump-step", "Dump simulation state every 'dump-step' seconds. \n\
#         Requires '-D' option to be enabled.", False, 0.01, "double")
#            cmd.add( ds_arg )
#    
#            xi_arg = PyStringArg("i","xml-input", "XML model input file.", False, "model.xml", "string")
#            cmd.add( xi_arg )
#    
#            o_arg = PyStringArg("o","out-prefix", "Text for prefixing the output filenames.", False, "output", "string")
#            cmd.add( o_arg )
#                
#            # Flags
#            dd_switch = PySwitchArg("D", "do-dump", "Dump / backup point every 'dump-step' seconds.", False)
#            cmd.add( dd_switch )
#            
#            r_switch = PySwitchArg("R", "recovery", "Starts the simulation from the last backup point.", False)
#            cmd.add( r_switch )
#
#            # Let's parse            
#            cmd.parse(argv)
#    
#            # Init values
#            nb_rep = nr_arg.getValue()
#            
#            d  = dr_arg.getValue()
#            b  = br_arg.getValue()
#            mu = mu_arg.getValue()
#            mo = mo_arg.getValue()
#            x1 = x1_arg.getValue()
#            x2 = x2_arg.getValue()
#            L  = Lr_arg.getValue()
#    
#            run_id = ri_arg.getValue();
#    
#            self.SetTimeDumpStep(ds_arg.getValue())
#            self.SetDoDump(dd_switch.getValue())
#            self.SetDoStartFromDump(r_switch.getValue())
#    
#            output_files_prefix = o_arg.getValue();
#            xml_input_filename = xi_arg.getValue();
#    
#            # Set net dump filename
#            net_dump_filename = output_files_prefix + ".lat." + run_id # + "." + str(GetCurrentRepetition())
#            self.SetNbRepetitions(nb_rep);
#            
#            
#            dd = dd_switch.getValue()
#            
#        # TODO (or not) : implement PyArgException (from the ArgException C++ one)
#        except Exception as e:          # catch any exceptions
#            print("Error:" + str(e))
#
#        return None
#
#    
#    def ParseXML(self, filename):
#
#        global d, b, mu, mo, x1, x2, L
#        global nb_rep, run_id
#        global output_files_prefix
#                
#        # Parse the args.
#        # TODO : handle file opening or parsing errors
#        doc = parse(filename)
#        root = doc.documentElement
#        
#        consts = root.getElementsByTagName("Constants")[0]
#        cst_vec = consts.getElementsByTagName("Const")
#        
#        for a_var in cst_vec:
#            str_n = a_var.attributes["name"].value
#            str_v = a_var.attributes["value"].value
#            
#            try: val = int(str_v)                                   # integer
#            except:                         
#                try: val = float(str_v)                             # float
#                except:     
#                    if (str_v == "True") or (str_v == "False"):     # boolean
#                        val = (str_v == "True")
#                    else: val = str_v                               # string
#                    
#            
#            t = type(val)
#            
#            if (str_n == "death-rate"):
#                if (t is float) or (t is int): d = val
#            elif (str_n == "birth-rate"):
#                if (t is float) or (t is int): b = val
#            elif (str_n == "mut-rate"):
#                if (t is float) or (t is int): mu = val
#            elif (str_n == "mov-rate"):
#                if (t is float) or (t is int): mo = val
#            elif (str_n == "x1-rate"):
#                if (t is float) or (t is int): x1 = val
#            elif (str_n == "x2-rate"):
#                if (t is float) or (t is int): x2 = val
#            elif (str_n == "l-rate"):
#                if (t is float) or (t is int): L = val
#            elif (str_n == "run-id"):
#                if (t is str): run_id = val
#            elif (str_n == "nb-rep"):
#                if (t is int): nb_rep = val
#            elif (str_n == "out-prefix"):
#                if (t is str): output_files_prefix = val
#            elif (str_n == "dump-step"):
#                if (t is float) or (t is int): self.SetTimeDumpStep(val)
#            elif (str_n == "do-dump"):
#                if (t is bool): self.SetDoDump(val)
#            elif (str_n == "recovery"):
#                if (t is bool): self.SetDoStartFromDump(val)
#            
#            # Set net dump filename
#            net_dump_filename = output_files_prefix + ".lat." + run_id
#        
#            self.SetNbRepetitions(nb_rep);
#             
    
#    def CheckInputs(self):
#        ok = True
#    
#        if (d  < 0.0 or d  > 1.0): ok = False
#        if (b  < 0.0 or b  > 1.0): ok = False
#        if (mu < 0.0 or mu > 1.0): ok = False
#        if (mo < 0.0 or mo > 1.0): ok = False
#        if (x1 < 0.0 or x1 > 1.0): ok = False
#        if (x2 < 0.0 or x2 > 1.0): ok = False
#        if (L  < 0.0 or L  > 1.0): ok = False
#    
#        if (not ok): print("The given d, b, mu, mo, x1, x2 or L value is not in xrange [0.0, 1.0]")
#        return ok
    
    def PrintInputs(self):
        global theta, mu, tho, sigma, rp, mut
        global E, S, I

        print("-------------------------------------")
        print("Input params for repetition '" + str(self.GetCurrentRepetition()) + "' :")
        print("-------------------------------------")
        print("S  = ", S)
        print("I  = ", I)
    
        # General rates
        print("theta  = ", theta)
        print("mu     = ", mu)
        print("tho    = ", tho)
        print("sigma  = ", sigma)
        print("rp     = ", rp)
        print("mut    = ", mut)
        print("-------------------------------------")


    def DefaultModels(self):
        
        global EMPTY, SUSCEPTIBLE, INFECTED
        global TREATED, GENO 

        global E_site, S_site, I_UT_site, I_T_site #, S_UT_site, S_T_site

        ### Create 1 model site for E : empty site
        E_site = PyLabSiteBase(self)
    
        # ATTRIBUTES
        E_site.PushAttribute("T", cyelp.IND_DEP, 0, True, 2)    # Treated or not [0 : false, 1 : true]
        E_site.PushAttribute("g", cyelp.IND_DEP, 0, True, 4)    # 4 possible pathogen genotypes
    
        # STATES
        E_site.PushState("EMPTY", 1, -1, -1)
        E_site.PushState("S",     1, -1, -1)
        E_site.PushState("I",     1, -1, -1)
    
        E_site.SetState(EMPTY, self.GetTime())
        E_site.SetAttrDouble(cyelp.IND_DEP, TREATED, -1)
        E_site.SetAttrDouble(cyelp.IND_DEP, GENO, -1)
    
    
        ### Create 1 models sites for S : susceptible sites - non-treated by default
        S_site = PyLabSiteBase(self)
        S_site.Copy(E_site, cyelp.IND_DEP)
        S_site.SetState(SUSCEPTIBLE, self.GetTime())
    
    
        ### Create 2 models sites for I : 1 for Untreated and 1 for Treated 

        # I-Untreated
        I_UT_site = PyLabSiteBase(self)
        I_UT_site.Copy(E_site, cyelp.IND_DEP)
        I_UT_site.SetAttrDouble(cyelp.IND_DEP, TREATED, 0)
        I_UT_site.SetAttrDouble(cyelp.IND_DEP, GENO, 0)
        I_UT_site.SetState(INFECTED, self.GetTime())
    
        # I-Treated
        I_T_site = PyLabSiteBase(self)
        I_T_site.Copy(I_UT_site, cyelp.IND_DEP)
        I_T_site.SetAttrDouble(cyelp.IND_DEP, TREATED, 1)



    def InitPopulation(self):
        global _C
        global popsize
                
        global E, S, I  
        global E_site, S_site, I_UT_site, I_T_site #, S_UT_site, S_T_site
        
        global tho
    
        print "Initializing population..."
    
        ### Create 5 stating models
        model_sites = []
        model_sites.append(E_site)
        model_sites.append(S_site)
        model_sites.append(I_UT_site)
        model_sites.append(I_T_site)
    
    
        # Populate with our 4 starting models
    
        I_T = int(round(I * tho))
        I_UT = int(I - I_T)
    
        popsize = S + I
    
#        # Cumsum of nomes must be 1.0 !!
#        dist = [0.63, 0.27, 0.02, 0.08]
#        self.arrayBinding.Populate1(nb_sites, dist, model_sites, True)
    
        # Cumsum of nomes must be equal to network's size !!
        dist = [E, S, I_UT, I_T]
        self.arrayBinding.Populate2(nb_sites, dist, model_sites, True)
    
    
        # Some counting useless tests
    
        U = _C.GS("U")
        universe = _C.CNT(U)
        print "UNIVERS = ", universe
        #del U
        universe = _C.GS("U").Cardinality()
        print "UNIVERS = ", universe


        empty = _C.GS("EMPTY").Cardinality()
        U = _C.GS("I") + _C.GS("S")                                 # Union
        full1 = (U).Cardinality()        
        full2 = (_C.GS("T0") + _C.GS("T1")).Cardinality()        
        occupied = (_C.GS("U") - _C.GS("EMPTY")).Cardinality()      # Difference
        treated = (_C.GS("T1")).Cardinality()
    
        print "OCCUPATION AT START >> ", universe, " | ", empty, " | ", occupied, " | ", treated
        print "OCCUPATION AT START >> ", full1, " | ", full2
    
        # Update frequency averages for the first time 
        self.Update_F_Averages()
        
        pass

    
    #
    # Update the propensity averages
    #
    def Update_F_Averages(self):
        
        global _C
        
        global virMOY, betaMOY, clearMOY
        global virMOY_I, virMOY_T, betaMOY_I, betaMOY_T, clearMOY_I, clearMOY_T
        
        global alpha, beta, gamma
        
        global VTCmtx


        nb_infect = float(_C.GS("I").Cardinality())
        nb_I      = float(_C.GS("T0").Cardinality())
        nb_T      = float(_C.GS("T1").Cardinality())
        
    
        virMOY     = 0.0
        virMOY_I   = 0.0
        virMOY_T   = 0.0
        betaMOY    = 0.0
        betaMOY_I  = 0.0
        betaMOY_T  = 0.0
        clearMOY   = 0.0
        clearMOY_I = 0.0
        clearMOY_T = 0.0
    
        for i in xrange(ngeno):

            s = "g" + str(i)
            
            I_UT = _C.GS("T0").CI(_C.GS(s))
            I_T  = _C.GS("T1").CI(_C.GS(s))
    
            if (nb_infect != 0.0):
#                virMOY += (vir[i]*gI[i] + virT[i]*gT[i]) / nb_infect
#                betaMOY += (beta[i]*gI[i] + betaT[i]*gT[i]) / nb_infect
#                clearMOY += (clear[i]*gI[i] + clearT[i]*gT[i]) / nb_infect
                virMOY   += ( VTCmtx[alpha][i][0] * I_UT + VTCmtx[alpha][i][1] * I_T )  /  nb_infect
                betaMOY  += ( VTCmtx[beta][i][0]  * I_UT + VTCmtx[beta][i][1] * I_T )   /  nb_infect
                clearMOY += ( VTCmtx[gamma][i][0] * I_UT + VTCmtx[gamma][i][1] * I_T )  /  nb_infect

            if (nb_I != 0.0):
#                virMOY_I += (vir[i]*gI[i]) / nb_I
#                clearMOY_I += (clear[i]*gI[i]) / nb_I
                virMOY_I   += (VTCmtx[alpha][i][0] * I_UT) / nb_I
                betaMOY_I  += (VTCmtx[beta][i][0]  * I_UT) / nb_I
                clearMOY_I += (VTCmtx[gamma][i][0] * I_UT) / nb_I

            if (nb_T != 0.0):
#                virMOY_T += (virT[i]*gT[i]) / nb_T
#                clearMOY_T += (clearT[i]*gT[i]) / nb_T
                virMOY_T   += (VTCmtx[alpha][i][1] * I_T) / nb_T
                betaMOY_T  += (VTCmtx[beta][i][1]  * I_T) / nb_T
                clearMOY_T += (VTCmtx[gamma][i][1] * I_T) / nb_T
    


    def RealizeEvent(self):
    
        global rng
        global nb_fired
    
        # Fires an event from the golbal eventStack
        p1 = rng.GetNextDouble()
        self.eventStack.RealizeEvent(p1, self.GetTime())
        self.eventStack.UpdateStackRates()
    
        self.Update_F_Averages()

        nb_fired += 1
            
    def Census(self):
        
        global _C
        
        global killedI, killedT, clearedI, clearedT
        global nb_fired

        
        self.outputer.DrawProgressBar(50, self.GetTime() / tt_end, False, True)
        print ""
        
        # Write a data line
        line = []
        tt = self.GetTime()
    
        if (tt == 0): self.outputer.InsertText("t    S    I_UT    I_T    I     (g0, g1, g2, g3)\n")
    
        line.append(tt)
    
        line.append(float(_C.GS("S").Cardinality()))
        line.append(float(_C.GS("T0").Cardinality()))
        line.append(float(_C.GS("T1").Cardinality()))
        line.append(float(_C.GS("I").Cardinality()))
        self.outputer.AddDataLine(line)
        self.outputer.WriteData()
        self.outputer.ResetDataSet()
    
        s = "( "    + str(_C.GS("g0").Cardinality()) + ", " \
                    + str(_C.GS("g1").Cardinality()) + ", " \
                    + str(_C.GS("g2").Cardinality()) + ", " \
                    + str(_C.GS("g3").Cardinality()) + " )\n"
        s += "K_UT/C_UT[" + str(killedI) + ", " + str(clearedI) + "]" + "   K_T/C_T[" + str(killedT) + ", " + str(clearedT) + "]\n"
        s += "[nb fired evts : " + str(nb_fired) + "]\n"
        self.outputer.InsertText(s)



    def SetOutputFilnames(self):
        global run_id
        global output_files_prefix
        self.outputFilename = output_files_prefix + "." + run_id + "." + str(self.GetCurrentRepetition())    
        if (self.outputer != None): 
            self.outputer.SetFilename(self.outputFilename, not(self.GetDoStartFromDump()))


    #@override
    def InitSimulation(self):
        
        global rng
        #global output_files_prefix
        
        rng = PyLabRng()

        # Populate the network 
        self.InitPopulation()
    
        # Outputs

        self.outputer = None
        if (self.outputer == None):
            self.outputer = PyLabOutputerLine()
            self.outputer.SetOuputMode(cyelp.TO_BOTH)
            self.outputer.SetSeparator("\t")
#            self.outputer.SetFilename(self.outputFilename)
        
        self.SetOutputFilnames()

        # Possible events [BIRTH, DEATH, INFECTION, CLEARANCE, SUPERINFECTION, MUTATION]
    
        rate_callback = PyLabCallBack(BirthRate)
        realize_callback = PyLabCallBack(RealizeBirth)
        self.eventStack.PushEvent("BIRTH", 0, None, rate_callback, realize_callback)
       
    
        rate_callback = PyLabCallBack(DeathRate)
        realize_callback = PyLabCallBack(RealizeDeath)
        self.eventStack.PushEvent("DEATH", 0, None, rate_callback, realize_callback)
    
    
        rate_callback = PyLabCallBack(InfectionRate)
        realize_callback = PyLabCallBack(RealizeInfection)
        self.eventStack.PushEvent("INFECTION", 0, None, rate_callback, realize_callback)
    
    
        rate_callback = PyLabCallBack(ClearanceRate)
        realize_callback = PyLabCallBack(RealizeClearance)
        self.eventStack.PushEvent("CLEARANCE", 0, None, rate_callback, realize_callback)
    
    
        rate_callback = PyLabCallBack(SuperinfectionRate)
        realize_callback = PyLabCallBack(RealizeSuperinfection)
        self.eventStack.PushEvent("SUPERINFECTION", 0, None, rate_callback, realize_callback)
    
    
        rate_callback = PyLabCallBack(MutationRate)
        realize_callback = PyLabCallBack(RealizeMutation)
        self.eventStack.PushEvent("MUTATION", 0, None, rate_callback, realize_callback)
    
    

        self.eventStack.UpdateStackRates()
        self.UpdateEventsRate(self.eventStack.GetStackRate())
    
    
        self.SetEndTime(tt_end)
        self.SetOutputTimeStep(tt_out)



    #@override
    def OutputSimulation(self):
        # Census pop
        self.Census()
        pass

    #@override
    def BeforeRun(self):
        
        global timer
        timer = PyLabTimer()
        timer.Start()
        
        # Params
        self.PrintInputs()


    #@override
    def AfterRun(self):
        
        print "\n"
        
        global timer
        timer.Stop()
        print "TOTAL SIMULATION TIME :"
        timer.Print()
        ###del timer


    #@override
    def BeforeStep(self):
        self.UpdateEventsRate(self.eventStack.GetStackRate());
        
        # Check extinction :
        if (_C.GS("I").Cardinality() == 0):
            print "Extinction at t=" + str(self.GetTime()) + " (Simulation terminated) !"
            sys.exit(0)
        pass

    #@override
    def AfterStep(self):
        # self.outputer.DrawProgressBar(50, self.GetTime() / tt_end, True)
        pass


    #@override
    def FireEvent(self):
    
        # Choose and realize an event
        self.RealizeEvent()
    
        self.UpdateEventsRate(self.eventStack.GetStackRate())
        pass



#    
#    #@override
#    def BinarizeAll(self):
#        pass
#        
#    #@override
#    def UnBinarizeAll(self):
#        pass

#    #@override
#    #Do Not override in the python version !
#    def Run(self):
#        pass


        
#@main
def main():

    global xml_input_filename
    global nb_sites
    
    global _C
    
    
    # Set sets calculator
    _C = PyLabSiteSetsCalculator()
    
    # Intantiate simulator
    sim = DrugRes()
    

    sim.SetMaxPopulationSize(nb_sites)

    # Initialize with default hardcoded values
    sim.DefaultInputs()

    # Initialize individuals models
    sim.DefaultModels()
    
#    # TODO : Parsing from CL or XML
#    # (Replace input model configuration by values from CL or XML)
#    xml_input_filename = sim.ParseCL(sys.argv)
#    if (xml_input_filename != None):
#        print("Loading XML model : ",  xml_input_filename);
#        sim.ParseXML(xml_input_filename);
#    
#    ok = sim.CheckInputs()
#    if (not ok): return -1
#    
#    sim.ofs = Py_ofstream()
#    sim.ifs = Py_ifstream()
#    sim.SetBinaryStreams(sim.ofs, sim.ifs)
    
    
    # Start the thing
    print ("LET'S START !")
    sim.Run()


#@run
if __name__ == "__main__":
    # Try to deal with Ctrl+C to abort the running simulation in terminal
    # (Doesn't work in Python - Don't really know why...)
    try:
        sys.exit(main())
    except (KeyboardInterrupt, SystemExit):
        print '\n! Received keyboard interrupt, quitting threads.\n'

