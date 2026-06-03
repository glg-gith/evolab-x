# distutils: language = c
# cython: cdivision = True
# cython: boundscheck = False
# cython: wraparound = False
# cython: profile = False

cimport numpy as cnp
import numpy as np
from cython.parallel import parallel, prange
from libc.math cimport sin
cimport openmp
cnp.import_array()

ctypedef cnp.float64_t FLOAT_t
ctypedef cnp.intp_t INT_t
ctypedef cnp.ulong_t INDEX_t
ctypedef cnp.uint8_t BOOL_t

# C memory alloc features
from libc.stdlib cimport malloc, free


cdef FLOAT_t MAXfloat64  = np.float64(np.inf)

cdef class Parent:
    cdef cnp.ndarray numbers
    cdef unsigned int i
    cdef PyObject ** workers
    cdef int nb_workers

    cdef int * buf_sizes
    cdef FLOAT_t ** buffers

    def __init__(Parent self, list numbers, int n_workers):
        self.numbers = <cnp.ndarray[FLOAT_t, ndim=1]> np.array(numbers,dtype=float)
        
        # Define number of workers
        self.nb_workers = n_workers
        self.workers = <PyObject **>malloc(self.nb_workers*cython.sizeof(cython.pointer(PyObject)))
        
        # Populate pool
        cdef int i
        cdef PyObject py_obj
        cdef object py_workers
        py_workers = []                     # For correct ref count
        for i in xrange(self.nb_workers):
            py_workers.append(Worker())
            self.workers[i] = <PyObject*>py_workers[i]
            
        self.init_buffers()
    
    cdef init_buffers(Parent self):
        cdef int i, j
        cdef int num_threads
        cdef int pos, pos_end
        cdef int buf_size
       
        num_threads = self.nb_workers
        buf_size    = len(self.numbers) // num_threads
 
        # Init buffers
        self.buffers   = <FLOAT_t **>malloc(self.nb_workers * cython.sizeof(cython.pointer(FLOAT_t)))
        self.buf_sizes = <int *>malloc(self.nb_workers * cython.sizeof(int))
        pos = 0
        buf_size = len(self.numbers) // num_threads
        
        for i in xrange(self.nb_workers):
            
            # If we are treating the last worker do everything left
            if (i == self.nb_workers-1):
                buf_size = len(self.numbers) - pos
                               
            self.buf_sizes[i] = buf_size
            pos_end = pos + buf_size

            self.buffers[i] = <FLOAT_t *>malloc(buf_size * cython.sizeof(FLOAT_t))
            
            for j in xrange(pos, pos_end):
                self.buffers[i][j-pos] = <FLOAT_t>self.numbers[j]
                
            pos = pos + buf_size

    
        
    cpdef run(Parent self, bint use_parallel):
    
        cdef int i
        cdef FLOAT_t best

        # Run the workers
        if use_parallel:
            print 'parallel'
            with nogil:
                for i in prange(self.nb_workers, num_threads=self.nb_workers):
                    # Changed "FLOAT_t[:]" python object to C array "FLOAT_t *"
                    (<Worker>self.workers[i]).run(<FLOAT_t *>self.buffers[i], self.buf_sizes[i])

        else:
            print 'serial'
            for i in xrange(self.nb_workers):
                (<Worker>self.workers[i]).run(<FLOAT_t *>self.buffers[i], self.buf_sizes[i])
        
        # Make sure they ran
        for i in xrange(self.nb_workers):
            print (<Worker>self.workers[i]).output

        # Choose the worker that had the best solution
        best = MAXfloat64
        for i in xrange(self.nb_workers):
            if ((<Worker>self.workers[i]).output < best):
                best = (<Worker>self.workers[i]).output

        return best

    
cdef class Worker:
    cdef public float output
    def __init__(Worker self):
        self.output = 0.0

    
    # Changed "FLOAT_t[:]" python object to C dyn array "FLOAT_t *"
    cdef void run(Worker self, FLOAT_t * numbers, unsigned int buffer_size) nogil:
        cdef unsigned int i, j
        cdef unsigned int n = buffer_size
        cdef FLOAT_t best
        cdef bint first = True
        cdef FLOAT_t value
        
        # Added initialization
        best = MAXfloat64
        for i in range(n):
            for j in range(n):
                value = sin(numbers[i]*numbers[j])
                if first or (value < best):
                    best = value
                    first = False
        self.output = best
        
        
  