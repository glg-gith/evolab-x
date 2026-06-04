from cpython.ref cimport PyObject
from libcpp.string cimport string
from libcpp.vector cimport vector


cdef extern from "../inc/LabEventStack.h" namespace "elps" :
    cdef cppclass LabEventStack:

        void PushEvent(string name, double initial_rate,
                void *param,
                LabCallBack2 *rate_callback,
                LabCallBack2 *realize_callback)
        void PopEvent()

    
        void Clear()
        int GetSize()
        double GetEventRate(int id)
        void SetEventRate(int id, double rate)
        string GetEventName(int id)
#        vector<t_event>& GetEvents()
    
        double GetStackRate()
        void ResetStackRate()
    
        void SetTime(double time)
        double GetTime()
    
        void UpdateStackRates()
        void RealizeEvent(int id, double time)            # Realize the given event
        void RealizeEvent(double p1, double time)         # Choose an event given a probability (Multinomial low), 
                                                          # then realize it
#        void RealizeEvent(gsl_rng *r, double time)
    
        void Copy(LabEventStack *src_stack)
    
        # Implement virtual
        void Binarize(ofstream *ostream)
        void UnBinarize(ifstream *istream)



cdef class PyLabEventStack:
    """
    Stack of events that can occur at a given time (depending on the rate of each event) 
    in a B{TIME based} simulation context.
    
    EVENTS :

    An event is an action that can be fired at any time during the simulation.
    
        - It has a given propensity to occur (which can change during the simulation life time)
        - It comes with an action (method) which is fired (executed) when the event occurs
    
    USAGE :
    =======
                                                                                        
        - Instantiate                                                                       
        - Push event(s) (PushEvent() function)                                              
        - RealizeEvent() is used to fire an event                                           
    	(the current time of the event stack is updated during this operation)         

            - Create a function of the form "double func_name(void* param)"                
    		for the update of the event's rate                                           
            - Create a function of the form "double func_name(void* param)"                
    		to be executed when the event is fired                                       
                                                                                        
                                                                                        
        - The events rates MUST then be updated using the UpdateStackRates() function       
                                                                                        
    B{Sample :}              
    
    First of all, get yourself an event stack :
    
    >>> eventStack = PyLabEventStack()
    
    Secondly, define the method to be fired. This has to be done in two steps :
        - The B{rate} callback
        - The B{realize} callback
    
    >>> def MyCallbackRate(param):
    ...     print "CB fired !"
    ...
    ...                                                 # DO SOMETHING with "param" or not...
    ... 
    ...     rate = 12.0                                 # Recompute the rate (constant in this ex.)
    ...     return rate
    ...
    >>> def MyCallbackRealize(param):
    ...     print "CB fired !"
    ...     # DO SOMETHING with node here... 
    ...     return 0.0

    Note : the B{param} parameter can be any Python object.
       
    Then we can create the callbacks and push them to the stack.
        
    >>> # EVENT
    ...
    ... # Push the event to be fired given an initial rate of 0.0.
    >>> rate_callback = PyLabCallBack(MyCallbackRate)
    >>> realize_callback = PyLabCallBack(MyCallbackRealize)
    >>> eventStack.PushEvent("AN_EVENT", 0.0, a_param, rate_callback, realize_callback)
    ...
    
    Push one callback per possible event according to the needs of your simulation
    for the given stack.

    """

    cdef LabEventStack* thisptr
    # Handling correct reference counting on the objects (Python specific)
    cdef object rate_callbacks
    cdef object realize_callbacks


    # For Epydoc only
    def __init__(self):
        """
        __init__(self)
        Default constructor.
        """ 

    def __cinit__(self):
        self.thisptr = new LabEventStack()
        self.rate_callbacks = []
        self.realize_callbacks = []
       
    def __dealloc__(self):
        if self.thisptr:
            del self.thisptr
        

    cpdef PushEvent(self, string name, double initial_rate,
            object param,
            PyLabCallBack rate_callback,
            PyLabCallBack realize_callback):
        """
        PushEvent(self, name, initial_rate, param, rate_callback, realize_callback)
        Add a new event to the event stack of this Site.
        
        Note : Usually, the C{param} parameter would be a L{PyLabNode} in case we are in
        SPATIALIZED context. This can be any kind of Python object otherwise...

        @param name: Name given to the event (better choose a unique one).
        @type name: C{str}
        @param initial_rate: Rate at which the event can be fired at start.
        @type initial_rate: C{float} 
        @param param: Parameter which will be passed to both rate and realize callbacks when fired
                     Should be left NULL (None) when sites are built from models (See L{PyLabNetBinding} and
                     L{PyLabArrayBinding} classes).
        @type param: C{object}
        @param rate_callback: Function to be called when updating the event's rate.
        @type rate_callback: C{function}
        @param realize_callback: Function to be called when realizing / firing the event.
        @type realize_callback: C{function}
        
        """
        self.rate_callbacks.append(rate_callback)
        self.realize_callbacks.append(realize_callback)
        self.thisptr.PushEvent(name, initial_rate, 
                                <void *>param,
                                <LabCallBack2 *>rate_callback.thisptr,
                                <LabCallBack2 *>realize_callback.thisptr
                                )
        
    cpdef PopEvent(self):
        """
        PopEvent(self)
        Remove the event located on top of the events stack.
        """
        self.rate_callbacks.pop()
        self.realize_callbacks.pop()
        self.thisptr.PopEvent()
                                
    cpdef double GetStackRate(self):
        """
        GetStackRate(self)
        Access the total rate of the events stack.
        @return: The cumulative stack rate.
        @rtype: C{float}
        """
        return self.thisptr.GetStackRate()
    cpdef ResetStackRate(self):
        """
        ResetStackRate(self)
        Reset the total rate of the events stack.
        """
        self.thisptr.ResetStackRate()

    cpdef SetTime(self, double time):
        """
        SetTime(self, time)
        Update the current time of the events stack.
        @param time: The current time (usually the current simulation time).
        @type time: C{float}
        """
        self.thisptr.SetTime(time)
    cpdef double GetTime(self):
        """
        GetTime(self)
        Get the current time of the events stack.
        @return: The time expressed as a double value.
        @rtype: C{float}
        """
        return self.thisptr.GetTime()
        
    cpdef UpdateStackRates(self):
        """
        UpdateStackRates(self)
        Update the global stack rate accordingly to the rates of the events stacked .
        """
        self.thisptr.UpdateStackRates()

         
    cpdef RealizeEvent(self, double p1, double time):
        """
        RealizeEvent(self, p1, time)
        Choose an event given a probability (Multinomial low), then realize it.
        (See L{PyLabEventStack.RealizeEvent(p1, time)}).
        @param p1: The probability [0.0, 1.0] used for selecting the event.
        @type p1: C{float}
        @param time: The given time becomes the current time of the stack.
        @type time: C{float}
        """
        self.thisptr.RealizeEvent(p1, time)


    def Copy(self, PyLabEventStack src_stack):
        """
        Copy(self, src_stack)
        Deep copy of the events stack from a given source.
        @param src_stack: The source stack to copy from.
        @type src_stack: L{PyLabEventStack}
        """
        self.thisptr.Copy(<LabEventStack *>src_stack.thisptr)
        
#        # Update the parent node callback's parameter value
#        cdef int i
#        if (self.parent != None):
#            for i in range(self.thisptr3.GetNbEvents()):
#                self.thisptr3.SetCallBackParamAt(i, <void *>self.parent)
        

    cpdef Binarize(self, Py_ofstream ostream):
        self.thisptr.Binarize(<ofstream *>ostream.thisptr)
        
    cpdef UnBinarize(self, Py_ifstream istream):
        self.thisptr.UnBinarize(<ifstream *>istream.thisptr)
        






