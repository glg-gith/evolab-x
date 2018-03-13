from libcpp.vector cimport vector

'''
CY CALLBACK PROTOTYPES
'''

cdef public api void cy_call_func_void(object self, char* method, bint *error):
    if (hasattr(self, method)):
        error[0] = 0
        getattr(self, method)();
    else:
        error[0] = 1

cdef public api bint cy_call_func_bool(object self, char* method, bint *error):
    if (hasattr(self, method)):
        error[0] = 0
        return getattr(self, method)();
    else:
        error[0] = 1

cdef public api int cy_call_func_int(object self, char* method, bint *error):
    if (hasattr(self, method)):
        error[0] = 0
        return getattr(self, method)();
    else:
        error[0] = 1

cdef public api double cy_call_func_double(object self, char* method, bint *error):
    if (hasattr(self, method)):
        error[0] = 0
        return getattr(self, method)();
    else:
        error[0] = 1
        
'''
RNG from GSL
'''

cdef extern from "../inc/LabRngUtils.h" namespace "elps" :
    cdef cppclass LabRng:
        LabRng(int ralgo, int rseed)
        # Rng
        double GetNextDouble()
        unsigned int GetNextInt(int max)
        unsigned int GetNextIntMultinomial(double max_cumsum, vector[double]& p_nomes)
        # RanDist
        unsigned int GetNextIntPoisson(double mu)

cdef class PyLabRng:
    """
    This class is a simplified access to B{Random Number Generation}.
    
    Mainly (but not only) a subset of features already available in GSL :
    
    U{http://www.gnu.org/software/gsl/manual/gsl-ref.html}
                               
    """
    cdef LabRng *thisptr

    # For Epydoc only
    def __init__(self, ralgo = 0, rseed = -1):
        """
        __init__(self, ralgo = 0, rseed = -1)
        Initializes a RNG specifying an algorithm (default is 0 : Mersenne Twister).             
        @param ralgo: Can be 0, 1 or 2 (respectively : mt19937 - the default, taus, gfsr4).
        @type ralgo: C{int}       
        @param rseed: Seed used to generate the numbers (default is -1 : clock based).             
        Series of random numbers are different each time the program is run, based on the 
        computer's time clock.
        @type rseed: C{int}       
        """ 

    def __cinit__(self, int ralgo = 0, int rseed = -1):
       self.thisptr = new LabRng(ralgo, rseed)

    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr
    
    cpdef double GetNextDouble(self):
        """
        GetNextDouble(self)
        GSL : Get a random double in range [0.0, 1.0].
        @return: A random double between 0.0 and 1.0 included.
        @rtype: C{float}          
        """
        return self.thisptr.GetNextDouble()
    
    cpdef int GetNextInt(self, int max):
        """
        GetNextInt(self, max)
        GSL : Get a random in in range [0, max-1].   
        @param max: The maximum returned plus 1.
        @type max: C{int}      
        @return: A random integer between 0 and max-1 included.
        @rtype: C{int}
        """
        return self.thisptr.GetNextInt(max)

    cpdef int GetNextIntMultinomial(self, double max_cumsum, object p_nomes):
        """
        GetNextIntMultinomial(self, max_cumsum, p_nomes)
        CUSTOM : Returns the index of a nome in the nomes list according to its probability  
        to be chosen. (assuming max_cumsum is the cumulative sum of all the nomes).                                                               
        @param max_cumsum: The cumulative sum of all the elements of vector "p_nomes". 
        @type max_cumsum: C{float}
        @param p_nomes: The vector of double containing the probabilities [0.0, 1.0] for each nome.
        @type p_nomes: C{list}
        @return: The index of the chosen nome in the vector "p_nomes".                 
        @rtype: C{int}
        """
        return self.thisptr.GetNextIntMultinomial(max_cumsum, p_nomes)
        #return 37812

    cpdef int GetNextIntPoisson(self, double mu):
        """
        GetNextIntPoisson(self, mu)
        GSL : This function returns a random integer from the Poisson distribution with mean mu.
        @param mu: Mean of the distribution.
        @type mu: C{float}                                                      
        @return: A random integer.
        @rtype: C{int}                                                                
        """
        return self.thisptr.GetNextIntPoisson(mu)
    

'''
TIME
'''

cdef extern from "../inc/LabTimeUtils.h" namespace "elps" :
    cdef cppclass LabTimer:
        LabTimer()
        void Start()
        void Stop()
        void Print()

