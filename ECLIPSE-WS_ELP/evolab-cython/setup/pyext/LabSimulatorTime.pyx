from cpython.ref cimport PyObject

# cdef extern from "adapter/ALabSimulatorTime.h" namespace "elps" :
#     cdef cppclass ALabSimulatorTime:
cdef extern from "../inc/LabSimulatorTime.h" namespace "elps" :
    cdef cppclass LabSimulatorTime:
        #ALabSimulatorTime(PyObject *obj)
        LabSimulatorTime(PyObject *obj)
        #void Run()
        
        PyObject * CyGetPyObj()
        void CySetPyObj(PyObject *obj)
        # Virtual overridable
#         void ResetTime()
#         double TimeStep()
#         void FireEvent()
#         # Virtual overridden from Base
#         ## Re-impl
#         void StepSimulation()
#         bint EndSimulation()
#         void RunSimulation()
#         ## Direct use
#         void InitSimulation()
#         void BeforeStep()
#         void AfterStep()
#         void OutputSimulation()    
#         void InitUnBinarize()
#         void StepBinarize()    
#         void BeforeRun()
#         void AfterRun()
#         void BinarizeAll()
#         void UnBinarizeAll()

        # Others
        void UpdateEventsRate(double rate)
        void SetEndTime(double end_time)
        void SetOutputTimeStep(double out_time_step)
        double GetTime()
        int GetNbFiredEvents()
        void SetTime(double time)
        

cdef class PyLabSimulatorTime(PyLabSimulatorBase):
    """
    A basic B{TIME based} subclass of L{PyLabSimulatorBase}.
                                      
    By default the time evolves continuously as follows ::                          
        t += -log(p) / ter
        where "p" is a random float number [0.0, 1.0] and "ter" is the current total event rate.                                                                    
                                                                                             
    The way the time is computed / evolves can be customized by overriding the hook B{TimeStep()}
    (See C++ C{LabSimulatorTime::TimeStep()} for more details).
    
    This class comes with additional dedicated hooks to be overridden. 
    (See C++ C{ResetTime()}, C{TimeStep()}, C{FireEvent()}, ... for an exhaustive list).
    
    B{Note }:: 
        We will often use the term "event(s) rate(s)" in time based contexts,             
        since this class provides a canvas for firing events according to time during     
        the simulation. By default in PyLabSimulatorTime, the events are fired relatively   
        to their rate (or propensity to occur).                            
    
    Of course, the default event based behavior of this class can be customized by overriding
    the appropriate hooks.                                    
    
    SPECIFICITIES :
    ===============
    
    The default inner code of B{RunSimulation()} is an override of the L{PyLabSimulatorBase} 
    C{RunSimulation()} hook.
    
    This hook can be overridden, but by default it consists in a re-implementation         
    of C{LabSimulatorBase.RunSimulation()} for the need of a TIME based process.        
    
    The only differences with base version are :
    
        - We add a reset of the time properties (ResetTime()) each time we call a new run 
            (right after the InitSimulation() call)                                         
        - This method is no more responsible for managing the output, since               
            StepSimulation() is                                                             
    
    >>> # Default simulation's loop behavior (time based version)                                                
    >>> def RunSimulation(self):                                              
    ...                                                                                    
    ...     done = False                                                                               
    ...     self.InitSimulation() 
    ...     self.ResetTime();                              # -- ADDED --
    ...     self.InitUnBinarize()
    ...     self.BeforeRun()                                                                     
    ...     while (not done):
    ...                                                                                     
    ...         #self.OutputSimulation()                   # -- REMOVED --                                                          
    ...         self.BeforeStep()                          # !! 'UpdateEventsRate()' must be called in there !!                                       
    ...         self.StepSimulation()                                                            
    ...         self.AfterStep()                                                               
    ...         self.StepBinarize()
    ...
    ...         if (self.EndSimulation()) done = True
    ...                                                                                     
    ...     self.OutputSimulation()                        # One more time for final state                   
    ...     self.AfterRun()                                                                      
    ...                                                                                    
    
    B{Note :} Also be aware that the total events rate is theoretically in constant evolution 
    and by the way, has to be updated in the stepping process.  
    
    B{StepSimulation()} method, is specialized too, and is very important : this is where the
    decisions for firing new events are taken. Consequently, you can keep it "AS IS" but you
    will have to override the B{FireEvent()} hook, because this is where everything happen.
    
    See the C++ documentation regarding the default implementation of 
    B{LabSimluatorTime::StepSimulation()}, which behavior can be customized through
    B{LabSimluatorTime::TimeStep()}, L{SetEndTime()} and L{SetOutputTimeStep()}.
    
    Here is how StepSimulation() looks like :
    
    >>> # Default StepSimulation()'s behavior
    >>> def StepSimulation(self):                                                   
    ...     # If time for output                                                                    
    ...     if(self.t_nextout <= self.t_nextevent && self.t_nextout <= self.t_end):                                       
    ...                                                                                             
    ...         self.t = self.t_nextout                                                                       
    ...                                                                                           
    ...         self.OutputSimulation()                     # OutputSimulation() fired every "t_outstep"
    ...                                                     # Feel free to override it...                                      
    ...                                                                                           
    ...         self.t_nextout = self.t + self.t_outstep                                                           
    ...                                                                                             
    ...                                                                                           
    ...     # If time for firing event                                                              
    ...     if(self.t_nextevent <= self.t_nextout && self.t_nextout <= self.t_end)                                       
    ...                                                                                             
    ...         self.t = self.t_nextevent                                                                     
    ...                                                                                           
    ...         self.FireEvent()                            # FireEvent() fired every "TimeStep()"      
    ...                                                     # Feel free to override it...                                      
    ...         self.nb_fired_events += 1                                                                   
    ...                                                                                           
    ...         self.t_nextevent = self.t + self.TimeStep()                                                        
    ...                                                                                             
    ...                                                                                           
    ...     # End reached = force t                                                                 
    ...     if(self.t_nextout > self.t_end): self.t = self.t_end                                                         
    
    COMMON USAGE :
    ==============                                                                           
    
        - Create a subclass of L{PyLabSimulatorTime}                                                  
        - Feel free to override the hooks (virtual) functions fitting your needs                 
    
    """
    
    #cdef ALabSimulatorTime* thisptr2
    ###cdef LabSimulatorTime* thisptr2

    # For Epydoc only
    def __init__(self):
        """
        __init__(self)
        Default constructor.
        """ 

    def __cinit__(self):
