cimport cython

from cpython.ref cimport PyObject
from libcpp.string cimport string
from libcpp.vector cimport vector

from libcpp.set cimport set
from sets import Set


# cdef extern from "adapter/ALabSiteSetsManager.h" namespace "elps" :
#     cdef cppclass ALabSet:
cdef extern from "../inc/LabSet.h" namespace "elps" :
    cdef cppclass LabSet:
        LabSet()
        LabSet(LabSet& src)
        LabSet(PyObject *)
        LabSet(PyObject *, int sizeinbits)
        LabSet(PyObject *, LabSet& src)
                
        PyObject *GetPyObj()
    

        void Clear()
        void Resize(unsigned int sizeinbits)
        void Set(int i)
        void UnSet(int i)
        int  Cardinality()
    
        bint Find(int i)
        void Print()
    
        int GetFirst()
        int ChooseOne(LabRng *rng)
        int GetAt(int n)
        
        int Capacity()
    
#        LabSet & operator = ( LabSet & )            # Not yet implemented in Cython !
        LabSet & operator << ( LabSet & )
    
        LabSet         operator ~  ()            
        LabSet         operator +  ( LabSet & )   
        LabSet         operator *  ( LabSet & )    
        LabSet         operator -  ( LabSet & )    
        LabSet         operator ^  ( LabSet & )    
    
        bint            operator == ( LabSet & )
        bint            operator != ( LabSet & )
    
#        LabSet & operator +=  ( LabSet & )          # Not yet implemented in Cython !
#        LabSet & operator *=  ( LabSet & )          # Not yet implemented in Cython !
#        LabSet & operator -=  ( LabSet & )          # Not yet implemented in Cython !
#        LabSet & operator ^=  ( LabSet & )          # Not yet implemented in Cython !
    
        LabSet & INV()
    
        int CU  ( LabSet & )
        int CI  ( LabSet & )
        int CD  ( LabSet & )
        int CSD ( LabSet & )

        # Nested class "iter"
        cppclass iter:
            iter(LabSet & bv)
            void reset()
            int  end()
            int  next()
            
            
        
