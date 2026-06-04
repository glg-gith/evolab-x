import sys
import math

from xml.dom.minidom import parse

import cyelp

from cyelp import PyLabSimulatorTimeNet
#from cyelp import PyLabNetBinding
from cyelp import PyLabNetwork
#from cyelp import PyLabNode
from cyelp import PyLabRng
from cyelp import PyLabTimer
from cyelp import PyLabOutputerLine

from cyelp import PyLabSiteEvent

from cyelp import Py_ofstream
from cyelp import Py_ifstream
from cyelp import PyLabBinaryUtils

from cyelp import PyLabCallBack

from cyelp import PyDoubleArg
from cyelp import PyIntArg
from cyelp import PyStringArg
from cyelp import PySwitchArg
from cyelp import PyCmdLine

from cyelp import PyLabSet
from cyelp import PyLabSiteSetsCalculator

#from cyelp import ForLooper

from cyelp import PyLabUserData
from cyelp import PyLabSiteBase

#from cyelp import PyLabSiteSetsManager

import numpy as np

'''
# GLOBALS
'''
run_id      = "0"
nb_rep      = 6
nb_sites    = 100000 #100000;//100;//
con_deg     = 4                    # Number of connected neighbors per node
nb_cols     = 100 #100;//10;//     # Number of columns for display

p_init      = 0.5 #0.5;//0.01
tt_out      = 1                    # output par pas de 1
tt_out_st   = 5
tt_end      = 50                   # when simulation ends

resynch     = 500                  # recalculate totalrate periodically to avoid numerical drift

totalrate = None
popsize = None                     # total pop size

output_files_prefix = None
net_dump_filename = None           # where to dump the network connections as text edges list
xml_input_filename = None          # XML file to be parsed


rng = None
timer = None

arr_mn = np.empty([nb_sites], dtype=float)

# Handling correct reference counting on the objects (Python specific)
# @See : http://stackoverflow.com/questions/12831280/how-python-handles-object-instantiation-in-a-for-loop
# No more required : done in PyLabNetBinding and PyLabSiteEvent now !!
#global l_site
#global l_cb

_C = None


'''
# INPUTS
'''
#double d, b, mu, mo, x1, x2, L;
d = b = mu = mo = x1 = x2 = L = None 


'''
MISC
'''
#typedef enum { EMPTY, RESIDENT, MUTANT } t_state;
EMPTY    = 0
RESIDENT = 1
MUTANT   = 2

#typedef enum { X, M } t_site_attr;
X = 0
M = 1

'''
RATES CALLBACKS
'''

#@callback
def NoneRate(node):
#    print "COOL !!"
    return 0.0

#@callback
def BirthRate(node):
    a_site = node.GetUserData()
    rate = b * a_site.GetAttrDouble(cyelp.IND_DEP, X)
    if (a_site.GetState() == MUTANT):
        rate *= 0.9;
    return rate;

#@callback
def DeathRate(node):
    a_site = node.GetUserData()
    return d * a_site.GetAttrDouble(cyelp.IND_DEP, X)

#@callback
def MovementRate(node):
    rate = node.GetUserData().GetAttrDouble(cyelp.IND_DEP, M)
    ##rate *= 1.0 - BdmLsim4.NbOccupiedNghs(node) / con_deg
    return rate;

#@callback
def MutationRate(node):
    return mu;


'''
REALIZES CALLBACKS
'''

#@callback
def RealizeNone(node):
    #print "NONE fired !"
    return 0.0

#@callback
def RealizeBirth(node):
#    print "BIRTH fired !"
    
    global popsize

    # Reproduce globally with probability L
    if (rng.GetNextDouble() < L):
        while True:
            birth_site = rng.GetNextInt(nb_sites)
            n_birthsite = node.GetNetwork().GetNodes()[birth_site]
            if (n_birthsite != node):
                break
    # Locally with probability 1-L
    else:
        birth_neighbour = rng.GetNextInt(con_deg)
        ###n_birthsite = node.GetNeighbors()[birth_neighbour]
        n_birthsite = node.GetNeighborAt(birth_neighbour)

    # Reproduction only if birth_site is empty
    s_data = node.GetUserData()
    bs_data = n_birthsite.GetUserData()
    if (bs_data.GetState() == EMPTY):
        # Sites update
        bs_data.SetState(s_data.GetState(), s_data.GetTimeEvent())
        bs_data.Copy3(s_data, cyelp.IND_DEP);

        # Rates update
        BdmLsim4.UpdateRatesLocal(node);
        BdmLsim4.UpdateRatesLocal(n_birthsite);

        # Population size update
        popsize += 1


    return 0.0


