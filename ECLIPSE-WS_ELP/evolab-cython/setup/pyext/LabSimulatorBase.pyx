from cpython.ref cimport PyObject
from libcpp.string cimport string

#import sys

# cdef extern from "adapter/ALabSimulatorBase.h" namespace "elps" :
#     cdef cppclass ALabSimulatorBase:
cdef extern from "../inc/LabSimulatorBase.h" namespace "elps" :
    cdef cppclass LabSimulatorBase:
    
        #ALabSimulatorBase(PyObject *obj)
        LabSimulatorBase(PyObject *obj)
        # Must not be overridable as it is the entry point
        # for running the simulation
        void Run()
        
        ###
        int GetNbRepetitions()
        void SetNbRepetitions(int nb_rep)
        int GetCurrentRepetition()
        void SetCurrentRepetition(int rep_num)
        
        PyObject * CyGetPyObj()
        void CySetPyObj(PyObject *obj)
        # Virtual overridable
#         void InitSimulation()
#         void StepSimulation()
#         void BeforeStep()
#         void AfterStep()
#         void OutputSimulation()
#         bint EndSimulation()
#     
#         void InitUnBinarize()
#         void StepBinarize()
#     
#         void RunSimulation()
#         void BeforeRun()
#         void AfterRun()
#     
#         void BinarizeAll()
#         void UnBinarizeAll()
    
        ###
        void SetBinaryStreams(ofstream *ofs, ifstream *ifs)
        int GetLastDumpFileIdAndClean()
        void CleanUnusedDumpFile()
        void CleanAllDumpFiles()
        ##
        double GetTimeDumpStep()
        void SetTimeDumpStep(double time)
        bint GetDoDump()
        void SetDoDump(bint do_it)
        bint GetDoStartFromDump()
        void SetDoStartFromDump(bint do_it)

        int GetMaxPopulationSize()
        void SetMaxPopulationSize(int pop_size)