cdef class PyLabSet:
    """
    Class implementing a B{Set of Big Integers}.

    B{Note :} The set is sorted in B{ascendant} order.
    
    USAGE :
    =======
    
    >>> a_set = PyLabSet(100)                # Get yourself a set which can contain numbers 
    ...                                      # in range [0..100] (100 excluded).
    >>> a_set.Set(11)                        # Add number '11' to the set
    >>> a_set.Set(84)                        # Add number '84' to the set
    >>> a_set.Print()                        # Display it
    ...                                      # Shows : "{ 11 84 }"
    
    B{Note :} You can iterate through a Set by using a L{PyLabSetIter}.
    
    """
    
    cdef LabSet *thisptr
    cdef bint embed_c

    # For Epydoc only
    def __init__(self, bit_size = -1, bint embed_c = True):
        """
        __init__(self, bit_size = -1, embed_c = True)
        Constructor.
        @param bit_size: Maximum number that can be stored by the Set (default is 10^6).
        @type bit_size: C{int}
        @param embed_c: DO NOT CHANGE - Leave it to True !
        @type embed_c: C{bool}
        """ 

    def __cinit__(self, int bit_size = -1, bint embed_c = True):
        self.embed_c = embed_c
        if (self.embed_c):
            if (bit_size == -1):
                self.thisptr = new LabSet(<PyObject *>self)
            else:
                self.thisptr = new LabSet(<PyObject *>self, bit_size)
       
    def __dealloc__(self):
        #print "delete from PY !", self
        if (self.thisptr) and (self.embed_c):
            del self.thisptr
            self.thisptr = NULL
        pass
    
            
    cpdef Clear(self):
        """
        Clear(self)
        Reset the Set to empty (full zeros).
        """
        self.thisptr.Clear()
    cpdef Resize(self, unsigned int sizeinbits):
        """
        Resize(self, sizeinbits)
        Resize the Set.
                                                                
        WARNING : If the new size is lower than the original size, the Set is truncated.
        @param sizeinbits: The new size.                                                 
        @type sizeinbits: C{int}
        """
        self.thisptr.Resize(sizeinbits)
    cpdef Set(self, int i):
        """
        Set(self, i)
        Add a number to the Set.     
                                               
        WARNING :: 
            No check is perform whether i is in the correct range :   
            i MUST be in [0, size_in_bits_ - 1]                       
        @param i: The number to be added. 
        @type i: C{int}                                          
        """
        self.thisptr.Set(i)
    cpdef UnSet(self, int i):
        """
        UnSet(self, i)
        Remove a number from the Set.  
                                           
        WARNING ::
            No check is perform whether i is in the correct range : 
            i MUST be in [0, size_in_bits_ - 1]                     
        @param i: The number to be removed.
        @type i: C{int}                                          
        """                                
        self.thisptr.UnSet(i)
    cpdef int Cardinality(self):
        """
        Cardinality(self)
        Get the number of elements currently in the Set.
        @return: The number of elements.
        @rtype: C{int}
        """
        return self.thisptr.Cardinality()
        
    cpdef bint Find(self, int i):
        """
        Find(self, i)
        Find an element in the set                         
        @param i: The number to look for.  
        @type i: C{int}                 
        @return: True if the number was found.
        @rtype: C{bool}                      
        """
        return self.thisptr.Find(i)
    cpdef Print(self):
        """
        Print(self)
        Print a string representation of the Set.      
        """
        self.thisptr.Print()

    cpdef int GetFirst(self):
        """
        GetFirst(self)
        Get the first (smallest) element of the Set.                         
        @return: The smallest integer found, or -2 if the Set was empty.
        @rtype: C{int}              
        """
        return self.thisptr.GetFirst()
    cpdef int ChooseOne(self, PyLabRng rng):
        """
        ChooseOne(self, rng)
        Choose an element using uniform integer random number generator.  
        @param rng: The Random Number Generator.
        @type rng: L{PyLabRng}                            
        @return: Return The chosen number, or -2 if the Set was empty.
        @rtype: C{int}
        """     
        return self.thisptr.ChooseOne(rng.thisptr)
    cpdef int GetAt(self, int n):
        """
        GetAt(self, n)
        Get an element at the specified position in the Set.      
        @param n: The position to look at.    
        @type n: C{int}                     
        @return: The requested number, or -2 if the Set was empty.
        @rtype: C{int}
        """ 
        return self.thisptr.GetAt(n)
    
    cpdef int Capacity(self):
        """
        Capacity(self)
        The maximum number the set can store (= size of the Set).      
        @return: The biggest number that can be handled by Set.
        @rtype: C{int}
        """
        return self.thisptr.Capacity()
    
    
    
    def __lshift__(PyLabSet self, PyLabSet other):
        """
        __lshift__(self, other)
        Assignment operator (self << other).
        """
        self.thisptr[0] = ( other.thisptr[0] )
        return self
    
    
    # INVERSION
    def __invert__(PyLabSet self):
        """
        __invert__(self)
        Inversion operator (self << ~self).
        """
        cdef PyLabSet p_s
        p_s = PyLabSet(self.thisptr.Capacity(), False)
        p_s.thisptr = new LabSet(<LabSet>( ~ self.thisptr[0] ))
        return p_s
        
    # UNION
    def __add__(PyLabSet self, PyLabSet other):
        """
        __add__(self, other)
        Union operator (return self + other).
        """
        cdef PyLabSet p_s
        p_s = PyLabSet(self.thisptr.Capacity(), False)
        p_s.thisptr = new LabSet( self.thisptr[0] + other.thisptr[0] )
        return p_s
    
    # INTERSECTION
    def __mul__(PyLabSet self, PyLabSet other):
        """
        __mul__(self, other)
        Intersection operator (return self * other).
        """
        cdef PyLabSet p_s
        p_s = PyLabSet(self.thisptr.Capacity(), False)
        p_s.thisptr = new LabSet( self.thisptr[0] * other.thisptr[0] )
        return p_s
    
    # DIFFERENCE
    def __sub__(PyLabSet self, PyLabSet other):
        """
        __sub__(self, other)
        Difference operator (return self - other).
        """
        cdef PyLabSet p_s
        p_s = PyLabSet(self.thisptr.Capacity(), False)
        p_s.thisptr = new LabSet( self.thisptr[0] - other.thisptr[0] )
        return p_s
    
    # SYMMETRICAL DIFFERENCE
    def __xor__(PyLabSet self, PyLabSet other):
        """
        __xor__(self, other)
        Symmetrical Difference operator (return self ^ other).
        """
        cdef PyLabSet p_s
        p_s = PyLabSet(self.thisptr.Capacity(), False)
        p_s.thisptr = new LabSet( self.thisptr[0] ^ other.thisptr[0] )
        return p_s
    

    def __richcmp__(PyLabSet self, PyLabSet other, int op):
        if op == 2:   return (self.thisptr[0] == other.thisptr[0])
        elif op == 3: return (self.thisptr[0] != other.thisptr[0])
