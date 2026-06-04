cimport cpython.ref as cpy_ref

cdef extern from "adapter/ATestClass.h" namespace "elps" :
    cdef cppclass ATestClass:
        ATestClass(cpy_ref.PyObject *obj)
        int getA()
        int override_me()
        int calculate(int a)

cdef class PyTestClass:
    """
    This class was created for Cython testing purpose and is pointless...
    """

    cdef ATestClass* thisptr

    def __cinit__(self):
       ##print "in TestClass: allocating thisptr"
       self.thisptr = new ATestClass(<cpy_ref.PyObject*>self)
    def __dealloc__(self):
       if self.thisptr:
           ##print "in TestClass: deallocating thisptr"
           del self.thisptr

    def getA(self):
       return self.thisptr.getA()

#    def override_me(self):
#        return self.thisptr.override_me()

    cpdef int calculate(self, int a):
        return self.thisptr.calculate(a) ;
    
# See 'Utils.pyx'
#cdef public api int cy_call_func(object self, char* method, int *error):
#    try:
#        func = getattr(self, method);
#    except AttributeError:
#        error[0] = 1
#    else:
#        error[0] = 0
#        return func()