#@callback
def RealizeDeath(node):
    
    global popsize

    # Switch to EMPTY state
    s_data = node.GetUserData()
    s_data.SetState(EMPTY, s_data.GetTimeEvent())
    s_data.SetAttrDouble(cyelp.IND_DEP, X, 0.0)
    s_data.SetAttrDouble(cyelp.IND_DEP, M, 0.0)

    # Rates update
    BdmLsim4.UpdateRatesLocal(node)

    popsize -= 1

    return 0.0
    

#@callback
#Not yet translated
def RealizeMovement(node):
    #print "MOVEMENT fired !"

    # TODO : The following should work : but is UNTESTED !!!
    # Ask Seb about this...
    
    # Choose a site to move to
    move_site = rng.GetNextInt(con_deg)
    ###n_movesite = node.GetNeighbors()[move_site];
    n_movesite = node.GetNeighborAt(move_site);

    # Try moving
    s_data = node.GetUserData()
    ms_data = n_movesite.GetUserData()
    if (ms_data.GetState() == EMPTY):
        # Copy old location properties to the new one :
        ms_data.Copy3(s_data, cyelp.IND_DEP);

        # Switch old location to EMPTY state :
        s_data.SetState(EMPTY, s_data.GetTimeEvent())
        s_data.SetAttrDouble(cyelp.IND_DEP, X, 0.0)
        s_data.SetAttrDouble(cyelp.IND_DEP, M, 0.0)

        # Rates update
        BdmLsim4.UpdateRatesLocal(node)
        BdmLsim4.UpdateRatesLocal(n_movesite)
        
    return 0.0

#@callback
def RealizeMutation(node):
    #print "MUTATION fired !"

    # Mutate
    s_data = node.GetUserData()
    if (s_data.GetState() == RESIDENT):
        s_data.SetState(MUTANT, s_data.GetTimeEvent());
        s_data.SetAttrDouble(cyelp.IND_DEP, X, x2);
        #s_data->x = x2;
    else:
        s_data.SetState(RESIDENT, s_data.GetTimeEvent());
        s_data.SetAttrDouble(cyelp.IND_DEP, X, x1);
        #s_data->x = x1;

    # Rates update
    BdmLsim4.UpdateRatesLocal(node)

    return 0.0


