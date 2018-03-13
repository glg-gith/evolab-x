from cpython.ref cimport PyObject
from libcpp.string cimport string
from libcpp.vector cimport vector

import sys

'''
MISC
'''
# Redefine 't_output' enum type (no other way to do ??)
cdef public enum t_deptype:
    #: Dependency level filter for 'INDIVIDUAL' attributes
    IND_DEP  = 1
    #: Dependency level filter for 'SITE' attributes
    SITE_DEP = 2
    #: Dependency level filter for 'ENVIRONMENT' attributes
    ENV_DEP  = 4


'''
CALLBACK IMPL
'''
ctypedef double (*Method)(void *param, void *user_data)

# 'LabCallBack2' :: Only for reference (libelp.so compatibility)
# Should never be instantiated...
cdef extern from "../inc/LabCallBack2.h" namespace "elps" :
    cdef cppclass LabCallBack2:
        LabCallBack2(Method method, void *user_data)
        double cy_execute(void *parameter)
        pass
        
cdef extern from "../inc/ALabCallBack.h" namespace "elps" :
    cdef cppclass ALabCallBack:
        ALabCallBack(Method method, void *user_data)
        double cy_execute(void *parameter)

    
# 'callback' :: The pattern/convert method to be used
cdef double callback(void *parameter, void *method):
    return (<object>method)(<object>parameter)

        
cdef class PyLabCallBack:
    """
    Utility class to deal with callbacks.
    
    This version of B{LabCallBack} obey to the following pattern ::
        double callback(void *parameter, void *method)
    That is to say that it returns a double and nothing else. Return 0.0
    if you do not need to return anything...
    
    USAGE :
    =======
    
    First of all define the callback methods to be fired :
    
    >>> def MyMethod(a_param):                                 
    ...     ret = a_param * 2.0                 # Do something with "a_param" (can be any Python object)
    ...     return ret                          # Return a float
           
    Then we can create the callback :
        
    >>> my_callback = PyLabCallBack(MyMethod)
    
    And call it later on :
    
    >>> print(my_callback.execute(25.0))        # Writes : 50.0
 
    """

    cdef LabCallBack2* thisptr
    cdef object func

    # For Epydoc only
    def __init__(self, method):
        """
        __init__(self, method)
        Constructor.
        @param method: The method to be executed.
        @type method: C{function} 
        """ 

    def __cinit__(self, method):
        # 'callback' :: The pattern/converter method to fire a Python 
        #               object method from C typed infos
        # 'method'   :: The effective method passed by the Python user 
       self.thisptr = new LabCallBack2(callback, <void*>method)
       self.func = method
       
    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr
        
    cpdef double execute(self, parameter):
        """
        execute(self, parameter)
        Execute the method.
        @param parameter: Parameter passed to the method (object of any kind).
        @type parameter: C{object}
        """
        # 'parameter' :: The parameter to be passed to the 'method'
        return self.thisptr.cy_execute(<void*>parameter)


'''
LabUserData
'''
# cdef extern from "../inc/LabUserData.h" namespace "elps" :
#     cdef cppclass LabUserData:
#         pass
    
# cdef extern from "adapter/ALabSite.h" namespace "elps" :
#     cdef cppclass ALabUserData:
cdef extern from "../inc/LabUserData.h" namespace "elps" :
    cdef cppclass LabUserData:
        LabUserData(PyObject *obj)
        LabUserData(PyObject *obj, PyObject *data)
        
        PyObject *CyGetPyObj()
        void CySetPyObj(PyObject *obj)
        PyObject *CyGetPyData()
        void CySetPyData(PyObject *data)

    