#        <  0
#        == 2
#        >  4
#        <= 1
#        != 3
#        >= 5

    cpdef PyLabSet INV(self):
        """
        INV(self)
        Inversion (return ~self).
        """
        self.thisptr.INV()
        return self

    cpdef int CU (PyLabSet self, PyLabSet other):
        """
        CU(self, other)
        Fast Count Union (return (self + other).Cardinality()).
        Note : A.CU(B) is faster than (A+B).Cardinality() 
        """
        return self.thisptr.CU(other.thisptr[0])
    cpdef int CI (PyLabSet self, PyLabSet other):
        """
        CI(self, other)
        Fast Count Intersection (return (self * other).Cardinality()).
        Note : A.CI(B) is faster than (A*B).Cardinality() 
        """
        return self.thisptr.CI(other.thisptr[0])
    cpdef int CD (PyLabSet self, PyLabSet other):
        """
        CD(self, other)
        Fast Count Difference (return (self - other).Cardinality()).
        Note : A.CD(B) is faster than (A-B).Cardinality() 
        """
        return self.thisptr.CD(other.thisptr[0])
    cpdef int CSD (PyLabSet self, PyLabSet other):
        """
        CD(self, other)
        Fast Count Symmetrical Difference (return (self ^ other).Cardinality()).
        Note : A.CSD(B) is faster than (A^B).Cardinality() 
        """
        return self.thisptr.CSD(other.thisptr[0])


cdef int LABSET_ITER_END = -2

  
#cdef extern from "adapter/ALabSiteSetsManager.h" namespace "elps" :
#    cdef cppclass ALabSetIter:
#        ALabSetIter(ALabSet bv)
#        void reset()
#        int  end()
#        int  next()

cdef class PyLabSetIter:
    """
    Basic L{PyLabSet} iterator utility.
    
    USAGE :                                                 
    =======
    
    >>> ls = PyLabSet()                                             
    ...
    >>> it = PyLabSetIter(ls)                           # Get the iterator        
    >>> while ((val = it.next()) != it.end()):          # Browse until the end
    ...     print(val)                                  # Do something with "val"
    ...                                              
                                                           
    """
    
    cdef LabSet.iter *thisptr
    
    # For Epydoc only
    def __init__(self, PyLabSet labset):
        """
        __init__(self, set)
        Instantiate an iterator for the given Set.
        @param set: Set to be browsed.
        @type set: L{PyLabSet} 
        """              

    def __cinit__(self, PyLabSet labset):
       self.thisptr = new LabSet.iter(labset.thisptr[0])
       
    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr
           
    cpdef reset(self):
        """
        reset(self)
        Reset the iterator.
        """
        self.thisptr.reset()
    cpdef int end(self):
        """
        end(self)
        Get the end marker.
        @return: END (= -2)
        @rtype: C{int}
        """
        self.thisptr.end()
    cpdef int next(self):
        """
        next(self)
        Get the next number from the current position of the iterator.
        @return: The number immediately following the current one.
        @rtype: C{int}
        """
        self.thisptr.next()