'''
# SIM CLASS : The main simulation definition class to be run
'''
class BdmLsim4(PyLabSimulatorTimeNet):
    
    # Dump files
    outputer = None
    outputer2 = None
    outputer3 = None
    outputFilename = None;
    outputFilename2 = None;
    outputFilename3 = None;
    
    # Binary streams
    ofs = None
    ifs = None
    
    def __init__(self):
        global output_files_prefix
        output_files_prefix = "output"

    
    def DefaultInputs(self):
        global d, b, mu, mo, x1, x2, L
        global net_dump_filename
        
        d     = 0.01   # Death rate
        b     = 0.01   # Birth rate
        mu    = 0.01   # Mutation rate
        mo    = 0.01   # Movement rate
        x1    = 0.01   # Trait coeff for Resident
        x2    = 0.02   # Trait coeff for Mutant
        L     = 0.01   # Reproduction rate
    
        net_dump_filename = "bdmLsim.res.lat." + run_id
        self.SetNbRepetitions(nb_rep)
        
        self.SetTimeDumpStep(1)
        self.SetDoDump(False)
        self.SetDoStartFromDump(False)

    
    def ParseCL(self, argv):
        global d, b, mu, mo, x1, x2, L
        global nb_rep, run_id
        global output_files_prefix, xml_input_filename

    
        # If XML input was set : skip CL parsing
        argc = len(argv)
        for i in xrange(argc):
            if (argc-1 > i and argv[i] == "-i"):
                return argv[i+1]
    
        # Wrap everything in a try block.  Do this every time,
        # because exceptions will be thrown for problems.
        try:
            
            cmd = PyCmdLine("BdmLsim4-python simulation (using EVOLAB-X1 project)", ord(' '), "0.4", True)
            
            dr_arg = PyDoubleArg("d", "death-rate", "Constant individual death factor.", True, 0.01, "double")
            cmd.add( dr_arg )
            
            br_arg = PyDoubleArg("b", "birth-rate", "Constant individual birth factor.", True, 0.01, "double")
            cmd.add( br_arg )
            
            mu_arg = PyDoubleArg("m","mut-rate", "Constant individual mutation factor.", True, 0.01, "double")
            cmd.add( mu_arg )
    
            mo_arg = PyDoubleArg("M","mov-rate", "Constant individual movement factor.", True, 0.01, "double")
            cmd.add( mo_arg )
    
            x1_arg = PyDoubleArg("x","x1-trait", "Constant value for resident's trait.", True, 0.01, "double")
            cmd.add( x1_arg )
    
            x2_arg = PyDoubleArg("X","x2-trait", "Constant value for mutant's trait.", True, 0.02, "double")
            cmd.add( x2_arg )
    
            Lr_arg = PyDoubleArg("c","child-rate", "Constant individual reproduction factor.", True, 0.01, "double")
            cmd.add( Lr_arg )
    
            ri_arg = PyStringArg("r","run-id", "String identifier for this whole simulation.", True, "run-0", "string")
            cmd.add( ri_arg )
    
            nr_arg = PyIntArg("n","nb-rep", "Number of repetitions.", True, 1, "int")
            cmd.add( nr_arg )
              
                
            ds_arg = PyDoubleArg("s","dump-step", "Dump simulation state every 'dump-step' seconds. \n\
         Requires '-D' option to be enabled.", False, 0.01, "double")
            cmd.add( ds_arg )
    
            xi_arg = PyStringArg("i","xml-input", "XML model input file.", False, "model.xml", "string")
            cmd.add( xi_arg )
    
            o_arg = PyStringArg("o","out-prefix", "Text for prefixing the output filenames.", False, "output", "string")
            cmd.add( o_arg )
                
            # Flags
            dd_switch = PySwitchArg("D", "do-dump", "Dump / backup point every 'dump-step' seconds.", False)
            cmd.add( dd_switch )
            
            r_switch = PySwitchArg("R", "recovery", "Starts the simulation from the last backup point.", False)
            cmd.add( r_switch )

            # Let's parse            
            cmd.parse(argv)
    
            # Init values
            nb_rep = nr_arg.getValue()
            
            d  = dr_arg.getValue()
            b  = br_arg.getValue()
            mu = mu_arg.getValue()
            mo = mo_arg.getValue()
            x1 = x1_arg.getValue()
            x2 = x2_arg.getValue()
            L  = Lr_arg.getValue()
    
            run_id = ri_arg.getValue();
    
            self.SetTimeDumpStep(ds_arg.getValue())
            self.SetDoDump(dd_switch.getValue())
            self.SetDoStartFromDump(r_switch.getValue())
    
            output_files_prefix = o_arg.getValue();
            xml_input_filename = xi_arg.getValue();
    
            # Set net dump filename
            net_dump_filename = output_files_prefix + ".lat." + run_id # + "." + str(GetCurrentRepetition())
            self.SetNbRepetitions(nb_rep);
            
            
            dd = dd_switch.getValue()
            
        # TODO (or not) : implement PyArgException (from the ArgException C++ one)
        except Exception as e:          # catch any exceptions
            print("Error:" + str(e))

        return None

    
    def ParseXML(self, filename):

        global d, b, mu, mo, x1, x2, L
        global nb_rep, run_id
        global output_files_prefix
                
        # Parse the args.
        # TODO : handle file opening or parsing errors
        doc = parse(filename)
        root = doc.documentElement
        
        consts = root.getElementsByTagName("Constants")[0]
        cst_vec = consts.getElementsByTagName("Const")
        
        for a_var in cst_vec:
            str_n = a_var.attributes["name"].value
            str_v = a_var.attributes["value"].value
            
            try: val = int(str_v)                                   # integer
            except:                         
                try: val = float(str_v)                             # float
                except:     
                    if (str_v == "True") or (str_v == "False"):     # boolean
                        val = (str_v == "True")
                    else: val = str_v                               # string
                    
            
            t = type(val)
            
            if (str_n == "death-rate"):
                if (t is float) or (t is int): d = val
            elif (str_n == "birth-rate"):
                if (t is float) or (t is int): b = val
            elif (str_n == "mut-rate"):
                if (t is float) or (t is int): mu = val
            elif (str_n == "mov-rate"):
                if (t is float) or (t is int): mo = val
            elif (str_n == "x1-rate"):
                if (t is float) or (t is int): x1 = val
            elif (str_n == "x2-rate"):
                if (t is float) or (t is int): x2 = val
            elif (str_n == "l-rate"):
                if (t is float) or (t is int): L = val
            elif (str_n == "run-id"):
                if (t is str): run_id = val
            elif (str_n == "nb-rep"):
                if (t is int): nb_rep = val
            elif (str_n == "out-prefix"):
                if (t is str): output_files_prefix = val
            elif (str_n == "dump-step"):
                if (t is float) or (t is int): self.SetTimeDumpStep(val)
            elif (str_n == "do-dump"):
                if (t is bool): self.SetDoDump(val)
            elif (str_n == "recovery"):
                if (t is bool): self.SetDoStartFromDump(val)
            
            # Set net dump filename
            net_dump_filename = output_files_prefix + ".lat." + run_id
        
            self.SetNbRepetitions(nb_rep);
             
    
    def CheckInputs(self):
        ok = True
    
        if (d  < 0.0 or d  > 1.0): ok = False
        if (b  < 0.0 or b  > 1.0): ok = False
        if (mu < 0.0 or mu > 1.0): ok = False
        if (mo < 0.0 or mo > 1.0): ok = False
        if (x1 < 0.0 or x1 > 1.0): ok = False
        if (x2 < 0.0 or x2 > 1.0): ok = False
        if (L  < 0.0 or L  > 1.0): ok = False
    
        if (not ok): print("The given d, b, mu, mo, x1, x2 or L value is not in xrange [0.0, 1.0]")
        return ok
    
    def PrintInputs(self):
        print("-------------------------------------")
        print("Input params for repetition '" + str(self.GetCurrentRepetition()) + "' :")
        print("-------------------------------------")
        print("d  =", d)
        print("b  =", b)
        print("mu =", mu)
        print("mo =", mo)
        print("x1 =", x1)
        print("x2 =", x2)
        print("L  =", L)
        print("-------------------------------------")

    
    @classmethod
    def NbOccupiedNghs(self, node):
        nb = 0;
        ###nghs = node.GetNeighbors();
        ###it = iter(nghs)
        ###for ngh in it:
        for i in range(node.GetNeighborsCount()):
            ###occupied = ( ngh.GetUserData().GetState() != EMPTY )
            occupied = ( node.GetNeighborAt(i).GetUserData().GetState() != EMPTY )
            if (occupied):
                nb += 1;
        return nb;

    
    @classmethod
    def UpdateRatesLocal(self, node):
        BdmLsim4.UpdateRates(node)
        ###nghs = node.GetNeighbors()
        ###it = iter(nghs)
        ###for ngh in it:
        for i in range(node.GetNeighborsCount()):
            ###BdmLsim4.UpdateRates(ngh);
            BdmLsim4.UpdateRates(node.GetNeighborAt(i));


    @classmethod
    def UpdateRates(self, node):
        global totalrate
        a_site = node.GetUserData()
        
        totalrate -= a_site.GetEventStackRate()
    
        if (a_site.GetState() != EMPTY):
            a_site.UpdateEventStackRates()
        else:
            a_site.ResetEventStackRate()
            
        arr_mn[node.GetIndex()] = a_site.GetEventStackRate()
    
        totalrate += a_site.GetEventStackRate()


    def ResynchTotalRate(self):
        global totalrate
        totalrate = 0.0
        it = iter(self.netBinding.GetNetwork().GetNodes())
        
        for item in it:
            a_site = item.GetUserData()
            totalrate += a_site.GetEventStackRate()


    def RealizeEvent(self):
        global totalrate

        # Choose a site