cdef class PyLabUserData:
    """
    This is the base class for all the classes used for describing a Site.
    A Site can be completely user defined (by using B{PyLabUserData}, a custom
    subclass of it, or one of the ready-made ones - See L{PyLabSiteBase}, ...).
    
    The related user specific information can be stored using L{PyLabUserData.SetData()}
    and / or by extending PyLabUserData class (adding members).
    
    """
    cdef LabUserData* thisptr

    # For Epydoc only
    def __init__(self, data = None, parent = None, embed_c = True):
        """
        __init__(self, data = None, parent = None, embed_c = True)
        Constructor.
        @param data: Optional additional data. Any Python object.
        @type data: C{object} 
        @param parent: Optional parent node.
        @type parent: L{PyLabNode} 
        @param embed_c: DO NOT CHANGE - Leave it to True !
        @type embed_c: C{bool}
        """ 
        
    def __cinit__(self, data = None, parent = None, bint embed_c = True):
        #print "INIT PyLabUserData"
        if (embed_c):
            if (data == None):
                self.thisptr = new LabUserData(<PyObject *>self)
            else:
                self.thisptr = new LabUserData(<PyObject *>self, <PyObject *>data)
       
    def __dealloc__(self):
        if self.thisptr:
            del self.thisptr


    def GetData(self):
        """
        GetData(self)
        Get the specific information stored by the user. Can be any Python object.
        @return: The stored Python object.
        @rtype: C{object}
        """
        return <object>self.thisptr.CyGetPyData()

    def SetData(self, object data):
        """
        SetData(self, data)
        Set the specific information the user is about to store. Can be any Python object.
        @param data: A custom user's class / object / raw type.
        @type data: C{object}  
        """
        self.thisptr.CySetPyData(<PyObject *>data)


'''
LabSiteBase
'''

# cdef extern from "../inc/LabSiteBase.h" namespace "elps" :
#     cdef cppclass LabSiteBase:
#         void *GetCyPyObj()
#         #void SetCyPyObj(void *)
#         pass

# cdef extern from "adapter/ALabSite.h" namespace "elps" :
#     cdef cppclass ALabSiteBase:
cdef extern from "../inc/LabSiteBase.h" namespace "elps" :
    cdef cppclass LabSiteBase:
        LabSiteBase(PyObject *, LabSimulatorBase *)
        LabSiteBase(PyObject *, LabSimulatorBase *, LabNode *)
        
        PyObject *CyGetPyObj()
        void CySetPyObj(PyObject *)
        
        # Direct access to LabAttributesStackS (ind + site) stuffs
        void PushAttribute(string name, t_deptype dep_type, double d_value, bint indexed, int nb_beans)
        void PopAttribute(t_deptype dep_type)
        
        int GetNbAttributes(t_deptype dep_filter)
        
#        vector<LabAttributesStack::t_attr> GetIndAttributes();
#        vector<LabAttributesStack::t_attr> GetSiteAttributes();
#        vector<LabAttributesStack::t_attr> GetEnvAttributes();
#    
        int GetAttrInt(t_deptype dep_type, int id)
        double GetAttrDouble(t_deptype dep_type, int id)
        
###        void SetAttrInt(t_deptype dep_type, int id, int value)
        void SetAttrDouble(t_deptype dep_type, int id, double value)
        
        
        
        # Direct access to LabStateStack stuffs
        int GetState()
        void SetState(int state, double time)
        void PushState(string name, int next_id, int timeout_id, double timeout)
        void PopState()
        void GotoNextState(double time)
        void TryDoTimeoutState(double time)

        int GetNbStates()
        
        double GetCurrentStateElapsedTime(double time)
        double GetCurrentStateRemainingTime(double time)


        # MISC
    
        void Copy(LabUserData *src_site, int dep_filter)
    
        # Implement virtual
        void Binarize(ofstream *ostream)
        void UnBinarize(ifstream *istream)

        string CyGetSiteModelType()
        void CySetSiteModelType(string model_type)