#
#cdef extern from "adapter/ALabSiteSetsManager.h" namespace "elps" :
#    cdef cppclass ALabSiteSetsManager:
#        ALabSiteSetsManager(PyObject *obj)    
#        
#        PyObject *GetPyObj()
#
#        
#        void SomethingTheManagerDoes()
#        
#        ALabSet GS(string set_name)
##        int           CNT (LabSet<t_elt> const& a_set) { return this->CNT(a_set); }
##        double        SUM (LabSet<t_elt> const& a_set, string attr_name) { return this->SUM(a_set, attr_name); }
##        double        AVG (LabSet<t_elt> const& a_set, string attr_name) { return this->AVG(a_set, attr_name); }
#
#        
#cdef class PyLabSiteSetsManager__singleton__:
#    """
#    Singleton !
#    
#    Usage :
#    =======
#        
#        - I{GOOD}, call it like this :
#        
#          B{import PyLabSiteSetsManager}
#        
#          B{PyLabSiteSetsManager.GetInstance().SomethingTheManagerDoes()}
#        
#        - I{WRONG}, instead of instantiating it :
#        
#          I{import PyLabSiteSetsManager__singleton__}
#        
#          I{PyLabSiteSetsManager__singleton__().SomethingTheManagerDoes()}
#    
#        The second way of doing would work, but lead to a serious performance drawback.
#    """
#    cdef ALabSiteSetsManager* thisptr
#
#    def __cinit__(self):
#       self.thisptr = new ALabSiteSetsManager(<PyObject*>self)
#       
#    def __dealloc__(self):
#       if self.thisptr:
#           del self.thisptr
#        
#    cpdef SomethingTheManagerDoes(self):
#        self.thisptr.SomethingTheManagerDoes()
#        
#    
##    TODO : Gerer les erreurs son on cherche a utiliser le 'PyLabSiteSetsManager' avec un set qui
##    contient des trucs qui ne sont pas de type PyLabSiteBase, ou PyLabSiteEvent (par exemple des PyLabUserData)
#    
#        
#        
##    cdef PyLabSet LabSetToPy(self, LabSet *lab_set):
##        cdef PyLabSet p_set
##        p_set.site = PyLabSiteBase()
##        p_set.site.thisptr2 = lab_set.site
##        p_set.sets = lab_set.sets
##        return p_set
#        
##    cpdef PyLabSet GS(self, string set_name):
##        return <PyLabSet>self.thisptr.GS(set_name).GetPyObj()
#        
#        
##    cpdef PyLabSiteSetsManager GS(self, string set_name):
##        return <PyLabSiteSetsManager>self.thisptr.GetPyObj()
#        
#
#    cpdef PyLabSiteSetsManager__singleton__ GetInstance(self):
#        return self
#        
#        
#        
#        
#PyLabSiteSetsManager = PyLabSiteSetsManager__singleton__()        
#        
        
        

# cdef extern from "../inc/LabSet.h" namespace "elps" :
#     cdef cppclass LabSet:
#         pass
        

cdef extern from "../inc/LabSiteSetsCalculator.h" namespace "elps" :
    cdef cppclass LabSiteSetsCalculator:
        LabSiteSetsCalculator() #(PyObject *obj)    
        
        #PyObject *GetPyObj()


        LabSet&       GS(string a_set_name)
    
        LabSiteBase * GI(int id)
        
        int GetSitesCount()

    
        int            CNT(LabSet a_set)
        
        double         AVG(LabSet a_set, t_deptype dep_type, int attr_id)
        vector[double] AVG(LabSet a_set, vector[int]& dep_types, vector[int]& attr_ids)
        double         LAVG(LabSet a_set, t_deptype dep_type, int attr_id)
        double         RAVG(LabSet a_set, t_deptype dep_type, int attr_id)
#        double         COV(LabSet a_set, LabSiteBase::t_deptype dep_type_1, int attr_id_1,
#                                                     LabSiteBase::t_deptype dep_type_2, int attr_id_2)
#        double         COR(LabSet a_set, LabSiteBase::t_deptype dep_type_1, int attr_id_1,
#                                                     LabSiteBase::t_deptype dep_type_2, int attr_id_2)
#        double*        DEV(LabSet a_set, LabSiteBase::t_deptype dep_type_1, int attr_id_1,
#                                                     LabSiteBase::t_deptype dep_type_2, int attr_id_2)
#                                                     
        int            CNT_NGH(LabSiteBase *ind, int state)
        int            CNT_NGH(LabSiteBase *ind, t_deptype dep_type, int indexed_attr_id, unsigned int attr_value)
        double         AVG_NGH(LabSiteBase *ind, t_deptype dep_type, int attr_id)

        
