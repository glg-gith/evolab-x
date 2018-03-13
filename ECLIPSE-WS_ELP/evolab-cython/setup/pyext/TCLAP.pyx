from cpython.ref cimport PyObject
from libcpp.string cimport string
from libcpp.vector cimport vector


#cdef extern from "../inc/Arg.h" namespace "TCLAP" :
#    cdef cppclass Arg:
#        pass
#    
#
#cdef class PyArg:
#    cdef Arg* thisptr
#
#    def __cinit__(self):
#       self.thisptr = new Arg()
#       
#    def __dealloc__(self):
#       if self.thisptr:
#           del self.thisptr

        


cdef extern from "../inc/Arg.h" namespace "TCLAP" :
    cdef cppclass Arg:
        pass

cdef class PyArg:
    """
    A virtual base class that defines the essential data for all arguments.
    This class, or one of its existing children, must be subclassed to do
    anything.
    """
    
    cdef Arg* thisptr0
#
#    def __cinit__(self, string a, string b, string c, bint d, double e, string f):
##       self.thisptr = new AValueArg(a, b, c, d, e, f)
#        pass
#       
#    def __dealloc__(self):
##       if self.thisptr:
##           del self.thisptr
    pass

        
cdef extern from "adapter/AValueArg.h" namespace "elps" :
    cdef cppclass ADoubleArg:
        ADoubleArg( string flag, 
                  string name, 
                  string desc, 
                  bint req, 
                  double value,
                  string typeDesc)
        double getValue()
        
    cdef cppclass AIntArg:
        AIntArg( string flag, 
                  string name, 
                  string desc, 
                  bint req, 
                  int value,
                  string typeDesc)
        int getValue()
        
    cdef cppclass AStringArg:
        AStringArg( string flag, 
                  string name, 
                  string desc, 
                  bint req, 
                  string value,
                  string typeDesc)
        string getValue()
           

cdef class PyDoubleArg(PyArg):
    """
    Class to define and parse a double value from the command line.
    """

    cdef ADoubleArg* thisptr

    # For Epydoc only
    def __init__(self, flag, name, desc, req, value, typeDesc):
        """
        __init__(self, flag, name, desc, req, value, typeDesc)
        Constructor.
        @param flag: The one character flag that identifies this argument on the command line.
        @type flag: C{str}
        @param name: A one word name for the argument. Can be used as a long flag on the command line. 
        @type name: C{str}
        @param desc: A description of what the argument is for or does. 
        @type desc: C{str}
        @param req: Whether the argument is required on the command line. 
        @type req: C{bool}
        @param value: The default value assigned to this argument if it is not present on the command line. 
        @type value: C{float}
        @param typeDesc: A short, human readable description of the type that this object expects.  
        This is used in the generation of the USAGE statement. The goal is to be helpful to the end user
        of the program. 
        @type typeDesc: C{str}
        """ 

    def __cinit__(self, string flag, string name, string desc, bint req, double value, string typeDesc):
       self.thisptr = new ADoubleArg(flag, name, desc, req, value, typeDesc)
       self.thisptr0 = <Arg *>self.thisptr
       
    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr
           
    cpdef double getValue(self):
        """
        getValue(self)
        Grab the double value.
        @return: The value for this argument.
        @rtype: C{float}
        """
        return self.thisptr.getValue()

cdef class PyIntArg(PyArg):
    """
    Class to define and parse an integer value from the command line.
    """

    cdef AIntArg* thisptr

    # For Epydoc only
    def __init__(self, flag, name, desc, req, value, typeDesc):
        """
        __init__(self, flag, name, desc, req, value, typeDesc)
        Constructor.
        @param flag: The one character flag that identifies this argument on the command line.
        @type flag: C{str}
        @param name: A one word name for the argument. Can be used as a long flag on the command line. 
        @type name: C{str}
        @param desc: A description of what the argument is for or does. 
        @type desc: C{str}
        @param req: Whether the argument is required on the command line. 
        @type req: C{bool}
        @param value: The default value assigned to this argument if it is not present on the command line. 
        @type value: C{int}
        @param typeDesc: A short, human readable description of the type that this object expects.  
        This is used in the generation of the USAGE statement. The goal is to be helpful to the end user
        of the program. 
        @type typeDesc: C{str}
        """ 

    def __cinit__(self, string a, string b, string c, bint d, int e, string f):
       self.thisptr = new AIntArg(a, b, c, d, e, f)
       self.thisptr0 = <Arg *>self.thisptr
       
    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr
           
    cpdef int getValue(self):
        """
        getValue(self)
        Grab the integer value.
        @return: The value for this argument.
        @rtype: C{int}
        """
        return self.thisptr.getValue()