cdef class PyLabSiteBase(PyLabUserData):
    """
    Base class for handling user defined data on a given SITE.

        - In a MASS ACTION context, the sites can be stored in an array (See L{PyLabArrayBinding}).
        - In a SPATIALIZED context, the sites would be stored in the node of a network (See L{PyLabNetBinding}).

    This class is specialized in managing 2 stacks of ATTRIBUTES (See PyLabAttributesStack - TODO).
    
        - The first one stores the attributes related to the INDIVIDUAL
        currently residing on the SITE.
        - The second one stores the attributes related to the SITE
        itself (typically, if the individual moves or dies, those attributes would persist...).

    It's state (the state of its attributes at a given time) can be
    binarized / dumped by implementing the features inherited from the
    LabIBinarizable interface (See L{Binarize()}, L{UnBinarize()}).

    It manages also a stack of states. A SITE can switch from one state to another.

    STATES (See also : PyLabStateStack - TODO) :

    A state (like ATTRIBUTES) can change in time. The differences are :
    
        - It can have only integer values [0..N]
        - They can be linked to each other (by giving the id of the next state coming after the current one)
        - They can have a limited life time - optional timeout - before returning to an other specified state
        
    Note : A Site is B{always} related to a simulator.
    
    USAGE :
    =======
    
    A basic use case would look like follows :

    >>> a_site = PyLabSiteBase(sim)                    # 'sim' : the related simulator
    ...
    ... # ATTRIBUTES
    ... # Indexed attribute : here, 3 sets/groups are created ("pop0", "pop1", "pop2")
    ... # [In case the site was registered by the 'Sets Manager' (See LabSiteSetsManager, 
    ... #  PyLabSiteSetsCalculator)]
    >>> a_site.PushAttribute("pop", LabSiteBase::IND_DEP, True, 3);
    ...
    ... # Classical attributes (doubles)
    >>> POP = 0
    >>> X   = 1
    >>> M   = 2
    ...
    >>> a_site.PushAttribute("x", cyelp.IND_DEP)
    >>> a_site.PushAttribute("m", cyelp.IND_DEP)
    ...
    ... # STATES
    ... # We define 3 possible states :
    >>> EMPTY    = 0
    >>> RESIDENT = 1
    >>> MUTANT   = 2
    ...
    ... # Parameters : 1, -1, -1 => next_id = 1, timeout_id = -1, timeout = -1.0
    >>> a_site.PushState("EMPTY",    1, -1, -1)
    >>> a_site.PushState("RESIDENT", 1, -1, -1)
    >>> a_site.PushState("MUTANT",   1, -1, -1)
    ... 
    ... # Choose whether the current site is in EMPTY or RESIDENT state from the
    ... # probability 'p_init'.
    >>> if(rng.GetNextDouble() < p_init) :
    ... 
    ...     # This site will be a member of the "RESIDENT" set
    ...     a_site.SetState(RESIDENT, sim.GetTime())
    ... 
    ...     a_site.SetAttrDouble(cyelp.IND_DEP, POP, 1)
    ...     a_site.SetAttrDouble(cyelp.IND_DEP, X,   x1)
    ...     a_site.SetAttrDouble(cyelp.IND_DEP, M,   mo1)
    ... 
    ... else :
    ... 
    ...     # This site will be a member of the "EMPTY" set
    ...     a_site.SetState(BdmLsim3::EMPTY, sim.GetTime())
    ... 
    ...     a_site.SetAttrDouble(cyelp.IND_DEP, POP, 0)
    ...     a_site.SetAttrDouble(cyelp.IND_DEP, X,   0.0)
    ...     a_site.SetAttrDouble(cyelp.IND_DEP, M,   0.0);
    ... 
    
    NOTE : About B{Sets Management}
    
    As mentioned in the source code above, in case the site was registered by the B{Sets Manager},
    indexed attribute would result in the creation of related Sets / groups (ex. 3 groups ["pop0", "pop1", "pop2"]
    are created from the indexed attribute "pop"...) - See L{PyLabSiteSetsCalculator} for more details concerning 
    Sets.
    
    Concerning states, Sets management behaves the same : in the above, the groups ["EMPTY", "RESIDENT", "MUTANT"]
    would be created.

    """
    
    #cdef LabSiteBase* thisptr2

    # For Epydoc only
    def __init__(self, sim, parent = None, embed_c = True):
        """
        __init__(self, sim, parent = None, embed_c = True)
        Basic constructor (user must provide a parent node in a SPATIALIZED context).
        @param sim: The Simulator this Site is related to.
        @type sim: C{PyLabSimulatorBase} 
        @param parent: The node this Site is related to.
        @type parent: C{PyLabNode} 
        @param embed_c: DO NOT CHANGE - Leave it to True !
        @type embed_c: C{bool}
        """ 

    def __cinit__(self, PyLabSimulatorBase sim, PyLabNode parent = None, bint embed_c = True):
        #print "INIT PyLabSiteBase"
        
        if (self.thisptr):
            del self.thisptr
        #print "refcount of self = ", sys.getrefcount(self)

        if (embed_c):