cdef class PyLabSimulatorBase:
    """
    Base class meant to be subclassed when writing a simulation.
    
    It provides a layout covering all the basic features required by a                  
    simulation. It is based on common hooks 
    (See C++ C{LabSimulatorBase} documentation, for an exhaustive list of the
    available hooks - C{InitSimulation()}, C{StepSimulation()}, ...)   
    which can be used (overridden) or not by the user. Most of those hooks are          
    virtual or empty (excepted the one concerning the main loop - have a look at        
    C{RunSimulation()} content to have a good preliminary overview on how the other        
    hooks might be used / organized).                                                     
                                                                                        
    B{Note :} The hooks concerning the basic implementation of the binarization            
    (aka : dump / saving the current state of the simulation) are also pre-written).                                                                    
                                                                                        
    The inner code of C{RunSimulation()} could look like this :                            
                                                                                  
    >>> # Default simulation loop behavior                                                 
    >>> def RunSimulation(self):                                              
    ...                                                                                    
    ...     done = False                                                                               
    ...     self.InitSimulation()                                                    
    ...     self.InitUnBinarize()
    ...     self.BeforeRun()                                                                     
    ...     while (not done):
    ...                                                                                     
    ...         self.OutputSimulation()                                                          
    ...         self.BeforeStep()                                                                
    ...         self.StepSimulation()                                                            
    ...         self.AfterStep()                                                               
    ...         self.StepBinarize()
    ...
    ...         if (self.EndSimulation()) done = True
    ...                                                                                     
    ...     self.OutputSimulation()                        # One more time for final state                   
    ...     self.AfterRun()                                                                      
    ...                                                                                    
    
    Assuming self is a custom simulator class inheriting from L{PyLabSimulatorBase} :
    
    >>> # Example of redefinition of the "InitSimulation()" hook
    >>> def InitSimulation(self):                                              
    ...     # Do custom initialization stuffs here...                                                                               
    ...     # This will override the default behavior.                                        
                                                                                
                                                                                        
                                                                                        
    COMMON USAGE : 
    ==============                                                                     
                                                                                        
        - Create a subclass of L{PyLabSimulatorBase}.                                             
        - Feel free to override the hooks (virtual) functions fitting your needs.            
                                                                                        
                                                                                        
    B{NOTE :} All the hooks that where not mentioned above remain empty in this base class,       
    but what they where designed for is described below...
                                      
    """
    
    #cdef ALabSimulatorBase* thisptr
    cdef LabSimulatorBase* thisptr

    # For Epydoc only
    def __init__(self):
        """
        __init__(self)
        Default constructor.
        """ 

    def __cinit__(self):
       self.thisptr = new LabSimulatorBase(<PyObject*>self)
       #self.thisptr.CySetPyObj(<PyObject*>self)
       
    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr

    ###
    cpdef int GetNbRepetitions(self):
        """
        GetNbRepetitions(self)
        Get the number of trials for the whole process.
        
        (aka : number of times the simulation must be entirely completed
        until the break condition - See EndSimulation() - is reached)     
        @return: The number of times the simulation will be repeated
        @rtype: C{int}                              
        """
        return self.thisptr.GetNbRepetitions()    
    cpdef SetNbRepetitions(self, int nb_rep):
        """
        SetNbRepetitions(self, nb_rep)
        Set the number of trials for the whole process.        
                              
        If ignored, the simulation will run only once (as the default returning value
        of EndSimulation() is True, meaning that the inner while loop of the         
        RunSimulation() hook while break at the end of the first iteration).         
        @param nb_rep: The number of times the simulation will be repeated.
        @type nb_rep: C{int}             
        """
        self.thisptr.SetNbRepetitions(nb_rep)        
    cpdef int GetCurrentRepetition(self):
        """
        GetCurrentRepetition(self)
        Catch the index of the iteration currently being run.          
        @return: The index, starting at 0 (index of the first iteration).
        @rtype: C{int}
        """
        return self.thisptr.GetCurrentRepetition()    
    cpdef SetCurrentRepetition(self, int rep_num):
        """
        WARNING ! Should not be forced by hand during a run. This functions was designed to 
        allow recovering the last state of the simulation before interruption of the process
        by the user or a crash. See the "UNbinarization" features (C{UnBinarizeAll()}). 
               
        Consequently, it should only be used to restart a simulation at a specific point.   
        @param rep_num: The repetition number to restart from.      
        @param type: C{int}
        """                          
        self.thisptr.SetNbRepetitions(rep_num)
    
    ###
#    cpdef InitSimulation(self):
#        self.thisptr.InitSimulation()
#    cpdef StepSimulation(self):
#        self.thisptr.StepSimulation()
#    cpdef BeforeStep(self):
#        self.thisptr.BeforeStep()
#    cpdef AfterStep(self):
#        self.thisptr.AfterStep()
#    cpdef OutputSimulation(self):
#        self.thisptr.OutputSimulation()    
#    cpdef bint EndSimulation(self):
#        return self.thisptr.EndSimulation()
#   
#    
#    cpdef InitUnBinarize(self):
#        self.thisptr.InitUnBinarize()    
#    cpdef StepBinarize(self):
#        self.thisptr.StepBinarize()    
#   
#    
#    cpdef RunSimulation(self):
#        self.thisptr.RunSimulation()
#    cpdef BeforeRun(self):
#        self.thisptr.BeforeRun()
#    cpdef AfterRun(self):
#        self.thisptr.AfterRun()
    cpdef Run(self):
        """
        Run(self)
        WARNING ::
            DO NOT OVERRIDE THIS METHOD !
            Use it "AS IS" in Python mode...
        """
        self.thisptr.Run()