#        #self.thisptr2 = new ALabSimulatorTime(<PyObject*>self)
#        self.thisptr2 = new LabSimulatorTime(<PyObject*>self)
#        #self.thisptr2.CySetPyObj(<PyObject*>self)
#        
#        # Redirect 'Base' inherited calls to 'Time'
#        # Note : only 'non-virtual' methods are concerned
#        self.thisptr = <LabSimulatorBase*>self.thisptr2
       
       if (self.thisptr):
           del self.thisptr
            
       self.thisptr = <LabSimulatorBase*>new LabSimulatorTime(<PyObject*>self)
       
       
    def __dealloc__(self):
        # Done by the parent '' class through 'self.thisptr'
        #if self.thisptr2:
        #    del self.thisptr2
        pass
           
    ###
#    cpdef Run(self):
#        self.thisptr2.Run()

    ###
#    cpdef ResetTime(self):
#        self.thisptr2.ResetTime()
#
#    cpdef double TimeStep(self):
#        return self.thisptr2.TimeStep()
#
#    cpdef FireEvent(self):
#        self.thisptr2.FireEvent()
#    
#    cpdef StepSimulation(self):
#        self.thisptr2.StepSimulation()
#    
#    cpdef bint EndSimulation(self):
#        return self.thisptr2.EndSimulation()
#    
#    cpdef RunSimulation(self):
#        self.thisptr2.RunSimulation()
    
    ###
    cpdef UpdateEventsRate(self, double rate):
        """
        UpdateEventsRate(self, rate)
        Update the total events rate (value used to compute the "time to next event" - See C{TimeStep()}).
        @param rate: The new total events rate.
        @type rate: C{float} 
        """
        (<LabSimulatorTime*>self.thisptr).UpdateEventsRate(rate)
    
    cpdef SetEndTime(self, double end_time):
        """
        SetEndTime(self, end_time)
        Set the maximum time for the simulation (better use a value related to the way chosen to      
        compute time - See TimeStep()).                                                               
        @param end_time: The final time expressed relatively to the TimeStep() method's inner algorithm.
        @type end_time: C{float} 
        """
        (<LabSimulatorTime*>self.thisptr).SetEndTime(end_time)
    
    cpdef SetOutputTimeStep(self, double out_time_step):
        """
        SetOutputTimeStep(self, out_time_step)
        Set step of time for writing the output (better use a value related to the way chosen to          
        compute time - See TimeStep()).                                                                   
        @param out_time_step: The time step expressed relatively to the TimeStep() method's inner algorithm.
        @type out_time_step: C{float}  
        """
        (<LabSimulatorTime*>self.thisptr).SetOutputTimeStep(out_time_step)
    
    cpdef double GetTime(self):
        """
        GetTime(self)
        Get the current time of this simulation.                                                         
        @return: The current time expressed relatively to the TimeStep() method's inner algorithm.
        @rtype: C{float}         
        """
        return (<LabSimulatorTime*>self.thisptr).GetTime()
    
    cpdef SetTime(self, double time):
        """
        SetTime(self, time)
        Force the current time (useful when starting the simulation from a given time instead of from         
        the beginning - when recovering from an interruption, by example).                                    
        @param time: The time at which to start expressed relatively to the TimeStep() method's inner algorithm.
        @type time: C{float}
        """
        (<LabSimulatorTime*>self.thisptr).SetTime(time)

    cpdef int GetNbFiredEvents(self):
        """
        GetNbFiredEvents(self)
        Get the number of events having been fired since the simulation has begun.
        @return: A number of events.
        @rtype: C{int}                                                        
        """
        return (<LabSimulatorTime*>self.thisptr).GetNbFiredEvents()
    