cdef class PyLabTimer:
    """
    Utility class for dealing with time (benchmarking).
    """
    cdef LabTimer *timer

    # For Epydoc only
    def __init__(self):
        """
        __init__(self)
        Default constructor. 
        """ 

    def __cinit__(self):
       self.timer = new LabTimer()

    def __dealloc__(self):
       if self.timer:
           del self.timer
    
    cpdef Start(self):
        """
        Start(self)
        Start timer or reset if already started.
        """
        self.timer.Start()
    
    cpdef Stop(self):
        """
        Stop(self)
        Stop timer.
        """
        self.timer.Stop()
    
    cpdef Print(self):
        """
        Print(self)
        Print the elapsed time between L{Start()} and L{Stop()} to the standard output.
        """
        self.timer.Print()


'''
FILE STREAMS
'''

cdef extern from "<fstream>" namespace "std" :
    cdef cppclass ofstream:
        pass
    
cdef class Py_ofstream:
    """
    C++ / STL B{std::ofstream} exposed to Python.
    """
    
    cdef ofstream *thisptr

    # For Epydoc only
    def __init__(self):
        """
        __init__(self)
        Default constructor. 
        """ 

    def __cinit__(self):
       self.thisptr = new ofstream()

    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr
#    pass
 

cdef extern from "<fstream>" namespace "std" :
    cdef cppclass ifstream:
        pass

    
cdef class Py_ifstream:
    """
    C++ / STL B{std::ifstream} exposed to Python.
    """

    cdef ifstream *thisptr

    # For Epydoc only
    def __init__(self):
        """
        __init__(self)
        Default constructor. 
        """ 

    def __cinit__(self):
       self.thisptr = new ifstream()

    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr
#       pass




cdef extern from "../inc/LabBinaryUtils.h" namespace "elps" :
    cdef cppclass LabBinaryUtils:
        void WriteValueToStream(int val, ofstream *ostream)
        void WriteValueToStream(double val, ofstream *ostream)
        int ReadIntFromStream(ifstream *istream)
        double ReadDoubleFromStream(ifstream *istream)
 

    
cdef class PyLabBinaryUtils:
    """
    Utility class for writing and reading information stored in binary files.
    """
    cdef LabBinaryUtils *thisptr

    def __cinit__(self):
       self.thisptr = new LabBinaryUtils()

    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr
#       pass

    #@classmethod
    cpdef WriteIntToStream(self, int val, Py_ofstream ostream):
        """
        WriteIntToStream(self, val, ostream)
        Write an integer value to an output binary stream.
        @param val: The object to write.
        @type val: C{int}                               
        @param ostream: The stream to write to.
        @type ostream: L{Py_ofstream}                       
        """
        self.thisptr.WriteValueToStream(<int>val, <ofstream *>(ostream.thisptr))
    
    cpdef WriteDoubleToStream(self, double val, Py_ofstream ostream):
        """
        WriteDoubleToStream(self, val, ostream)
        Write a double value to an output binary stream.
        @param val: The object to write.
        @type val: C{float}                               
        @param ostream: The stream to write to.                       
        @type ostream: L{Py_ofstream}                       
        """
        self.thisptr.WriteValueToStream(<double>val, <ofstream *>ostream.thisptr)

    cpdef int ReadIntFromStream(self, Py_ifstream istream):
        """
        ReadIntFromStream(self, istream)
        Read an integer value from an input binary stream.
        @param istream: The stream to read form.
        @type istream: L{Py_ifstream}                               
        @return: The integer value.
        @rtype: C{int}                       
        """
        return self.thisptr.ReadIntFromStream(<ifstream *>istream.thisptr)

    cpdef double ReadDoubleFromStream(self, Py_ifstream istream):
        """
        ReadDoubleFromStream(self, istream)
        Read a double value from an input binary stream.
        @param istream: The stream to read form.
        @type istream: L{Py_ifstream}                               
        @return: The double value.
        @rtype: C{float}                       
        """
        return self.thisptr.ReadDoubleFromStream(<ifstream *>istream.thisptr)


'''
FOR LOOPER
'''
"""
cpdef public ForLooper(object obj, int size):
    cdef unsigned int i
    cdef unsigned int max = size
    if (hasattr(obj, "Hook")):
        for i in range (max):
            getattr(obj, "Hook")();
"""

'''
MEMORY
'''

# ABORTED : See 'ThisToString()' function of the 'Node' class instead !
#cimport cpython
#
#cdef inline object fromvoidptr(LabNode *a):
#     cdef cpython.PyObject *o
#     o = <cpython.PyObject *> a
#     cpython.Py_XINCREF(o)
#     return <object> o
 