#        add = 0.0;
#        p1 = rng.GetNextDouble() * totalrate
        
        nodes = self.netBinding.GetNetwork().GetNodes()
#        length = len(nodes)
#        n = 0
#        for i in xrange(length):
#            add += nodes[i].GetUserData().GetEventStackRate()
#            if (p1 < add): 
#                break
#            else:
#                n += 1
        n = rng.GetNextIntMultinomial(totalrate, arr_mn)

        p1 = rng.GetNextDouble()
        # Realize event for site 'n'
        nodes[n].GetUserData().RealizeEvent(p1, self.GetTime());
        
            
    def Census(self):
        nx = 0
        nox = 0
        nxx = 0
        noox = 0
        nxox = 0
    
        nodes = self.netBinding.GetNetwork().GetNodes()
        it = iter(nodes)
        for node in it:
            if (node.GetUserData().GetState() != EMPTY):
                nx += 1
    
#                it2 = iter(node.GetNeighbors())
#                for ngh in it2:
#                    if (ngh.GetUserData().GetState() != EMPTY):
#                        nox += 1
#                        it3 = iter(ngh.GetNeighbors())
#                        for ngh2 in it3:
#                            if (ngh2.GetUserData().GetState() == EMPTY):
#                                noox += 1
#                            else:
#                                if (ngh2 != node):
#                                    nxox += 1
#                        # END IT3
#                    else:
#                        nxx += 1
#                # END IT2


                for i in range(node.GetNeighborsCount()):
                    ngh = node.GetNeighborAt(i)
                    if (ngh.GetUserData().GetState() != EMPTY):
                        nox += 1
                        for j in range(ngh.GetNeighborsCount()):
                            ngh2 = ngh.GetNeighborAt(j)
                            if (ngh2.GetUserData().GetState() == EMPTY):
                                noox += 1
                            else:
                                if (ngh2 != node):
                                    nxox += 1
                        # END IT3
                    else:
                        nxx += 1
                # END IT2
                
            # END IF STATE
        # END IT
    
        # Write a data line
        line = []
        tt = self.GetTime()
    
        line.append(tt)
        line.append(nx)
        line.append(nox)
        line.append(nxx)
        line.append(nxox)
        line.append(noox)
        self.outputer.AddDataLine(line)
        if (tt == 0):
            self.outputer2.InsertText("t nx nox nxx nxox noox\n")
        self.outputer2.AddDataLine(line)
    
        if ( math.floor(tt) % tt_out_st == 0 ):
            line[:] = []
            line.append(tt)
            self.outputer2.AddDataLine(line)
            line[:] = []
    
            i = 0
            it = iter(nodes)
            for node in it:
                line.append(node.GetUserData().GetState())
                i += 1
                if (i % nb_cols == 0):
                    self.outputer2.AddDataLine(line)
                    line[:] = []
    
        self.outputer.WriteData()
        self.outputer.ResetDataSet()
    
        self.outputer2.InsertText("\n")
        self.outputer2.WriteData()
        self.outputer2.ResetDataSet()
        
        return (nx);

    
    def DumpLattice(self):
        i = 0
    
        a_line = []
    
        nodes = self.netBinding.GetNetwork().GetNodes()
        it = iter(nodes)
        for node in it:
            a_site = node.GetUserData()
            a_line[:] = []
            a_line.append(i)
            a_line.append(a_site.GetState())
            a_line.append(a_site.GetAttrDouble(cyelp.IND_DEP, X))
            self.outputer3.AddDataLine(a_line)
            i += 1
    
        self.outputer3.WriteData();
        self.outputer3.ResetDataSet();


    def SetOutputFilnames(self):
        # Re-synchronize output filenames
        self.outputFilename  = output_files_prefix + "." + run_id + "." + str(self.GetCurrentRepetition())
        self.outputFilename2 = "spat-" + self.outputFilename
        self.outputFilename3 = "dump.state-L" + str(L) + "-" + self.outputFilename
        if (self.outputer != None):
            self.outputer.SetFilename(self.outputFilename, (not self.GetDoStartFromDump()))
        if (self.outputer2 != None):
            self.outputer2.SetFilename(self.outputFilename2, (not self.GetDoStartFromDump()))
        if (self.outputer3 != None):
            self.outputer3.SetFilename(self.outputFilename3, (not self.GetDoStartFromDump()))


    def InitPopulation(self):
        global popsize
        
        timer3 = PyLabTimer()
        timer3.Start()

        popsize = 0
        nodes = self.netBinding.GetNetwork().GetNodes()
        
        print "Initializing population..."
        
        self.SetMaxPopulationSize(nb_sites) 
    
        ## EVENTS (Common refs - callback functions)
    
        # Create 2 models : 1 for Residents and 1 for Empties
        
        
        
                
        # Let's start populating the network
        it = iter(nodes)
        for item in it:
            a_site = PyLabSiteEvent(self, item)
            