cdef class PyLabSiteSetsCalculator:
    """
    This class gives methods to deal with the "Sets" of Sites handled by 
    the B{Sets Manager} (See C++ LabSiteSetsManager class).
    
    It allows to :
        - Get a set (See GS())
        - Get a site (See GI())
        - Perform basic operations on the sites of a set (See AVG(), ...)
    
    USAGE :
    =======
    
    Assuming sets "SET_A" and "SET_B" already exist and are not empty :
    
        >>> _C = PyLabSiteSetsCalculator()                # Get a calculator
        ...
        >>> A = _C.GS("SET_A")                            # Get a set from the manager             
        >>> B = _C.GS("SET_B") 
        ...
        >>> C << A + B                                    # Store the Union
        ...
        >>> S = _C.GI(12)                                 # Get '12'th site (from manager's list of sites)
        ...
        >>> A.Set(12);                                    # Put the '12'th site in set 'A'
        >>> A.Set(84)
        ...
        >>> avg = _C.AVG(A, cyelp.IND, 0)                 # Compute the average of attribute '0' aver all the set 'A'
        ...
    
    """
    
    cdef LabSiteSetsCalculator* thisptr
    
    # For Epydoc only
    def __init__(self):
        """
        __init__(self)
        Default constructor.
        """  
    
    def __cinit__(self):
       self.thisptr = new LabSiteSetsCalculator() #(<PyObject*>self)
       
    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr
    
    cpdef PyLabSet GS(self, string a_set_name):
        """ 
        GS(self, a_set_name)
        Get a Set from the sets manager (See C++ LabSiteSetsManager) .
        @param a_set_name: The name of the set. 
        @type a_set_name: C{str}                            
        @return: The Set object corresponding to that name.
        @rtype: L{PyLabSet}
        """        
        cdef PyLabSet p_s
        p_s = PyLabSet(0, False)
        p_s.thisptr = &(self.thisptr.GS(a_set_name))
        return p_s

    cpdef object GI(self, int id):
        """
        GI(self, id)
        Get a Site from the sites list of the LabSiteSetsManager (in the order                      
        it has been added to the manger ! See C++ LabUserData.SetId() and LabSiteSetsManager.AddSite(), 
        which are used in the various "Populate*()" functions available from L{PyLabArrayBinding} and     
        L{PyLabNetBinding}).                                                                             
        @param id: The index in the sites list.
        @type id: C{int}                                                       
        @return: Returns a pointer on the Site found at this index.
        @rtype: L{PyLabSiteBase}
        """                                  
        if (id < 0) or (id >= self.thisptr.GetSitesCount()):
            return None
        else:
#            return <object>(<ALabSiteBase *>self.thisptr.GI(id)).GetPyObj()
            return <object>(<LabSiteBase *>self.thisptr.GI(id)).CyGetPyObj()
        
        
    cpdef int CNT(self, PyLabSet a_set):
        """
        CNT(self, a_set)
        Same as "a_set.Cardinality()"
        """
        return self.thisptr.CNT(<LabSet>a_set.thisptr[0])

    cpdef double AVG(self, PyLabSet a_set, t_deptype dep_type, int attr_id):
        """
        AVG(self, a_set, dep_type, attr_id)
        Get the average of attribute "attr_id" of type "dep_type" over all the site in set "a_set".                                  
        @param a_set: The set of sites to average.
        @type a_set: L{PyLabSet} 
        @param dep_type: Dependency level of the attribute (See L{PyLabSiteBase.PushAttribute()})
        @type dep_type: C{int}                           
        @param attr_id: Index of the attribute (assuming all the sites in set A have this attribute)
        @type attr_id: C{int}                           
        @return: The average value.
        @rtype: C{float}                                   
        """
        return self.thisptr.AVG(a_set.thisptr[0], dep_type, attr_id)   
    cpdef object MAVG(self, PyLabSet a_set, vector[int] dep_types, vector[int] attr_ids):
        return self.thisptr.AVG(a_set.thisptr[0], dep_types, attr_ids)
    cpdef double LAVG(self, PyLabSet a_set, t_deptype dep_type, int attr_id):
        return self.thisptr.LAVG(a_set.thisptr[0], dep_type, attr_id)
    cpdef double RAVG(self, PyLabSet a_set, t_deptype dep_type, int attr_id):
        return self.thisptr.RAVG(a_set.thisptr[0], dep_type, attr_id)

    cpdef int CNT_NGH_S(self, PyLabSiteBase ind, int state):
        return self.thisptr.CNT_NGH(<LabSiteBase *>ind.thisptr, state)
    cpdef int CNT_NGH_A(self, PyLabSiteBase ind, t_deptype dep_type, int indexed_attr_id, unsigned int attr_value):
        return self.thisptr.CNT_NGH(<LabSiteBase *>ind.thisptr, dep_type, indexed_attr_id, attr_value)
    cpdef double AVG_NGH(self, PyLabSiteBase ind, t_deptype dep_type, int attr_id):
        return self.thisptr.AVG_NGH(<LabSiteBase *>ind.thisptr, dep_type, attr_id)
        
        
    
#    TODO : Gerer les erreurs son on cherche a utiliser le 'PyLabSiteSetsManager' avec un set qui
#    contient des trucs qui ne sont pas de type PyLabSiteBase, ou PyLabSiteEvent (par exemple des PyLabUserData)
    
        
        
        
        
        
        