#             if (parent == None):
#                 self.thisptr2 = new LabSiteBase(<PyObject *>self, sim.thisptr)
#             else:
#                 self.thisptr2 = new LabSiteBase(<PyObject *>self, sim.thisptr, parent.thisptr)
#             self.thisptr = <LabUserData*>self.thisptr2
            if (parent == None):
                self.thisptr = <LabUserData *>new LabSiteBase(<PyObject *>self, sim.thisptr)
            else:
                self.thisptr = <LabUserData *>new LabSiteBase(<PyObject *>self, sim.thisptr, parent.thisptr)
       
    def __dealloc__(self):
        # Done by the parent 'PyLabUserData' class through 'self.thisptr'
        #if self.thisptr:
        #    del self.thisptr
        pass

    cpdef PushAttribute(self, string name, t_deptype dep_type, double d_value = 0.0, bint indexed = False, int nb_beans = 1):
        """
        PushAttribute(self, name, dep_type, d_value = 0.0, indexed = False, nb_beans = 1)
        Add a new attribute on top of the stack specified by B{dep_type}.
        @param name: Name for this attribute (better use a unique name for each one).
        @type name: C{str}
        @param dep_type: Dependency type (cyelp.IND, cyelp.SITE or cyelp.ENV).
        @type dep_type: C{int}
        @param d_value: Value for the attribute. Pass an C{int} for indexed attributes.
        @type d_value: C{float} or C{int}
        @param indexed: If this attribute is an INDEXED one.
        @type indexed: C{bool}
        @param nb_beans: Number of distinct values the indexed attribute can take [0..nb_beans].
        @type nb_beans: C{int}
        """
        (<LabSiteBase *>self.thisptr).PushAttribute(name, dep_type, d_value, indexed, nb_beans)

    cpdef PopAttribute(self, t_deptype dep_type):
        """
        PopAttribute(self, dep_type)
        Remove the last pushed attribute from the top of the specified stack.
        @param dep_type: The dependency of the attribute to be popped.
        @type dep_type: C{int} 
         """
        (<LabSiteBase *>self.thisptr).PopAttribute(dep_type)

    cpdef int GetNbAttributes(self, t_deptype dep_filter):
        """
        GetNbAttributes(self, dep_filter)
        Get the number of attributes at the given dependency level using a filter.
        @param dep_filter: Filter can be any combination (ex. cyelp.IND | cyelp.SITE).
        @type dep_filter: C{int}
        @return A number.
        @rtype: C{int}
        """
        return (<LabSiteBase *>self.thisptr).GetNbAttributes(dep_filter)

    cpdef int GetAttrInt(self, t_deptype dep_type, int id):
        """
        GetAttrInt(self, dep_type, id)
        Get the integer value of an attribute specified by its id in the
        attributes stack and its dependency level.
        @param dep_type: Dependency level of the targeted attribute.
        @type dep_type: C{int}
        @param id: Identifier in the list relatively to the specified dependency level.
        @type id: C{int}
        @return: The integer value of the attribute.
        @rtype: C{int}
        """
        return (<LabSiteBase *>self.thisptr).GetAttrInt(dep_type, id)
        
    cpdef double GetAttrDouble(self, t_deptype dep_type, int id):
        """
        GetAttrDouble(self, dep_type, id)
        Get the integer value of an attribute specified by its id in the
        attributes stack and its dependency level.
        @param dep_type: Dependency level of the targeted attribute.
        @type dep_type: C{int}
        @param id: Identifier in the list relatively to the specified dependency level.
        @type id: C{int}
        @return: The value of the attribute as a double.
        @rtype: C{float}
        """
        return (<LabSiteBase *>self.thisptr).GetAttrDouble(dep_type, id)