#            l_site.append(a_site)
                
            # ATTRIBUTES
            a_site.PushAttribute("x", cyelp.IND_DEP)
            a_site.PushAttribute("m", cyelp.IND_DEP)
    
            # STATES
            a_site.PushState("EMPTY", 1, -1, -1)
            a_site.PushState("RESIDENT", 1, -1, -1)
            a_site.PushState("MUTANT", 1, -1, -1)
            
            if (rng.GetNextDouble() < p_init):
                a_site.SetState(RESIDENT, self.GetTime())
                a_site.SetAttrDouble(cyelp.IND_DEP, X, x1)
                #a_site.SetAttrDouble(cyelp.IND_DEP, M, mo)
                a_site.SetAttrDouble(cyelp.IND_DEP, M, 0.0)
    
                popsize += 1
            else:
                a_site.SetState(EMPTY, self.GetTime());
                a_site.SetAttrDouble(cyelp.IND_DEP, X, 0.0);
                a_site.SetAttrDouble(cyelp.IND_DEP, M, 0.0);
    
            # EVENTS
            rate_callback = PyLabCallBack(NoneRate)
            realize_callback = PyLabCallBack(RealizeNone)
            a_site.PushEvent("NONE", 0, item, rate_callback, realize_callback);
#            l_cb.append(rate_callback)
#            l_cb.append(realize_callback)
    
            rate_callback = PyLabCallBack(BirthRate)
            realize_callback = PyLabCallBack(RealizeBirth)
            a_site.PushEvent("BIRTH", 0, item, rate_callback, realize_callback);
    
            rate_callback = PyLabCallBack(DeathRate)
            realize_callback = PyLabCallBack(RealizeDeath)
            a_site.PushEvent("DEATH", 0, item, rate_callback, realize_callback);
    
            rate_callback = PyLabCallBack(MovementRate)
            realize_callback = PyLabCallBack(RealizeMovement)
            a_site.PushEvent("MOVEMENT", 0, item, rate_callback, realize_callback);
    
            rate_callback = PyLabCallBack(MutationRate)
            realize_callback = PyLabCallBack(RealizeMutation)
            a_site.PushEvent("MUTATION", 0, item, rate_callback, realize_callback);
    
            
            item.SetUserData(a_site)
            
        # END IT

        it = iter(nodes)
        for item in it:
            BdmLsim4.UpdateRates(item)
        self.ResynchTotalRate()
        
        timer3.Stop()
        print "INIT POPULATION :"
        timer3.Print()


    #@override
    def InitSimulation(self):
        
        global totalrate, rng, output_files_prefix
 
        totalrate = 0.0
        
        rng = PyLabRng()

        # Populate the network 
        self.InitPopulation()
    
        # Outputs

        self.outputer = None
        self.outputer2 = None
        self.outputer3 = None
        if (self.outputer == None):
            self.outputer = PyLabOutputerLine()
            self.outputer.SetOuputMode(cyelp.TO_BOTH)
            self.outputer.SetSeparator("\t")
