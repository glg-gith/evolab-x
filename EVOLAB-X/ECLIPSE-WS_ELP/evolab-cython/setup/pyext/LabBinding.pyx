from cpython.ref cimport PyObject
from libcpp.string cimport string
from libcpp.vector cimport vector

import sys


'''
NODE
'''
#ctypedef void* t_user_data

# cdef extern from "../inc/LabNetwork.h" namespace "elps" :
#     cdef cppclass LabNode:
#         pass

# cdef extern from "adapter/ALabNetBinding.h" namespace "elps" :
#     cdef cppclass ALabNode:
cdef extern from "../inc/LabNetwork.h" namespace "elps" :
    cdef cppclass LabNode:
        LabNode()       
        #LabNode(PyObject *obj)       
        PyObject *CyGetPyObj()
        void CySetPyObj(PyObject *obj)
        
        PyObject * CyGetUserData()
        void CySetUserData(PyObject *data)
        vector[LabNode *] GetNeighbors()

        vector[double] MinPaths(vector[LabNode *]& nodes)

cdef class PyLabNode:
    """
    The node is the base entity constituting a Network.
    Those classes have a natural hierarchy of L{PyLabNetwork} > L{PyLabNode} > C{edges}.
    
    A PyLabNode is a node of the L{PyLabNetwork} (they are link between each other by edges).
    Each node can carry an object / site (of any kind, user is free to choose a custom 
    representation, but it is recommended to use existing L{PyLabSiteBase}, L{PyLabSiteEvent} 
    or at least sub-classes of L{PyLabSiteBase}).
    
    USAGE :
    =======
    
    A node cannot exist without a network (See L{PyLabNetwork} first). Once your network
    is populated of nodes, each node has to be setup. There is two ways of doing this :
    
    B{Manually} :
    
    >>> a_site = PyLabSiteBase(sim)         # Create an object to be stored in the node
    >>> a_node.SetUserData(a_site)          # Assign this object to the node
    
    There is also more B{automated ways} which come with the L{PyLabNetBinding} available 
    via the "netBinding" member of all B{*Net} versions of the simulator (See, for example, 
    the L{PyLabSimulatorTimeNet}). See L{PyLabNetBinding}, for a deeper description of
    those methods.
    
    Moreover those B{automated ways} give access to more advanced features (which are
    not available while proceeding B{manually}) : the Sets related features (See 
    L{PyLabSiteSetsCalculator} for getting a full understanding of the B{Sets based}
    system).
    
    """
    
    cdef LabNode *thisptr
    cdef PyLabNetwork network
    # Handling correct reference counting on the objects (Python specific)
    cdef object userdata
    # Index in the Network list
    cdef int index

    # For Epydoc only
    def __init__(self, embed_c = True):
        """
        __init__(self, embed_c = True)
        Default constructor.
        @param embed_c: DO NOT CHANGE - Leave it to True !
        @type embed_c: C{bool}
        """ 
        pass

    def __cinit__(self, bint embed_c = True):
        if (embed_c):
            self.thisptr = new LabNode()
            #self.thisptr = new LabNode(<PyObject *>self) # Slow, don't know why ...
              
    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr

    cdef SetNetwork(self, PyLabNetwork net):
        self.network = net
    cpdef GetNetwork(self):
        return self.network
        
        
    cpdef object GetUserData(self):
        """
        GetUserData(self)
        Get user's data embedded in this PyLabNode 
        (usually a Site - See L{PyLabSiteBase} and L{PyLabSiteEvent})
        @return: The data carried by this node.
        @rtype: C{object}
        """
        return <object>self.thisptr.CyGetUserData()
#    cdef void* GetUserData0(self):
#        return self.thisptr.GetUserData()
        
    cpdef SetUserData(self, object):
        """
        SetUserData(self, object)
        Set user's data embedded in this PyLabNode 
        (usually a Site - See L{PyLabSiteBase} and L{PyLabSiteEvent})
        @param object: The data carried by this node.
        @type object: C{object}
        """
        self.userdata = object
        self.thisptr.CySetUserData(<PyObject *>object)

#    def GetNeighbors(self):
#        return self.neighbors
    cpdef int GetNeighborsCount(self):
        """
        GetNeighborsCount(self)
        Get the number of direct neighbors of the current LabNode.
        @return: A number of neighbors.
        @rtype: C{int}
        """
        return self.thisptr.GetNeighbors().size()
    cpdef PyLabNode GetNeighborAt(self, int i):
        """
        GetNeighborAt(self, i)
        Get the direct neighbor of the current LabNode located at the specified index.
        @param i: Index in the neighbors list.
        @type i: C{int} 
        """
        return <PyLabNode>self.thisptr.GetNeighbors()[i].CyGetPyObj()
    

    cpdef int GetIndex(self):
        """
        GetIndex(self)
        Get the index of the node in its network.
        @return: An index in the nodes list of the parent network.
        @rtype: C{int}
        """
        return self.index
    cdef SetIndex(self, idx):
        self.index = idx

    
    # GLG WARN : UNTESTED !!!
    cpdef vector[double] MinPaths(self, nodes):
        """
        Get the distance between this PyLabNode and each other in the given group.
        @param nodes: The group of nodes to be confronted to this one.
        @type nodes: C{list}
        @return: The distances vector of doubles.
        @rtype: C{list}
        """
        cdef vector[LabNode *] c_nodes
        cdef unsigned int i
        for i in range(len(nodes)):
            c_nodes.push_back((<PyLabNode>nodes[i]).thisptr)
        return self.thisptr.MinPaths(c_nodes)

        