###    cpdef SetAttrInt(self, t_deptype dep_type, int id, int value):
###        self.thisptr2.SetAttrInt(dep_type, id, value)
        
    cpdef SetAttrDouble(self, t_deptype dep_type, int id, double value):
        """
        SetAttrDouble(self, dep_type, id, value)
        Set the double value of an attribute specified by its id in the
        attributes stack and its dependency level.
        @param dep_type: Dependency level of the targeted attribute.
        @type dep_type: C{int}
        @param id: Identifier in the list relatively to the specified dependency level.
        @type id: C{int}
        @param value: The new double value of the attribute.
        @type value: C{float}
        """
        (<LabSiteBase *>self.thisptr).SetAttrDouble(dep_type, id, value)



    cpdef int GetState(self):
        """
        GetState(self)
        Get the current state of this SITE.
        @return: A state expressed as an index in the states stack.
        @rtype: C{int}
        """
        return (<LabSiteBase *>self.thisptr).GetState()
        
    cpdef SetState(self, int state, double time):
        """
        SetState(self, state, time)
        Set the current state of this SITE.
        @param state: New state (new index in the states stack).
        @type state: C{int}
        @param time: Time at which this change is performed (usually the current simulation time).
                     See L{PyLabSimulatorTime.GetTime()}.
        @type time: C{float} 
        """
        (<LabSiteBase *>self.thisptr).SetState(state, time)
        
    cpdef PushState(self, string name, int next_id = -1, int timeout_id = -1, double timeout = -1.0):
        """
        PushState(self, name, next_id = -1, timeout_id = -1, timeout = -1.0)
        Add a new state to the state stack.
        (By default : no next logical state and no timeout).
        @param name: Name of the new state
        @type name: C{str}
        @param next_id: Next logical state to this state (if any)
        @type next_id: C{int}
        @param timeout_id: Index of the state to switch to when timeout is reached (if any timeout)
        @type timeout_id: C{int}
        @param timeout: Delta of time before switching to the "timeout_id" state (if any timeout)
        @type timeout: C{float}
        """
        (<LabSiteBase *>self.thisptr).PushState(name, next_id, timeout_id, timeout)
        
    cpdef PopState(self):
        """
        PopState(self)
        Remove the state located on top of the states stack.
        """
        (<LabSiteBase *>self.thisptr).PopState()

    cpdef int GetNbStates(self):
        """
        GetNbStates(self)
        Get the number of states in the states stack.
        @return: Number of states currently stored.
        @rtype: C{int}
        """
        return (<LabSiteBase *>self.thisptr).GetNbStates()

    cpdef double GetCurrentStateElapsedTime(self, double time):
        """
        GetCurrentStateElapsedTime(self, time)
        Get the time elapsed since the current state was entered.
        @param time: The reference time (usually the current time of the simulation).
        @type time: C{float} 
        @return: The time elapsed.
        @rtype: C{float} 
        """
        return (<LabSiteBase *>self.thisptr).GetCurrentStateElapsedTime(time)
    cpdef double GetCurrentStateRemainingTime(self, double time):
        """
        GetCurrentStateRemainingTime(self, time)
        Get the time remaining until the current state will be quit (timeout reached).
        @param time: The reference time (usually the current time of the simulation).
        @type time: C{float} 
        @return: The time remaining.
        @rtype: C{float}
        """
        return (<LabSiteBase *>self.thisptr).GetCurrentStateRemainingTime(time)


    def Copy(self, PyLabSiteBase src_site, int dep_filter):
        """
        Copy(self, src_site, dep_filter)
        Make a copy of the whole attributes stack from another SITE.
        @param src_site: The site (L{PyLabSiteBase}) to copy from.
        @type src_site: L{PyLabSiteBase}
        @param dep_filter: Filter used to select which attributes will be copied.
                        (Ex1 : dep_filter = IND_DEP | SITE_DEP, means : copies only non-environmental attributes.
                        Ex2 : dep_filter = IND_DEP | SITE_DEP | ENV_DEP : copies all).
        @type dep_filter: C{int}
        """
        (<LabSiteBase *>self.thisptr).Copy(<LabUserData *>src_site.thisptr, dep_filter)

    
    cpdef Binarize(self, Py_ofstream ofs):
        (<LabSiteBase *>self.thisptr).Binarize(<ofstream *>ofs.thisptr)
    
    cpdef UnBinarize(self, Py_ifstream ifs):
        (<LabSiteBase *>self.thisptr).UnBinarize(<ifstream *>ifs.thisptr)

    cdef string CyGetSiteModelType(self):
        return (<LabSiteBase *>self.thisptr).CyGetSiteModelType()
    cdef CySetSiteModelType(self, string model_type):
        (<LabSiteBase *>self.thisptr).CySetSiteModelType(model_type)

        