cdef class PyStringArg(PyArg):
    """
    Class to define and parse a string value from the command line.
    """

    cdef AStringArg* thisptr

    # For Epydoc only
    def __init__(self, flag, name, desc, req, value, typeDesc):
        """
        __init__(self, flag, name, desc, req, value, typeDesc)
        Constructor.
        @param flag: The one character flag that identifies this argument on the command line.
        @type flag: C{str}
        @param name: A one word name for the argument. Can be used as a long flag on the command line. 
        @type name: C{str}
        @param desc: A description of what the argument is for or does. 
        @type desc: C{str}
        @param req: Whether the argument is required on the command line. 
        @type req: C{bool}
        @param value: The default value assigned to this argument if it is not present on the command line. 
        @type value: C{str}
        @param typeDesc: A short, human readable description of the type that this object expects.  
        This is used in the generation of the USAGE statement. The goal is to be helpful to the end user
        of the program. 
        @type typeDesc: C{str}
        """ 

    def __cinit__(self, string a, string b, string c, bint d, string e, string f):
       self.thisptr = new AStringArg(a, b, c, d, e, f)
       self.thisptr0 = <Arg *>self.thisptr
       
    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr
           
    cpdef string getValue(self):
        """
        getValue(self)
        Grab the string value.
        @return: The value for this argument.
        @rtype: C{str}
        """
        return self.thisptr.getValue()
    
        
cdef extern from "../inc/SwitchArg.h" namespace "TCLAP" :
    cdef cppclass SwitchArg:
        SwitchArg(string flag, 
                  string name, 
                  string desc,
                  bint defn)
        bint getValue()


cdef class PySwitchArg(PyArg):
    """
    A simple switch argument. If the switch is set on the command line, then
    the getValue method will return the opposite of the default value for the
    switch.                                                                  
    """
    
    cdef SwitchArg* thisptr

    # For Epydoc only
    def __init__(self, flag, name, desc, defin):
        """
        __init__(self, flag, name, desc, defin)
        Constructor.
        @param flag: The one character flag that identifies this argument on the command line.
        @type flag: C{str}
        @param name: A one word name for the argument. Can be used as a long flag on the command line. 
        @type name: C{str}
        @param desc: A description of what the argument is for or does. 
        @type desc: C{str}
        @param defin: The default value for this Switch.
        @type defin: C{bool}
        """ 

    def __cinit__(self, string flag, string name, string desc, bint defin):
       self.thisptr = new SwitchArg(flag, name, desc, defin)
       self.thisptr0 = <Arg *>self.thisptr
       
    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr
           
    cpdef bint getValue(self):
        """
        getValue(self)
        Grab the boolean value.
        @return: The value for this argument.
        @rtype: C{bool}
        """
        return self.thisptr.getValue()
    
    



cdef extern from "../inc/CmdLine.h" namespace "TCLAP" :
    cdef cppclass CmdLine:
        CmdLine(string a, char b, string c, bint d)
        void parse(vector[string] args)
        void add(Arg* a)



cdef class PyCmdLine:
    """
    The base class that manages the command line definition and passes
    along the parsing to the appropriate Arg classes.
    
    USAGE :
    =======
    
    >>> # Create a command line handler
    >>> cmd = PyCmdLine("BdmLsim4-python simulation (using EVOLAB-X1 project)", ord(' '), "0.4", True)
    ...
    ... # Double argument
    >>> dr_arg = PyDoubleArg("d", "death-rate", "Constant individual death factor.", True, 0.01, "double")
    >>> cmd.add( dr_arg )
    ... 
    ... # Flag (Switch) argument
    >>> dd_switch = PySwitchArg("D", "do-dump", "Dump / backup point every 'dump-step' seconds.", False)
    >>> cmd.add( dd_switch )
    ... 
    ... # Let's parse            
    >>> cmd.parse(argv)
    ... 
    ... # Grab values
    >>> dr  = dr_arg.getValue()
    >>> dd  = dd_switch.getValue()
    ...  
    
    """
    
    cdef CmdLine* thisptr

    # For Epydoc only
    def __init__(self, message, delimiter, version, helpAndVersion):
        """
        __init__(self, message, delimiter, version, helpAndVersion)
        Constructor.
        @param message: The message to be used in the usage output.
        @type message: C{str}
        @param delimiter: The character that is used to separate the argument flag/name from the value.  Defaults to ' ' (space).
        @type delimiter: C{str}
        @param version: The version number to be used in the --version switch.
        @type version: C{str}
        @param helpAndVersion: Whether or not to create the Help and Version switches. Defaults to true.
        @type helpAndVersion: C{bool}
        """
        
    def __cinit__(self, string message, char delimiter, string version, bint helpAndVersion):
       self.thisptr = new CmdLine(message, delimiter, version, helpAndVersion)
       pass
       
    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr

    cpdef parse(self, vector[string] args):
        self.thisptr.parse(args)

    cpdef add(self, PyArg arg):
        """
        add(self, arg)
        Adds an argument to the list of arguments to be parsed.
        @param arg: Argument to be added.
        @type arg: L{PyArg}
        """
        self.thisptr.add(arg.thisptr0)