'''
NETWORK
'''
# cdef extern from "adapter/ALabNetBinding.h" namespace "elps" :
#     cdef cppclass ALabNetwork:
cdef extern from "../inc/LabNetwork.h" namespace "elps" :
    cdef cppclass LabNetwork:
        LabNetwork(PyObject *obj)
        PyObject *CyGetPyObj()
        void CySetPyObj(PyObject *obj)

        void Populate(int nb_sites)
        
        vector[LabNode *]& GetNodes()
        int GetSize()
        
        bint ErdosRenyi(double _lambda)
        bint SparseRandomGraph(double _lambda)
        bint FastRandomGraph(double _lambda)
        bint RingLattice(int N, int K)
        bint SquareLattice(int R, int C, bint diag)
        bint SmallWorld(int N, int K, double beta)
        bint RandConnectUser(vector[double]& dist)
        bint RandConnectUserLinear(int con_deg)
        bint RandConnectExplicit(vector[int]& deg_series)
        bint RandConnectPoisson(double _lambda)
        bint RandConnectPowerlaw(double alpha, double kappa)
        bint RandConnectExponential(double _lambda)
        
        void MigrateNodes(double fraction_to_migrate)

    
cdef class PyLabNetwork:
    """
    Main class for handling a Network (in a B{SPATIALIZED} context).
    Those classes have a natural hierarchy of L{PyLabNetwork} > L{PyLabNode} > C{edges}.
    
    (an edge is a connection between a node and one of its neighbors).
    
    That means you probably should not be doing something with nodes L{PyLabNode}
    unless they already belong to a network L{PyLabNetwork}.
    
    There are many ways you can construct a network, and several functions are
    available to automate the process, by providing algorithms for connecting the edges.
    
    USAGE :
    =======
    
    Start out by getting yourself a network :
    
    >>> my_network = PyLabNetwork()
    
    Then construct it (populate with nodes and connect choosing an algorithm) :

    >>> my_network.Populate(10000)         # Add 10^4 nodes to the network.
    >>> my_network.RandConnectPoisson(4)   # Connect the nodes using a
    ...                                    # Poisson(4) degree distribution and get rid of 
    ...                                    # self-loops and multi-edges (done automatically).
    
    """
    cdef LabNetwork *thisptr
    cdef object nodes

    # For Epydoc only
    def __init__(self):
        """
        __init__(self)
        Default constructor.
        """ 
    def __cinit__(self):
       self.thisptr = new LabNetwork(<PyObject *>self)
       self.nodes = []
       
    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr


    cdef inline setup_pyobjects(self):
        # Populate a PyNodes list / Mirroring the C++ LabNetwork
        
        # Clear the nodes list before rebuilding it
        del self.nodes[:]
        
        cdef unsigned int i
        cdef PyLabNode a_node
        for i in range(self.thisptr.GetNodes().size()):
            # Set node
            a_node = PyLabNode(False)
            self.nodes.append(a_node)
            """
            >>> WARNING :    Downcasting a 'LabNode' to a 'ALabNode' is not safe at all.
            >>> -------      We keep it this way but we !! MUST !! be aware of that when 
            >>>              modifying 'ALabSiteBase' class
            >>>              => The 'ALabNode' adapter !! MUST !! contain methods only, NO ATTRIBUTES !
            >>>              => The attributes must be READ and WRITTEN from within the base (LabNode) !
            >>>              Note : dynamic_cast<???> doesn't yet exists in cython... 
            """
            a_node.thisptr = <LabNode *>self.thisptr.GetNodes()[i]
            a_node.thisptr.CySetPyObj(<PyObject *>a_node)
            a_node.SetNetwork(self)
            a_node.SetIndex(i)
            
        
        
    cpdef Populate(self, int nb_sites):
        """
        Populate(self, nb_sites)
        Populate the network with C{nb_sites} nodes.
        @param nb_sites: Number of nodes (= sites).
        @type nb_sites: C{int}
        """
        self.thisptr.Populate(nb_sites)
        self.setup_pyobjects()
        
        
    
    cpdef int GetSize(self):
        """
        GetSize(self)
        Get the size of the network.
        @rtype: C{int}
        @return: The number of nodes / sites of this network.
        """
        return self.thisptr.GetSize()

    cpdef list GetNodes(self):
        """
        GetNodes(self)
        Get the list of all nodes.
        @rtype: C{list}
        @return: An indexed list of L{PyLabNode} objects.
        """
        return self.nodes
    cpdef PyLabNode GetNodeAt(self, int i):
        """
        GetNodeAt(self, i)
        Get a node at the specified index in the nodes list.
        @param i: The index in the list.
        @type i: C{int}.
        @rtype: L{PyLabNode}
        @return: A node object.
        """
        return <PyLabNode>(<LabNode *>self.thisptr.GetNodes()[i]).CyGetPyObj()
    
    
        
    cpdef bint ErdosRenyi(self, double _lambda):
        """
        ErdosRenyi(self, _lambda)
        Erdos Renyi Network.
        
        ErdosRenyi() and RandConnectPoisson() (should) produce equivalent
        networks, although the former will always allow degree 0 nodes, while
        the later can be modified to use a left-truncated distribution.
        
        Erdos-Renyi can only produce Poisson distributed networks, whereas
        all the RandConnect* functions use an adaptation of the Molloy-Reed
        algorithm, and therefore can take any discrete, non-negative distribution.
        @param _lambda: 
        @type _lambda: C{double}
        """
        return self.thisptr.ErdosRenyi(_lambda)
    
    cpdef bint SparseRandomGraph(self, double _lambda):
        """
        SparseRandomGraph(self, _lambda)
        Sparse distribution.
        @param _lambda: 
        @type _lambda: C{double}
        """
        return self.thisptr.SparseRandomGraph(_lambda)
    
    cpdef bint FastRandomGraph(self, double _lambda):
        """
        FastRandomGraph(self, _lambda)
        FastRandomGraph() tries to pick the fastest algorithm based on parameters given.
        @param _lambda: 
        @type _lambda: C{double}
        """
        return self.thisptr.FastRandomGraph(_lambda)
    
    cpdef bint RingLattice(self, int N, int K):
        """
        RingLattice(self, N, K)
        Ring lattice with N nodes, each connected to K nearest neighbors.
        @param N: Number of nodes.
        @type N: C{int}
        @param K: Number of nearest neighbors for each node.
        @type K: C{int}
        """
        return self.thisptr.RingLattice(N, K)
    
    cpdef bint SquareLattice(self, int R, int C, bint diag):
        """
        SquareLattice(self, R, C, diag)
        R x C lattice, including diagonals if C{diag} is True.
        @param R: Number of rows.
        @type R: C{int}
        @param C: Number of columns.
        @type C: C{int}
        @param diag: If the diagonals should be connected.
        @type diag: C{bool}
        """
        return self.thisptr.SquareLattice(R, C, diag)
    
    cpdef bint SmallWorld(self, int N, int K, double beta):
        """
        SmallWorld(self, N, K, beta)
        Watts-Strogatz small world network with N nodes initially connected to K neighbors
        and shuffled with probability beta.
        @param N: Number of nodes.
        @type N: C{int}
        @param K: Number of initially connected neighbors for each node.
        @type K: C{int}
        @param beta: Shuffle probability.
        @type beta: C{double}
        """
        return self.thisptr.SmallWorld(N, K, beta)
    
    cpdef bint RandConnectUser(self, object dist):
        """
        RandConnectUser(self, dist)
        User provides arbitrary (normalized!) distribution. Each value should be the probability
        of the index (index = degree), i.e. if the contents of the vector are
        (0, 0.13, 0.2, 0.04, 0.63), then the probability of drawing a deviate (=degree)
        of 0 is 0, of 1 is 13%, of 2 is 20%, and so on.
        @param dist: Vector of probabilities.
        @type dist: C{list}
        """
        return self.thisptr.RandConnectUser(dist)
    
    cpdef bint RandConnectUserLinear(self, int con_deg):
        """
        RandConnectUserLinear(self, con_deg)
        Same as RandConnectUser(), but specifying only the connection degree.
        The algorithm takes care of computing linearly the intermediate values. Sum of
        all degrees should be even, or else it's impossible to connect all the nodes.
        @param con_deg: Number of connected neighbors.
        @type con_deg: C{int}
        """
        return self.thisptr.RandConnectUserLinear(con_deg)
    
    cpdef bint RandConnectExplicit(self, object deg_series):
        """
        RandConnectExplicit(self, deg_series)
        User provides an explicit degree series, one degree per node. Sum of
        all degrees should be even, or else it's impossible to connect all nodes.
        @param deg_series: Vector of C{int}.
        @type deg_series: C{list}
        """
        return self.thisptr.RandConnectExplicit(deg_series)
    
    cpdef bint RandConnectPoisson(self, double _lambda):
        """
        RandConnectPoisson(self, _lambda)
        Same as RandConnectUser() using a Poisson distribution.
        @param _lambda: 
        @type _lambda: C{double}
        """
        return self.thisptr.RandConnectPoisson(_lambda)
    
    cpdef bint RandConnectPowerlaw(self, double alpha, double kappa):
        """
        RandConnectPowerlaw(self, alpha, kappa)
        Same as RandConnectUser() using a Powerlaw distribution.
        @param alpha: 
        @type alpha: C{double}
        @param kappa: 
        @type kappa: C{double}
        """
        return self.thisptr.RandConnectPowerlaw(alpha, kappa)
    
    cpdef bint RandConnectExponential(self, double _lambda):
        """
        RandConnectPoisson(self, _lambda)
        Same as RandConnectUser() using a Exponential distribution.
        @param _lambda: 
        @type _lambda: C{double}
        """
        return self.thisptr.RandConnectExponential(_lambda)
    
        
    cpdef MigrateNodes(self, double fraction_to_migrate):
        """
        MigrateNodes(self, fraction_to_migrate)
        Migrates the given fraction of Nodes in the network by rearranging their edges (= connections).
        
        Algorithm details :

        At each call (period), (fraction_to_migrate * 100) % of all nodes "migrate".
        A different group is randomly selected each period. If a nodes is in the migrating
        group, all of its edges are broken; this means the node's neighbors also
        end up with at least one broken edge. These broken edges ("stubs") are
        then randomly reconnected. The reconnection process will make sure that
        no self-loops or parallel edges get created (it's possible that
        additional edges will get shuffled in order to get rid of these).
        
        @param fraction_to_migrate: The fraction [0.0, 1.0] of nodes to be reorganized.
        @type fraction_to_migrate: C{double}
        """
        self.thisptr.MigrateNodes(fraction_to_migrate)