#     cdef inline set_c_pointers(self, LabSiteBase *a_lab_site):
#         # Update C pointers
#         if (type(self) == PyLabSiteEvent):
#             (<PyLabSiteEvent>self).thisptr3 = <LabSiteEvent *>a_lab_site
#         self.thisptr2 = <LabSiteBase *>a_lab_site
#         self.thisptr = <LabUserData *>a_lab_site


'''
LabSiteEvent
'''
# cdef extern from "adapter/ALabSite.h" namespace "elps" :
#     cdef cppclass ALabSiteEvent:
cdef extern from "../inc/LabSiteEvent.h" namespace "elps" :
    cdef cppclass LabSiteEvent:
        LabSiteEvent(PyObject *, LabSimulatorBase *)
        LabSiteEvent(PyObject *, LabSimulatorBase *, LabNode *)
        
        PyObject *CyGetPyObj()
        void CySetPyObj(PyObject *)
        
        # Direct access to LabEventStack stuffs
    
        void PushEvent(string name, double initial_rate,
                void *param,
                LabCallBack2 *rate_callback,
                LabCallBack2 *realize_callback)
        void PopEvent()

        int GetNbEvents()

        double GetEventStackRate()
        void ResetEventStackRate()
        
        void SetTimeEvent(double time)
        double GetTimeEvent()
        void UpdateEventStackRates()
        void RealizeEvent(int id, double time)            # Realize the given event
        void RealizeEvent(double p1, double time)         # Choose an event given a probability (Multinomial low), 
                                                          # then realize it


        # MISC

        void Copy(LabUserData *src_site, int dep_filter)

        # Implement virtual
        void Binarize(ofstream *ostream)
        void UnBinarize(ifstream *istream)

    
        # DON'T USE ME ! [Cython usage only]
        void SetCallBackParamAt(int i, void *p)