#            self.outputer.SetFilename(self.outputFilename)
        
        if (self.outputer2 == None):
            self.outputer2 = PyLabOutputerLine()
            self.outputer2.SetOuputMode(cyelp.TO_FILE)
            self.outputer2.SetSeparator("\t")
#            self.outputer2.SetFilename(self.outputFilename2)
        
        if (self.outputer3 == None):
            self.outputer3 = PyLabOutputerLine()
            self.outputer3.SetOuputMode(cyelp.TO_FILE)
            self.outputer3.SetSeparator("\t")
#            self.outputer3.SetFilename(self.outputFilename3)
        
        self.SetOutputFilnames()

    
    
        self.UpdateEventsRate(totalrate)
        self.SetEndTime(tt_end)
        self.SetOutputTimeStep(tt_out)


    #@override
    def OutputSimulation(self):
#        # Census pop
#        self.Census()
#        # Periodically dump lattice state
#        self.DumpLattice()
        pass
 

    #@override
    def BeforeRun(self):
        
        global timer
        timer = PyLabTimer()
        timer.Start()
        
        # Params
        self.PrintInputs()
    
        # Save the generated network
        if (self.GetDoDump() and not(self.GetDoStartFromDump())):
            self.netBinding.SaveNetwork(net_dump_filename)


    #@override
    def AfterRun(self):
        
        print "\n"
        
        global timer
        timer.Stop()
        print "TOTAL SIMULATION TIME :"
        timer.Print()
        del timer


    #@override
    def BeforeStep(self):
        self.UpdateEventsRate(totalrate)