'''
UTILS
'''

cdef py_get_class = lambda x: globals()[x]


'''
NET BINDING
'''
cdef extern from "../inc/LabNetBinding.h" namespace "elps" :
    cdef cppclass LabNetBinding:
        LabNetwork *GetNetwork()
        void SetNetwork(LabNetwork *net)
        void SaveNetwork(string filename)
        void LoadNetwork(string filename)
            
#        void Populate(ALabSiteBase *default_site, bool draw_progess = false)
        void Populate(vector[double]& dist, vector[LabSiteBase *]& model_sites, bint draw_progress)
        void Populate(vector[int]& dist, vector[LabSiteBase *]& model_sites, bint draw_progress)
    
        void SetOneSiteAt(LabSiteBase *a_site, int node_index)
        void BeginSetSites()
        void EndSetSites()

        # TODO : Untested
        void CopySite(LabSiteBase *dest_site, LabSiteBase *src_site)
        # TODO : Untested
        void SwapSite(LabSiteBase *l_site, LabSiteBase *r_site)
        
        int GetSize()
        LabSiteBase **GetSites()

 
    
cdef class PyLabNetBinding:
    """
    This class embeds a network (See L{PyLabNetwork}) and provides features
    to populate it (for use in a B{SPATIALIZED} context). As well as short access 
    features to backup and restore the network (using text formatted files).
    
    All the B{*Net} sub-classes of the simulator (See L{PyLabSimulatorBase})
    have an aggregated instance of PyLabNetBinding (See, for example, 
    L{PyLabSimulatorTimeNet.netBinding} for an illustration of this).
    
    USAGE :
    =======
    
    First you will need a network and a simulator :
    
    >>> pop_max = 10000
    >>> net = PyLabNetwork()                          # Get yourself a network
    >>> net.Populate(pop_max)                         # Populate the network with nodes
    ...
    >>> sim = PyLabSimulatorTimeNet()                 # Get yourself a simulator
    >>> sim.SetMaxPopulationSize(pop_max)             # Set the simulator's capacity
    >>> sim.netBinding.SetNetwork(net)                # Assign the network to the simulator
    
    Then, as explained in L{PyLabNode}, you'll need object(s) representing each Site. 
    Each node needs a Site.
    For this you can use ready made ones (ex. L{PyLabSiteBase}, ... or derivatives) and
    assign them manually (See L{PyLabNode.SetUserData()}). But, PyLabNetBinding provides 
    methods to ease the assignment of B{sites model objects} to the nodes 
    (See L{PyLabNetBinding.Populate1()} and L{PyLabNetBinding.Populate2()}).
    
    Another big advantage of using those methods is that it turns on the B{Sets Manager}
    (See L{PyLabSiteSetsCalculator}, ...).
    
    B{The half-manual way :}
    
    >>> sim.netBinding.BeginSetSites()                # Mandatory to open the sites in writing
    ...
    >>> a_site = PyLabSiteEvent(sim, a_node)          # Create a Site
    >>> sim.SetOneSiteAt(a_site, a_node.GetIndex())   # Mandatory to assign a site to a node
    ...                                               # (this enables the Sets Manager to monitor the Site)
    ...                                               
    ...                                               # Repeat the above for EVERY single site
    ...
    >>> sim.netBinding.EndSetSites()                  # Close the sites in writing
    
    This allows to customize each node on the fly, but keep in mind that every single node
    has to be assigned before closure.
    
    B{The automated way :}
    
    >>> models = []
    >>> a_model = PyLabSiteEvent(sim)
    >>> model_sites.append(a_model)                   # Do this for each different kind of site you need
    ...
    >>> dist = [0.25, 0.75]                           # Define a distribution (here 2 models to be spread)
    >>> sim.netBinding.Populate1(dist, models, True)  # Batch assignment (spread) of the sites over the network
    ...
    
    Note : L{PyLabNetBinding.Populate1()} and L{PyLabNetBinding.Populate2()} take in charge
    everything for you. Your role is restrained to the creation of the models, and the choice
    of the spreading method. 
    """               

    cdef LabNetBinding* thisptr
    cdef PyLabNetwork net
    cdef object sites
    cdef PyLabSimulatorBase simulator

    # For Epydoc only
    def __init__(self, sim):
        """
        __init__(self, sim)
        Constructor.
        @param sim: The simulator this binding is attached to.
        @type sim: L{PyLabSimulatorBase} 
        """ 

    def __cinit__(self, PyLabSimulatorBase sim):
       self.thisptr = new LabNetBinding()
       self.sites = []
       simulator = sim
       
    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr
           
    cpdef PyLabNetwork GetNetwork(self):
        """
        GetNetwork(self)
        Get the related Network - See L{PyLabNetwork}.
        @return: The binded Network.
        @rtype: L{PyLabNetwork}
        """
        return <PyLabNetwork>self.thisptr.GetNetwork().CyGetPyObj()
    
    cpdef SetNetwork(self, PyLabNetwork net):
        """
        SetNetwork(self, net)
        Link the binder to a given network.
        @param net: The Network to link to.
        @type net: L{PyLabNetwork} 
        """ 
        self.net = net
        self.thisptr.SetNetwork(net.thisptr)

    cpdef SaveNetwork(self, string filename):
        """
        SaveNetwork(self, filename)
        Save the network to a text file (defining the edges connections).
        @param filename: The Network to link to.
        @type filename: C{str} 
        """ 
        self.thisptr.SaveNetwork(filename)

    # !!! UNTESTED !!!
    cpdef LoadNetwork(self, string filename):
        """
        LoadNetwork(self, filename)
        Restore the network from a text file (defining the edges connections).
        @param filename: The path of the file to load from (can be relative).
        @type filename: C{str} 
        """ 
        self.thisptr.LoadNetwork(filename)
        self.net = PyLabNetwork()
        self.net.thisptr = self.thisptr.GetNetwork()
        self.thisptr.GetNetwork().CySetPyObj(<PyObject *>self.net)
        self.net.setup_pyobjects()



    cdef inline setup_pyobjects(self):
        # Populate a PySites list / Mirroring the C++ LabNet
        
        print "Setting up Python objects..."
        
        # Clear the sites list before rebuilding it
        del self.sites[:]
        
        cdef int i
        cdef int j
        cdef PyLabSiteBase a_site
        cdef string a_class_name
        
        cdef LabSiteBase *a_lab_site
        
        
        cdef int nb_sites
        nb_sites = self.thisptr.GetSize()
        
        print "setup 1"
        
        for i in xrange(nb_sites):
            
            """
            >>> WARNING :    Downcasting a 'LabSiteBase' to a 'ALabSiteBase' is not safe at all.
            >>> -------      We keep it this way but we !! MUST !! be aware of that when 
            >>>              modifying 'ALabSiteBase' class.
            >>>              => The 'ALabSiteBase' adapter !! MUST !! contain methods only, NO ATTRIBUTES !
            >>>              => The attributes must be READ and WRITTEN from within the base (LabSiteBase) !
            >>>              Note : dynamic_cast<???> doesn't yet exists in cython... 
            """
            a_lab_site = self.thisptr.GetSites()[i]
            
            # Create python site
            a_class_name = a_lab_site.CyGetSiteModelType()
            a_site = py_get_class(a_class_name)(self.simulator, self.GetNetwork().GetNodes()[i], False)
            
            
            # Update C pointer
            a_site.thisptr = <LabUserData *>a_lab_site
           
            self.GetNetwork().GetNodes()[i].SetUserData(a_site)
            
            # Could be useful for reference counting, but 
            # we here SetUsreData handles it well using userdata attribute...
            #self.sites.append(a_site)
            
            # Update PyObject pointer
            a_site.thisptr.CySetPyObj(<PyObject *>a_site)                
            
            # Update the parent node callback's parameter value
            if (isinstance(a_site, PyLabSiteEvent)):
                for j in xrange((<LabSiteEvent *>a_site.thisptr).GetNbEvents()):
                    (<LabSiteEvent *>a_site.thisptr).SetCallBackParamAt(j, <void *>((<PyLabSiteEvent>a_site).parent))
                    
            if ((i % (nb_sites // 100)) == 0): 
                sys.stdout.write('.')
                sys.stdout.flush()
                

        sys.stdout.write("\nSetting up Python objects... Done !\n")
        sys.stdout.flush()


#    void Populate0(ALabSiteBase *default_site, bool draw_progess = false)


    cpdef Populate1(self, vector[double] dist, object model_sites, bint draw_progress):
        """
        Populate1(self, dist, model_sites, draw_progress)
        Populate randomly according to the given distribution and the models.
        
        Note : dist and model_sites must be the same size.
        @param dist: Vector of C{float}. Cumulative sum must be 1.0.
        @type dist: C{list} 
        @param model_sites: Vector of models / template sites - See L{PyLabSiteBase}. Cumulative sum must be 1.0.
        @type model_sites: C{list} 
        @param draw_progress: If the progress bar must be displayed while populating.
        @type draw_progress: C{bool} 
        """
        cdef unsigned int i
        
#        cdef vector[double] c_dist
#        for i in xrange(len(dist)):
#            c_dist.push_back(dist[i])
            
        cdef vector[LabSiteBase *] c_model_sites
        cdef PyLabSiteBase p_site
        for i in xrange(len(model_sites)):
            p_site = model_sites[i]
            p_site.CySetSiteModelType(type(p_site).__name__)
            c_model_sites.push_back(<LabSiteBase *>(p_site.thisptr))
            
        self.thisptr.Populate(dist, c_model_sites, draw_progress)
        self.setup_pyobjects()
        
        
    cpdef Populate2(self, vector[int] dist, object model_sites, bint draw_progress):
        """
        Populate2(self, dist, model_sites, draw_progress)
        Populate with exactly the given amounts given by dist and the models.
        
        Note : dist and model_sites must be the network's size.
        @param dist: Vector of C{int}. Cumulative sum must be network's size.
        @type dist: C{list} 
        @param model_sites: Vector of models / template sites - See L{PyLabSiteBase}. Cumulative sum must be 1.0.
        @type model_sites: C{list} 
        @param draw_progress: If the progress bar must be displayed while populating.
        @type draw_progress: C{bool} 
        """
        cdef unsigned int i
        
#        cdef vector[int] c_dist
#        for i in xrange(len(dist)):
#            c_dist.push_back(dist[i])
            
        cdef vector[LabSiteBase *] c_model_sites
        cdef PyLabSiteBase p_site
        for i in xrange(len(model_sites)):
            p_site = model_sites[i]
            p_site.CySetSiteModelType(type(p_site).__name__)
            c_model_sites.push_back(<LabSiteBase *>(p_site.thisptr))
            
        self.thisptr.Populate(dist, c_model_sites, draw_progress)
        self.setup_pyobjects()
        

    cpdef BeginSetSites(self):
        """
        BeginSetSites(self)
        Starts populating with a given number of sites.
        
        WARNING : This will reset all the sets that have been set earlier.
        Must be call before to start adding sites with L{SetOneSiteAt()}.
        """
        self.thisptr.BeginSetSites()

    cpdef SetOneSiteAt(self, PyLabSiteBase a_site, int node_index):
        """
        SetOneSiteAt(self, a_site, node_index)
        Adds a single site to the sets manager.                                                              
        A call to this function must be enclosed between L{BeginSetSites()} and L{EndSetSites()} calls.
        
        WARNING : All sites B{must} be set before ending - See L{EndSetSites()}.                                   
        @param a_site: Site to be set.
        @type a_site: L{PyLabSiteBase} 
        @param node_index: Index at which the Site is to be set (regarding the ordered list of this population).
        See also L{PyLabSiteSetsCalculator.GI()}.
        @type node_index: C{int} 
        """
        self.thisptr.SetOneSiteAt(<LabSiteBase *>a_site.thisptr, node_index)
            
    cpdef EndSetSites(self):
        """
        EndSetSites(self)
        Ends populating sites operation. Checks if all sites have been set. If not, terminates the program.  
        All network's sites must have been set before ending.             
        """
        self.thisptr.EndSetSites()
        self.setup_pyobjects()


    # TODO : Untested
    cpdef CopySite(self, PyLabSiteBase dest_site, PyLabSiteBase src_site):
        """
        TODO : UNTESTED !
        """
        self.thisptr.CopySite(<LabSiteBase *>dest_site.thisptr, <LabSiteBase *>src_site.thisptr)
    # TODO : Untested
    cpdef SwapSite(self, PyLabSiteBase l_site, PyLabSiteBase r_site):
        """
        TODO : UNTESTED !
        """
        self.thisptr.SwapSite(<LabSiteBase *>l_site.thisptr, <LabSiteBase *>r_site.thisptr)
        



'''
ARRAY BINDING
'''
cdef extern from "../inc/LabArrayBinding.h" namespace "elps" :
    cdef cppclass LabArrayBinding:
            
#        void Populate(int max_size, ALabSiteBase *default_site, bool draw_progess = false)
        void Populate(int max_size, vector[double]& dist, vector[LabSiteBase *]& model_sites, bint draw_progress)
        void Populate(int max_size, vector[int]& dist, vector[LabSiteBase *]& model_sites, bint draw_progress)
    
        void SetOneSiteAt(LabSiteBase *a_site, int node_index)
        void BeginSetSites(int max_size)
        void EndSetSites()
    
        # TODO : Untested
        void CopySite(LabSiteBase *dest_site, LabSiteBase *src_site)
        # TODO : Untested
        void SwapSite(LabSiteBase *l_site, LabSiteBase *r_site)
        
        int GetSize()
        LabSiteBase **GetSites()

    
cdef class PyLabArrayBinding:
    """
    This class embeds a list of sites and provides features to populate it
    (for use in a B{MASS ACTION} context).
    
    All the B{*Array} sub-classes of the simulator (See L{PyLabSimulatorBase})
    have an aggregated instance of PyLabArrayBinding (See, for example, 
    L{PyLabSimulatorTimeArray.arrayBinding} for an illustration of this).
    
    USAGE :
    =======
    
    First you will need an B{array based} simulator :
    
    >>> pop_max = 10000
    >>> sim = PyLabSimulatorTimeArray()                         # Get yourself a simulator
    >>> sim.SetMaxPopulationSize(pop_max)                       # Set the simulator's capacity

    Then fill the array binding with the corresponding number of sites. 
    The array must contain as many Sites as the simulator's capacity (10000 in the above
    example).
    For this you can use ready made ones (ex. L{PyLabSiteBase}, ... or derivatives) and
    assign them manually. But, PyLabArrayBinding provides methods to ease the filling 
    of the array from B{sites model objects} (See L{PyLabArrayBinding.Populate1()} and 
    L{PyLabArrayBinding.Populate2()}).

    Another big advantage of using those methods is that it turns on the B{Sets Manager}
    (See L{PyLabSiteSetsCalculator}, ...).

    The usages are pretty much same as in L{PyLabNetBinding}.

    B{The half-manual way :}
    
    >>> sim.arrayBinding.BeginSetSites(pop_max)                  # Mandatory to open the sites in writing
    ...
    >>> a_site = PyLabSiteBase(sim)                              # Create a Site
    >>> sim->SetOneSiteAt(a_site, 0)                             # Mandatory to assign a site to a specified location in the 
    ...                                                          # array (this enables the Sets Manager to monitor the Site)
    ...                                               
    ...                                                          # Repeat the above for EVERY single site
    ...
    >>> sim.arrayBinding.EndSetSites()                           # Close the sites in writing
    
    This allows to customize each site on the fly, but keep in mind that every single site in the array
    has to be assigned before closure.
    
    B{The automated way :}
    
    >>> models = []
    >>> a_model = PyLabSiteBase(sim)
    >>> model_sites.append(a_model)                              # Do this for each different kind of site you need
    ...
    >>> dist = [0.25, 0.75]                                      # Define a distribution (here 2 models to be spread)
    >>> sim.arrayBinding.Populate1(pop_max, dist, models, True)  # Batch assignment (spread) of the sites over the network
    ...
    
    Note : L{PyLabArrayBinding.Populate1()} and L{PyLabArrayBinding.Populate2()} take in charge
    everything for you. Your role is restrained to the creation of the models, and the choice
    of the spreading method. 
    """
    
    cdef LabArrayBinding* thisptr
    cdef object sites
    cdef PyLabSimulatorBase simulator


    # For Epydoc only
    def __init__(self, sim):
        """
        __init__(self, sim)
        Constructor.
        @param sim: The simulator this binding is attached to.
        @type sim: L{PyLabSimulatorBase} 
        """ 

    def __cinit__(self, PyLabSimulatorBase sim):
       self.thisptr = new LabArrayBinding()
       self.sites = []
       simulator = sim
       
       add_sites_opened = False
       
    def __dealloc__(self):
       if self.thisptr:
           del self.thisptr
           
           
    cdef inline setup_pyobjects(self):
        # Populate a PySites list / Mirroring the C++ LabNet
        
        print "Setting up Python objects..."
        
        # Clear the sites list before rebuilding it
        del self.sites[:]
        
        cdef int i
        cdef int j
        cdef PyLabSiteBase a_site
        cdef string a_class_name
        
        cdef LabSiteBase *a_lab_site
        
        cdef int nb_sites
        nb_sites = self.thisptr.GetSize()
        
        for i in xrange(nb_sites):

            a_lab_site = self.thisptr.GetSites()[i]

            # Create python site
            a_class_name = a_lab_site.CyGetSiteModelType()
            a_site = py_get_class(a_class_name)(self.simulator, None, False)
            
            # Update C pointer
            a_site.thisptr = <LabUserData *>a_lab_site
           
            # Useful for reference counting, instead of doing it 
            # using Py_XINCREF (in LabUserData.cpp)...
            self.sites.append(a_site)
            
            # Update PyObject pointer
            a_site.thisptr.CySetPyObj(<PyObject *>a_site) 
            
#            # Update the parent node callback's parameter value
#             if (isinstance(a_site, PyLabSiteEvent)):
#                 for j in xrange((<LabSiteEvent *>a_site.thisptr).GetNbEvents()):
#                     (<LabSiteEvent *>a_site.thisptr).SetCallBackParamAt(j, <void *>((<PyLabSiteEvent>a_site).parent))
                    
            if ((i % (nb_sites // 100)) == 0): 
                sys.stdout.write('.')
                sys.stdout.flush()
                

        sys.stdout.write("\nSetting up Python objects... Done !\n")
        sys.stdout.flush()

           
#    void Populate0(int max_size, ALabSiteBase *default_site, bool draw_progess = false)


    cpdef Populate1(self, int max_size, vector[double] dist, object model_sites, bint draw_progress):
        """
        Populate1(self, max_size, dist, model_sites, draw_progress)
        Populate randomly according to the given distribution and the models.
        
        Note : dist and model_sites must be the same size.
        @param max_size: Maximum number of sites in the population (implies that all the 'max_size'
        sites have been instantiated before running properly the simulation)
        @type max_size: C{int}
        @param dist: Vector of C{float}. Cumulative sum must be 1.0.
        @type dist: C{list} 
        @param model_sites: Vector of models / template sites - See L{PyLabSiteBase}. Cumulative sum must be 1.0.
        @type model_sites: C{list} 
        @param draw_progress: If the progress bar must be displayed while populating.
        @type draw_progress: C{bool} 
        """
        cdef unsigned int i
        
#        cdef vector[double] c_dist
#        for i in xrange(len(dist)):
#            c_dist.push_back(dist[i])
            
        cdef vector[LabSiteBase *] c_model_sites
        cdef PyLabSiteBase p_site
        for i in xrange(len(model_sites)):
            p_site = model_sites[i]
            p_site.CySetSiteModelType(type(p_site).__name__)
            c_model_sites.push_back(<LabSiteBase *>(p_site.thisptr))
            
        self.thisptr.Populate(max_size, dist, c_model_sites, draw_progress)
        self.setup_pyobjects()
        
        
    cpdef Populate2(self, int max_size, vector[int] dist, object model_sites, bint draw_progress):
        """
        Populate1(self, max_size, dist, model_sites, draw_progress)
        Populate with exactly the given amounts given by dist and the models.
        
        Note : dist and model_sites must be the same size.
        @param max_size: Maximum number of sites in the population (implies that all the 'max_size'
        sites have been instantiated before running properly the simulation).
        @type max_size: C{int}
        @param dist: Vector of C{int}. Cumulative sum must be the maximum population size.
        @type dist: C{list} 
        @param model_sites: Vector of models / template sites - See L{PyLabSiteBase}. Cumulative sum must be 1.0.
        @type model_sites: C{list} 
        @param draw_progress: If the progress bar must be displayed while populating.
        @type draw_progress: C{bool} 
        """
        cdef unsigned int i
        
#        cdef vector[int] c_dist
#        for i in xrange(len(dist)):
#            c_dist.push_back(dist[i])
            
        cdef vector[LabSiteBase *] c_model_sites
        cdef PyLabSiteBase p_site
        for i in xrange(len(model_sites)):
            p_site = model_sites[i]
            p_site.CySetSiteModelType(type(p_site).__name__)
            c_model_sites.push_back(<LabSiteBase *>(p_site.thisptr))
            
        self.thisptr.Populate(max_size, dist, c_model_sites, draw_progress)
        self.setup_pyobjects()
        

    cpdef BeginSetSites(self, int max_size):
        """
        BeginSetSites(self, max_size)
        Starts populating with a given number of sites.
        
        WARNING : This will reset all the sets that have been set earlier.
        Must be call before to start adding sites with L{SetOneSiteAt()}.
        @param max_size: Maximum number of sites in the population (including empty sites).
        @type max_size: C{int}
        """
        self.thisptr.BeginSetSites(max_size)

    cpdef SetOneSiteAt(self, PyLabSiteBase a_site, int index):
        """
        SetOneSiteAt(self, a_site, node_index)
        Adds a single site to the sets manager.                                                              
        A call to this function must be enclosed between L{BeginSetSites()} and L{EndSetSites()} calls.
        
        WARNING : All sites B{must} be set before ending - See L{EndSetSites()}.                                   
        @param a_site: Site to be set.
        @type a_site: L{PyLabSiteBase} 
        @param node_index: Index at which the Site is to be set (regarding the ordered list of this population).
        See also L{PyLabSiteSetsCalculator.GI()}.
        @type node_index: C{int} 
        """
        self.thisptr.SetOneSiteAt(<LabSiteBase *>a_site.thisptr, index)
            
    cpdef EndSetSites(self):
        """
        EndSetSites(self)
        Ends populating sites operation. Checks if all sites have been set. If not, terminates the program.  
        All 'max_size' sites must have been set before ending. See L{BeginSetSites(max_size)}.             
        """
        self.thisptr.EndSetSites()
        self.setup_pyobjects()


    # TODO : Untested
    cpdef CopySite(self, PyLabSiteBase dest_site, PyLabSiteBase src_site):
        """
        TODO : UNTESTED !
        """
        self.thisptr.CopySite(<LabSiteBase *>dest_site.thisptr, <LabSiteBase *>src_site.thisptr)
    # TODO : Untested
    cpdef SwapSite(self, PyLabSiteBase l_site, PyLabSiteBase r_site):
        """
        TODO : UNTESTED !
        """
        self.thisptr.SwapSite(<LabSiteBase *>l_site.thisptr, <LabSiteBase *>r_site.thisptr)
        