cdef class PyLabSiteEvent(PyLabSiteBase):
    """
    This class handles ATTRIBUTES and STATES (since it inherits from L{PyLabSiteBase}), but also EVENTS.
    
    B{Note :} The concept of SITE (data of a node in a network - SPATIALIZED context) has been kept 
    for MASS ACTION context for preserving genericity.
    
    All the features related to ATTRIBUTES and STATES are deeply described in L{PyLabSiteBase}, since
    PyLabSiteEvent inherits from this class.
    
    EVENTS (See also : L{PyLabEventStack}) :

    An event is an action that can be fired at any time on the SITE (locally).
    
        - It has a given propensity to occur (which can change during the simulation life time)
        - It comes with an action (method) which is fired (executed) when the event occurs
    
    The PyLabSiteEvent class is nothing else but an aggregate of the attributes, states and events stacks :
    See C++ classes C{LabAttributesStack}, C{LabStateStack} and C{LabEventStack}.
    
    B{Note :} A Site is B{always} related to a simulator.
    
    USAGE :
    =======
    
    A basic use case would look like follows (assuming the setup for ATTRIBUTES and STATES has already
    been done - See L{PyLabSiteBase}) :
    
    First of all define the callback methods to be fired. Those must be of the form :
    
    >>> def BirthRate(node):                                    # We choose to pass a PyLabNode
    ...     a_site = node.GetUserData()                         # Get the Site embedded in this node 
    ...     rate = b * a_site.GetAttrDouble(cyelp.IND_DEP, X)   # Compute the return value
    ...     if (a_site.GetState() == MUTANT):
    ...         rate *= 0.9
    ...     return rate                                         # return a float
    
    >>> def RealizeBirth(node):
    ...     print "BIRTH fired !"
    ...     # DO SOMETHING with node here... 
    ...     return 0.0

    B{Note :} the B{node} parameter can be any Python object (here we assume we are in a SPATIALIZED
    context, that's why we use the "node" term).
       
    Then we can create the callbacks and assign them to the Site :
        
    >>> # EVENTS
    ...
    ... # Push the event to be fired when a "birth" should occur.
    >>> rate_callback = PyLabCallBack(BirthRate)
    >>> realize_callback = PyLabCallBack(RealizeBirth)
    >>> a_site.PushEvent("BIRTH", 0, a_node, rate_callback, realize_callback);
    ...
    ... # Push the event to be fired when a "death" should occur.
    >>> rate_callback = PyLabCallBack(DeathRate)
    >>> realize_callback = PyLabCallBack(RealizeDeath)
    >>> a_site.PushEvent("DEATH", 0, a_node, rate_callback, realize_callback);
    ...
    ... # etc.
    
    Push one callback per possible event according to the needs of your simulation
    for the given Site.

    """
     
    #cdef LabSiteEvent* thisptr3
    
    # Handling correct reference counting on the objects (Python specific)
    cdef object rate_callbacks
    cdef object realize_callbacks
    cdef PyLabNode parent

    # For Epydoc only
    def __init__(self, sim, parent = None, embed_c = True):
        """
        __init__(self, sim, parent = None, embed_c = True)
        Basic constructor (user must provide a parent node in a SPATIALIZED context).
        @param sim: The Simulator this Site is related to.
        @type sim: C{PyLabSimulatorBase} 
        @param parent: The node this Site is related to.
        @type parent: C{PyLabNode} 
        @param embed_c: DO NOT CHANGE - Leave it to True !
        @type embed_c: C{bool}
        """ 

    def __cinit__(self, PyLabSimulatorBase sim, PyLabNode parent_node = None, bint embed_c = True):
        #print "INIT PyLabSiteEvent"
        
        if (self.thisptr):
            del self.thisptr
        #print "refcount of self = ", sys.getrefcount(self)
        
        if (embed_c):