#    
#    cpdef BinarizeAll(self):
#        self.thisptr.BinarizeAll()
#    
#    cpdef UnBinarizeAll(self):
#        self.thisptr.UnBinarizeAll()
    
    ###
    cpdef SetBinaryStreams(self, Py_ofstream ofs, Py_ifstream ifs):
        """
        SetBinaryStreams(self, ofs, ifs)
        Possibility to specify the output and input streams to respectively dump and restore 
        using the binarization features.                                                                      
        @param ofs: The file output stream to write to.
        @type ofs: L{Py_ofstream}                                           
        @param ifs: The file input stream to read from.                                          
        @type ifs: L{Py_ifstream}                                           
        """
        self.thisptr.SetBinaryStreams(<ofstream *>ofs.thisptr, <ifstream *>ifs.thisptr)
    cpdef int GetLastDumpFileIdAndClean(self):
        """
        GetLastDumpFileIdAndClean(self)
        Read the penultimate binary dump file (to make sure the integrity of the file we are
        working with is preserved, we don't pick up the very last one, as it could have     
        been corrupted...).                                                                 
                                                                                            
        @return: The id of the dump from which the system's state is to be recovered.
        @rtype: C{int}                
        """
        return self.thisptr.GetLastDumpFileIdAndClean()
    cpdef CleanUnusedDumpFile(self):
        """
        CleanUnusedDumpFile(self)
        Remove the most ancient dump files during the stepping process (See StepBinarize()).
        """
        self.thisptr.CleanUnusedDumpFile()
    cpdef CleanAllDumpFiles(self):
        """
        CleanAllDumpFiles(self)
        Remove from disk all the previously generated dumps.
        """
        self.thisptr.CleanAllDumpFiles()

    cpdef double GetTimeDumpStep(self):
        """
        GetTimeDumpStep(self)
        Get the delta of time between each dump (when 'do_dump' is set to true).
        @return: A time interval expressed in seconds (See t_sys_cur).
        @rtype: C{float}                   
        """
        return self.thisptr.GetTimeDumpStep()
    cpdef SetTimeDumpStep(self, double time):
        """
        SetTimeDumpStep(self, time)
        Set the interval between each dump (when 'do_dump' is set to true).
        @param time: Interval of time expressed in seconds.
        @type time: C{double}                   
        """
        self.thisptr.SetTimeDumpStep(time)
    cpdef bint GetDoDump(self):
        """
        GetDoDump(self)
        Check if the dumping system has been activated.
        @return: True if it has.
        @rtype: C{bool}                        
        """
        return self.thisptr.GetDoDump()
    cpdef SetDoDump(self, bint do_it):
        """
        SetDoDump(self, do_it)
        Set if the dump of the system must be activated.
        @param do_it: Pass 'true' to activate.
        @type do_it: C{bool}            
        """
        self.thisptr.SetDoDump(do_it)
    cpdef bint GetDoStartFromDump(self):
        """
        GetDoStartFromDump(self)
        Check if the system must be recovered from dump files.
        @return: True if it must. 
        @rtype: C{bool}                             
        """
        return self.thisptr.GetDoStartFromDump()
    cpdef SetDoStartFromDump(self, bint do_it):
        """
        SetDoStartFromDump(self, do_it)
        Set if the system must be recovered from dump files.
        @param do_it: Pass 'true' to activate the recovery.  
        @type do_it: C{bool}            
        """
        self.thisptr.SetDoStartFromDump(do_it)

    cpdef int GetMaxPopulationSize(self):
        """
        GetMaxPopulationSize(self)
        Requires a preliminary call to SetMaxPopulationSize()   
        @return: Maximum number of SITES handled by the simulator.
        @rtype: C{int}     
        """
        return self.thisptr.GetMaxPopulationSize()
    cpdef SetMaxPopulationSize(self, int pop_size):
        """
        Sets the maximum number of sites / individuals to be handled by the simulation.
        @param pop_size: Should be at least one individual.
        @type pop_size: C{int}                                     
        """
        self.thisptr.SetMaxPopulationSize(pop_size)