cdef class PyLabSimulatorTimeNet(PyLabSimulatorTime): #, PyLabNetBinding):
    """
    Class designed to create SPACE and TIME based simulation.
    
    It's got all the features of a L{PyLabSimulatorTime} simulation              
    but additionally aggregates a network (See L{PyLabNetwork}) via              
    a L{PyLabNetBinding}.                                          
                                                                            
    COMMON USAGE : 
    ==============                                                         
                                                                            
        - Create a subclass of LabSimulatorTimeNet                              
        - Feel free to override the hooks (virtual) functions fitting your needs
                                                                            
    """

    # Net specifics
    # Here we aggregate since multiple inheritance is crap in Python
    cdef PyLabNetBinding _netBinding

    # For Epydoc only
    def __init__(self):
        """
        __init__(self)
        Default constructor.
        """ 
    
    def __cinit__(self):
        self._netBinding = PyLabNetBinding(self)

    def __dealloc__(self):
#        if self._netBinding:
#            del(self._netBinding)
        pass # Look, let the garbage collector do his job !

    
    def GetNetBinding(self):
        """
        L{netBinding} property accessor - See L{PyLabNetBinding} for details.
        @rtype: L{PyLabNetBinding}
        """
        return self._netBinding
    
    def SetNetBinding(self, value):
        """
        SetNetBinding(self, value)
        Setter for the B{netBinding} property.
        
        WARNING ::
            The netBinding property is read-only !
        """
        #self._netBinding = value
        print "WARNING : The netBinding property is read-only !"
    
    netBinding = property(GetNetBinding, SetNetBinding)
    #"""See L{PyLabNetBinding} for details."""


cdef class PyLabSimulatorTimeArray(PyLabSimulatorTime): #, PyLabArrayBinding):
    """
    Class designed to create MASS ACTION and TIME based simulation.
    
    It's got all the features of a L{PyLabSimulatorTime} simulation              
    but additionally aggregates a list of Sites via a L{PyLabArrayBinding}.                                            
                                                                            
    COMMON USAGE :    
    ==============                                                      
                                                                            
        - Create a subclass of LabSimulatorTimeArray                            
        - Feel free to override the hooks (virtual) functions fitting your needs
    
    """
    
    # Array specifics
    # Here we aggregate since multiple inheritance is crap in Python
    cdef PyLabArrayBinding _arrayBinding

    # For Epydoc only
    def __init__(self):
        """
        __init__(self)
        Default constructor.
        """ 
    
    def __cinit__(self):
        self._arrayBinding = PyLabArrayBinding(self)

    def __dealloc__(self):
#        if self._arrayBinding:
#            del(self._arrayBinding)
        pass # Look, let the garbage collector do his job !

    
    def GetArrayBinding(self):
        """
        L{arrayBinding} property accessor - See L{PyLabArrayBinding} for details.
        @rtype: L{PyLabArrayBinding}
        """
        return self._arrayBinding
    
    def SetArrayBinding(self, value):
        """
        SetArrayBinding(self, value)
        Setter for the B{arrayBinding} property.
        
        WARNING ::
            The arrayBinding property is read-only !
        """
        #self._arrayBinding = value
        print "WARNING : The arrayBinding property is read-only !"
    
    arrayBinding = property(GetArrayBinding, SetArrayBinding)