#             if (parent_node == None):
#                 self.thisptr3 = new LabSiteEvent(<PyObject *>self, sim.thisptr)
#             else:
#                 self.thisptr3 = new LabSiteEvent(<PyObject *>self, sim.thisptr, parent_node.thisptr)
#             self.thisptr2 = <LabSiteBase*>self.thisptr3
#             self.thisptr = <LabUserData*>self.thisptr3
            if (parent_node == None):
                self.thisptr = <LabUserData *>new LabSiteEvent(<PyObject *>self, sim.thisptr)
            else:
                self.thisptr = <LabUserData *>new LabSiteEvent(<PyObject *>self, sim.thisptr, parent_node.thisptr)
                
                
        self.rate_callbacks = []
        self.realize_callbacks = []
        self.parent = parent_node
       
    def __dealloc__(self):
        # Done by the parent 'PyLabUserData' class through 'self.thisptr'
        #if self.thisptr:
        #    del self.thisptr
        pass
        

    cpdef PushEvent(self, string name, double initial_rate,
            object param,
            PyLabCallBack rate_callback,
            PyLabCallBack realize_callback):
        """
        PushEvent(self, name, initial_rate, param, rate_callback, realize_callback)
        Add a new event to the event stack of this Site.
        
        Note : Usually, the B{param} parameter would be a L{PyLabNode} in case we are in
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
        (<LabSiteEvent *>self.thisptr).PushEvent(name, initial_rate, 
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
        (<LabSiteEvent *>self.thisptr).PopEvent()

    cpdef int GetNbEvents(self):
        """
        GetNbEvents(self)
        Get the number of events in the events stack.
        @return: Number of events currently stored.
        @rtype: C{int}
        """
        return (<LabSiteEvent *>self.thisptr).GetNbEvents()
                                
    cpdef double GetEventStackRate(self):
        """
        GetEventStackRate(self)
        Access the total rate of the events stack.
        @return: The cumulative stack rate.
        @rtype: C{float}
        """
        return (<LabSiteEvent *>self.thisptr).GetEventStackRate()
    cpdef ResetEventStackRate(self):
        """
        ResetEventStackRate(self)
        Reset the total rate of the events stack.
        """
        (<LabSiteEvent *>self.thisptr).ResetEventStackRate()

    cpdef SetTimeEvent(self, double time):
        """
        SetTimeEvent(self, time)
        Update the current time of the events stack.
        @param time: The current time (usually the current simulation time).
        @type time: C{float}
        """
        (<LabSiteEvent *>self.thisptr).SetTimeEvent(time)
    cpdef double GetTimeEvent(self):
        """
        GetTimeEvent(self)
        Get the current time of the events stack.
        @return: The time expressed as a double value.
        @rtype: C{float}
        """
        return (<LabSiteEvent *>self.thisptr).GetTimeEvent()
        
    cpdef UpdateEventStackRates(self):
        """
        UpdateEventStackRates(self)
        Update the global stack rate accordingly to the rates of the events stacked 
        (See L{PyLabEventStack.UpdateStackRates()}).
        """
        (<LabSiteEvent *>self.thisptr).UpdateEventStackRates()

         
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
        #self.UpdateEventStackRates()
        (<LabSiteEvent *>self.thisptr).RealizeEvent(p1, time)


#    cdef Copy3_4_C(self, ALabSiteEvent *src_site, int dep_filter):
#        self.thisptr3.Copy(<LabUserData *>src_site, dep_filter)

    def Copy(self, PyLabSiteEvent src_site, int dep_filter):
        """
        Copy of the 3 stacks (attributes, states, events) given a source SITE to this one.
        
        The dependency level (B{dep_filter}) concerns only the attributes stack
        (See C++ LabAttributesStack::Copy()).
        """
        (<LabSiteEvent *>self.thisptr).Copy(src_site.thisptr, dep_filter)
        
        # Update the parent node callback's parameter value
        cdef int i
        if (self.parent != None):
            for i in range((<LabSiteEvent *>self.thisptr).GetNbEvents()):
                (<LabSiteEvent *>self.thisptr).SetCallBackParamAt(i, <void *>self.parent)
        

    cpdef Binarize(self, Py_ofstream ostream):
        (<LabSiteEvent *>self.thisptr).Binarize(<ofstream *>ostream.thisptr)
        
    cpdef UnBinarize(self, Py_ifstream istream):
        (<LabSiteEvent *>self.thisptr).UnBinarize(<ifstream *>istream.thisptr)
        

        
        