#    #@override
    def AfterStep(self):
        self.outputer.DrawProgressBar(50, self.GetTime() / tt_end, True);
        pass


    #@override
    def FireEvent(self):
        global totalrate
        # Choose and realize an event
        self.RealizeEvent();    
        # Periodically recalculate totalrate
        if (self.GetNbFiredEvents() % resynch == 0):
            self.ResynchTotalRate();
    
        self.UpdateEventsRate(totalrate);


    
    #@override
    def BinarizeAll(self):
        bu = PyLabBinaryUtils()
        
        # Binarize current simulation time
        bu.WriteDoubleToStream(self.GetTime(), self.ofs);
    
        # Binarize current simulation repetition
        bu.WriteIntToStream(self.GetCurrentRepetition(), self.ofs);
    
        # Binarize all sites
        it = iter(self.netBinding.GetNetwork().GetNodes())
        for node in it:
            node.GetUserData().Binarize(self.ofs);
            #print node.GetUserData().GetState()
            
        
    #@override
    def UnBinarizeAll(self):
        bu = PyLabBinaryUtils()
        
        # UnBinarize current simulation time
        time = bu.ReadDoubleFromStream(self.ifs)
        self.SetTime(time)
    
        # UnBinarize current simulation repetition
        rep_num = bu.ReadIntFromStream(self.ifs)
        self.SetCurrentRepetition(rep_num)
        # Update filenames according to the rep num
        self.SetOutputFilnames()                
    
        print("Starting at t =", self.GetTime())
        print(" ( rep =", self.GetCurrentRepetition(), ")")
    
        # UnBinarize all sites
        it = iter(self.netBinding.GetNetwork().GetNodes())
        for node in it:
            node.GetUserData().UnBinarize(self.ifs)
            ###if (len(node.GetNeighbors()) != 4):
            if (node.GetNeighborsCount() != 4):
                print("3=", node.GetNeighborsCount())


#    #@override
#    #Do Not override in the python version !
#    def Run(self):
#        pass


#@main
def main():

    global xml_input_filename, net_dump_filename
    global nb_sites, con_deg
    
    global _C
    
    
    # Set sets calculator
    _C = PyLabSiteSetsCalculator()
    
    # Intantiate simulator
    sim = BdmLsim4()
    
    
    sim.DefaultInputs()   
    
    # TODO : Parsing from CL or XML
    # (Replace input model configuration by values from CL or XML)
    xml_input_filename = sim.ParseCL(sys.argv)
    if (xml_input_filename != None):
        print("Loading XML model : ",  xml_input_filename);
        sim.ParseXML(xml_input_filename);
    
    ok = sim.CheckInputs()
    if (not ok): return -1
    
    sim.ofs = Py_ofstream()
    sim.ifs = Py_ifstream()
    sim.SetBinaryStreams(sim.ofs, sim.ifs)
    
     
    timer2 = PyLabTimer()
    timer2.Start()
   
    # Initialize network
    if (sim.GetDoStartFromDump()):
        sim.netBinding.LoadNetwork(net_dump_filename)
    else:
        net = PyLabNetwork()
        
        net.Populate(nb_sites)
        
#        net.populate(nb_sites)
#        
#        if ((con_deg * net.size()) % 2 == 1):
#            print ("The sum of all degrees must be even\nThis is not possible with the network parameters you have specified.")
#            return -1;
#        
#        dist = [0.0 for i in xrange(con_deg+1)]
#        dist[con_deg] = 1.0
#        if (not net.rand_connect_user(dist)):
#            print ("NETWORK CONNECTION FAILED !")
#            return -1;
#        
#        sim.netBinding.SetNetwork(net)

        ok = net.RandConnectUserLinear(con_deg)
        print "CONNECTION ? ", ok
        sim.netBinding.SetNetwork(net)
    
    timer2.Stop()
    print "BUILDING NETWORK :"
    timer2.Print()

    print ("NB NETWORK NODES = ", sim.netBinding.GetNetwork().GetSize())
    
    # Start the thing
    print ("LET'S START !")
    sim.Run()


#@run
if __name__ == "__main__":
    try:
        sys.exit(main())
    except (KeyboardInterrupt, SystemExit):
        print '\n! Received keyboard interrupt, quitting threads.\n'

