/*
 * Copyright (c) 2008, Thomas J. Hladish and Eugene Melamud
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the names of the copyright holders nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Slightly modified by Gauthier BOAGLIO - CNRS - 2012-09-13
 * See main LICENSE file terms for details
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <iterator>
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <limits>

#include "Utility.h"
//#include "MersenneTwister.h"
#include "debug.h"

//// Cython !!
#ifndef NOPYTHON
	#include <Python.h>
#endif

#include <LabConsts.h>
#include <LabUserData.h>

using namespace std;


namespace elps {

//predeclare classes
class LabEdge;
class LabNode;
typedef int stateType;

// GLG HACK : added handling for user additional data
//typedef void* t_user_data;
//typedef LabUserData* t_user_data;



/***************************************************************************
 *
 * BEGIN NETWORK CLASS
 *
 **************************************************************************/

/**
 * Those classes have a natural hierarchy of LabNetwork > LabNode > LabEdge.<br/>
 * That means you probably should not be doing something with nodes unless they already
 * belong to a network, and you probably shouldn't be handling edges unless
 * they already belong to at least one node (and a network).
 *
 * There are many ways you can construct a network, but they fall into two
 * categories: automated (and probably randomized) and manual.
 *
 * USAGE :
 * =======
 *
 * Start out by getting yourself a network :
 *
 * \code
 * 	LabNetwork my_network("name", false);    // "name" is any string you want, and "false"
 * 	                                         // means network is undirected (true = directed).
 * \endcode
 *
 * If you want to automate the construction :
 *
 * \code
 * 	my_network.Populate(10000);              // Add 10^4 nodes to the network.
 * 	my_network.RandConnectPoisson(4);        // Connect the nodes using a Poisson(4) degree
 * 	                                         // distribution and get rid of self-loops and
 * 	                                         // multi-edges (done automatically).
 * \endcode
 *
 */
class LIB_CLASS LabNetwork
{
    static int id_counter;       // remains in memory until end of the program
    static MTRand mtrand;        // random number generator
    friend class LabNode;
    friend class LabEdge;

    public:
        typedef enum { Undirected=0, Directed=1 } netType;

        /***************************************************************************
         * LabNetwork Constructor and Destructor
         **************************************************************************/

        // GLG HACK : Added default constructor to allow derivation
        LabNetwork();
        //// Cython !!!
#ifndef NOPYTHON
        LabNetwork(PyObject *obj);
#endif


        void Init(string name, netType directed);

        LabNetwork( string name, netType directed );
        LabNetwork( const LabNetwork& net);
        ~LabNetwork();

        LabNetwork* duplicate();    // Return a copy, identical except for the network ID

        /***************************************************************************
         * LabNetwork operators
         **************************************************************************/
                                 // Test whether two networks are
        bool operator==( const LabNetwork& n2 );
                                 // actually the same object

        /***************************************************************************
         * LabNetwork Accessor Functions (Simple)
         **************************************************************************/
        inline int              get_id() { return id; }
        inline string           get_name() { return name; }
                                 // # of nodes in the network
        inline int              size() {
            return node_list.size();
        }
                                 // do all edges have same length or cost
        inline bool             has_unit_edges() {
            return unit_edges;
        }
        inline bool             is_directed() {return (bool) directed; }
                                 // get a pointer to the random number generator
        inline MTRand*          get_rng() {
            return &mtrand;
        }

        /***************************************************************************
         * LabNetwork Accessor Functions
         **************************************************************************/
                                 // get all nodes
        inline vector<LabNode*>& get_nodes() {
            return node_list;
        }
                                 // get a particular node
        LabNode*                get_node(int node_id);
                                 // get a random node
        LabNode*                get_rand_node();
                                 // get all edges
        vector<LabEdge*>        get_edges();
                                 // get a particular edge
        LabEdge*                get_edge(int id);

        // GLG NOTE : better use the Get/SetUserData instead
        // (allows any level of complexity of what information to store in a LabNode)
                                 // get states of all nodes
        vector<stateType>    get_node_states();

        // get edges that loop back to the same node, or result in
        // redundant connections between all nodes A and B.  NB: To use
        // this, pass in two empty LabEdge* vectors; they will be populated
        // with the problematic edges inside the function
        void get_bad_edges(vector<LabEdge*> &self_loops, vector<LabEdge*> &multiedges);

        vector<LabNode*> get_component(LabNode* node);  // get the component this node is in
        vector< vector<LabNode*> > get_components(); // get all components
        vector<LabNode*> get_biggest_component();
        
        inline bool topology_altered() { return _topology_altered; }
        // vector< vector<LabNode*> > get_components(){};

        /***************************************************************************
         * LabNetwork Modifier Functions
         **************************************************************************/
        LabNode* add_new_node();    //creates new node and adds it to the network
        void populate(int n);    //add "n" new nodes to the network
                                 // add an existing node to the network
        void add_node( LabNode* node );
                                 // delete a specified node (deleting associated edges)
        void delete_node( LabNode* node);

        // erdos_renyi() and rand_connect_poisson() (should) produce equivalent
        // networks, although the former will always allow degree 0 nodes, while
        // the later can be modified to use a left-truncated distribution.
        //
        // Erdos-Renyi can only produce Poisson distributed networks, whereas
        // all the rand_connect* functions use an adaptation of the Molloy-Reed
        // algorithm, and therefore can take any discrete, non-negative distribution.
        bool erdos_renyi(double lambda);
        bool sparse_random_graph(double lambda);
        //fast_random_graph() tries to pick the fastest algorithm based on parameters given
        bool fast_random_graph(double lambda);
        // Ring lattice with N nodes, each connected to K nearest neighbors
        bool ring_lattice(int N, int K);
        // RxC lattice, including diagonals if diag
        bool square_lattice(int R, int C, bool diag);
        //Watts-Strogatz small world network with N nodes initially connected to K neighbors
        // and shuffled with probability beta
        bool small_world(int N, int K, double beta);
        bool rand_connect_poisson(double lambda);
        bool rand_connect_powerlaw(double alpha, double kappa);
        bool rand_connect_exponential(double lambda);
        //void rand_connect_user(map<int,double>); not implemented

        // User provides arbitrary (normalized!) distribution.  normalize_dist(my_dist)
        // in Utility.h can be used for that.  Each value should be the probability
        // of the index (index = degree), i.e. if the contents of the vector are
        // (0, 0.13, 0.2, 0.04, 0.63), then the probability of drawing a deviate (=degree)
        // of 0 is 0, of 1 is 13%, of 2 is 20%, and so on.
        bool rand_connect_user(vector<double> dist);

        // User provides an explicit degree series, one degree per node. Sum of
        // all degrees should be even, or else it's impossible to connect all nodes
        bool rand_connect_explicit(vector<int> deg_series);

        // You probably don't want this, unless you are manually creating stubs for
        // each node.  It is likely easier for you to call one of the other
        // rand_connect* functions that takes either a distribution or
        // distribution parameters.
        bool rand_connect_stubs(vector<LabEdge*> stubs);

        // Gets rid of self-loops and multi-edges.  This is called automatically
        // by erdos_renyi() and all of the rand_connect* functions.  You only
        // need it if you are using your own algorithm to connect nodes.
        bool lose_loops();

        // Unpopulate the network.
        void clear_nodes();      // { for (int i = 0; i < size(); i++) delete node_list[i]; }

        // Completely disconnect network.  Degree of every node goes to zero, but things like
        // the node's id, name, state stay the same.
        void clear_edges();

        // Disconnect network, but don't delete edges--leave them as stubs (edges
        // with starting nodes but no ending nodes).  You'll probably want to
        // follow a call to disconnect_edges() with rand_connect_stubs() to
        // reconnect those edges in a new, randomized way.
        void disconnect_edges();

        // Randomly rewire a fraction of the edges in the network.  Currently only
        // undirected networks are supported.
        bool shuffle_edges(double frac);

        // GLG NOTE : better use the Get/SetUserData instead
        void set_node_states(vector<stateType> &states);
        
        inline void set_topology_altered(bool flag) { _topology_altered = flag; }

        /***************************************************************************
         * LabNetwork Input/Output (including visualization)
         **************************************************************************/
                                 // read network structure from file
        void read_edgelist(string filename, char sep = ',');
                                 // write network to file
        void write_edgelist(string filename);

        /*                         // read network structure from file
        void read_adj_matrix(string filename, char sep = ',');
                                 // write network to file
        void write_adj_matrix(string filename);*/

                                 // output a graphviz file
        void graphviz(string filename);
        void dumper();           // print the network object contents in the terminal

        /***************************************************************************
         * LabNetwork Properties
         **************************************************************************/
        bool gen_deg_series(vector<int> &deg_series);
        vector<int> get_states();// get the states of all nodes
                                 // get the state sequences, indexed by degree
        vector< vector<int> > get_states_by_degree();

        bool validate();

                                 // list of degrees, one for each node
        vector<int> get_deg_series ();
                                 // counts of 0 deg nodes, 1 deg nodes, ...
        vector<int> get_deg_dist ();
                                 // dist to sample to draw new degrees (frequencies)
        vector<double> get_gen_deg_dist ();
        double mean_deg();       // calculated mean of the degree series
                                 // measure of clustering of nodes in node_set;
        double transitivity(vector<LabNode*> node_set);
                                 // if node_set is empty, use all nodes
        double mean_dist( vector<LabNode*> node_set);      // mean distANCE between all nodes A and B
                                 // 2D matrix of distances
                               
                                 // distances == edge costs
        vector< vector<double> > calculate_distances( vector<LabNode*> destinations );
                                 // edge lengths assumed to be 1
        vector< vector<double> > calculate_unweighted_distances( vector<LabNode*> destinations );



        //// Cython !!!
#ifndef NOPYTHON
        PyObject *CyGetPyObj();
        void CySetPyObj(PyObject *obj);
#endif


        ////////////////////////////////////////////////////////////////////
        //			GLG : FOR SHORT (only externally useful features)
        ////////////////////////////////////////////////////////////////////

        /**
         * Get the size of the network.
         * @return A number of nodes.
         */
    	int GetSize();
    	/**
    	 * Populate the network with n nodes.
    	 * @param n Number of nodes.
    	 */
    	void Populate(int n);
    	/**
    	 * Get the network's nodes list
    	 * @return A vector of LabNode pointers
    	 */
    	vector<LabNode*>& GetNodes();
    	/**
    	 * Pick up a node by its id in the nodes list.
    	 * @param node_id Identifier in the nodes list.
    	 * @return
    	 */
    	LabNode *GetNode(int node_id);

        /**
         * ErdosRenyi() and RandConnectPoisson() (should) produce equivalent
         * networks, although the former will always allow degree 0 nodes, while
         * the later can be modified to use a left-truncated distribution.<br/>
         *
         * Erdos-Renyi can only produce Poisson distributed networks, whereas
         * all the RandConnect* functions use an adaptation of the Molloy-Reed
         * algorithm, and therefore can take any discrete, non-negative distribution.
    	 *
    	 * @param lambda
    	 * @return
    	 */
        bool ErdosRenyi(double lambda);
        /**
         * Sparse distribution.
         * @param lambda
         * @return
         */
        bool SparseRandomGraph(double lambda);
        /**
         * FastRandomGraph() tries to pick the fastest algorithm based on parameters given.
         *
         * @param lambda
         * @return
         */
        bool FastRandomGraph(double lambda);
        /**
         * Ring lattice with N nodes, each connected to K nearest neighbors.
         *
         * @param lambda
         * @return
         */
        bool RingLattice(int N, int K);
        /**
         * RxC lattice, including diagonals if diag is True.
         *
         * @param R Number of rows.
         * @param C Number of columns.
         * @param diag If the diagonal should be connected.
         * @return
         */
        bool SquareLattice(int R, int C, bool diag);
        /** Watts-Strogatz small world network with N nodes initially connected to K neighbors
         * and shuffled with probability beta.
         *
         * @param N
         * @param K
         * @param beta
         * @return
         */
        bool SmallWorld(int N, int K, double beta);

        /**
         * User provides arbitrary (normalized!) distribution.  normalize_dist(my_dist)
         * in Utility.h can be used for that.  Each value should be the probability
         * of the index (index = degree), i.e. if the contents of the vector are
         * (0, 0.13, 0.2, 0.04, 0.63), then the probability of drawing a deviate (=degree)
         * of 0 is 0, of 1 is 13%, of 2 is 20%, and so on.
         *
         * @param dist
         * @return
         */
        bool RandConnectUser(vector<double>& dist);

        // GLG : added for short version of RandConnectUser()
        /**
         * Same as RandConnectUser(), but specifying only the connection degree.<br/>
         * The algorithm takes care of computing linearly the intermediate values. Sum of
         * all degrees should be even, or else it's impossible to connect all the nodes.
         * @param con_deg
         * @return
         */
    	bool RandConnectUserLinear(int con_deg);
        /**
         * User provides an explicit degree series, one degree per node. Sum of
         * all degrees should be even, or else it's impossible to connect all nodes.
         * @param deg_series
         * @return
         */
        bool RandConnectExplicit(vector<int>& deg_series);
    	/**
         * Same as RandConnectUser() using a Poisson distribution.
         * @param lambda
         * @return
         */

        bool RandConnectPoisson(double lambda);
        /**
         * Same as RandConnectUser() using a Powerlaw distribution.
         * @param alpha
         * @param kappa
         * @return
         */
        bool RandConnectPowerlaw(double alpha, double kappa);
        /**
         * Same as RandConnectUser() using an Exponential distribution.
         * @param lambda
         * @return
         */
        bool RandConnectExponential(double lambda);

        // GLG : additional dynamic (migrating) network feature
        /**
         * Migrates the given fraction of Nodes in the network by rearranging their Edges.<br/><br/>
         * Algorithm details :<br/>
         * At each call (period), (fraction_to_migrate * 100) % of all nodes "migrate".
         * A different group is randomly selected each period. If a nodes is in the migrating
         * group, all of its edges are broken; this means the node's neighbors also
         * end up with at least one broken edge. These broken edges ("stubs") are
         * then randomly reconnected. The reconnection process will make sure that
         * no self-loops or parallel edges get created (it's possible that
         * additional edges will get shuffled in order to get rid of these).
         *
         * @param fraction_to_migrate The fraction [0.0, 1.0] of nodes to be reorganized.
         */
        void MigrateNodes(double fraction_to_migrate);


        /***************************************************************************
         * Process status & control
         **************************************************************************/
        // Allows outside control of terminating some long-running network processes
        void stop_processing() { process_stopped = true; }
        void reset_processing_flag() { process_stopped = false; }

    private:

        //// Cython !!!
#ifndef NOPYTHON
        PyObject *m_obj;
#endif

        bool is_stopped();       // checks process status, resets to false if true
        int id;                  // unique id for the node
        string name;
        vector<LabNode*> node_list;
        bool unit_edges;
        netType directed;
                                 // the generating distribution
        vector<double> gen_deg_dist;
                                 // used to draw random degrees

        bool _topology_altered;  //has the network topology changed?
        int node_id_counter;
        int edge_id_counter;
        bool _assign_deg_series();
        
        // The network has no stubs, but gen_deg_dist (a normalized degree distribution
        // to draw deviates from) has already been stored.
        bool _rand_connect();

        // This is checked during some long-running processes to determine whether to
        // continue
        volatile bool process_stopped;
        int known_nodes; // bookkeeping var; allows get_component() to report % complete

};

/**
 * The node is the base entity constituting a Network.
 * Those classes have a natural hierarchy of LabNetwork > LabNode > LabEdge.
 *
 * A PyLabNode is a node of the LabNetwork (they are link between each other by edges).
 * Each node can carry an object / site (of any kind, user is free to choose a custom
 * representation, but it is recommended to use existing LabSiteBase, LabSiteEvent
 * or at least sub-classes of LabSiteBase).
 *
 * USAGE :
 * =======
 *
 * A node cannot exist without a network (See LabNetwork first). Once your network
 * is populated of nodes, each node has to be setup. There is two ways of doing this :
 *
 * <B>Manually :</B>
 *
 * \code
 * 	LabSiteBase *a_site = new LabSiteBase(sim);         // Create an object to be stored in the node
 * 	a_node.SetUserData(a_site);                         // Assign this object to the node
 * \endcode
 *
 * There is also more <B>automated ways</B> which come with the LabNetBinding available
 * via the "netBinding" member of all <B>*Net</B> versions of the simulator (See, for example,
 * the LabSimulatorTimeNet). See LabNetBinding, for a deeper description of
 * those methods.
 *
 * Moreover those <B>automated ways</B> give access to more advanced features (which are
 * not available while proceeding <B>manually</B>) : the Sets related features (See
 * LabSiteSetsCalculator and LabSiteSetsManager for getting a full understanding of
 * the <B>Sets based</B> system).
 *
 */
class LIB_CLASS LabNode
{
    friend class LabNetwork;
    friend class LabEdge;
    
    public:

        inline bool is_stopped() {return network->is_stopped();}

        /***************************************************************************
         * Constructor and Destructor
         **************************************************************************/
        //Use LabNetwork::add_new_node() to add nodes
        void delete_node();

        void set_network( LabNetwork* network );

        inline int get_id() { return id; }
        inline string get_name() {  return name; }
        inline LabNetwork* get_network() { return network; }
        inline vector<LabEdge*> get_edges_in() { return edges_in; }
        inline vector<LabEdge*> get_edges_out() { return edges_out; }
        inline vector<double> get_loc() { return loc; }
        inline stateType get_state()   { return state; }

        inline void set_loc(const vector<double>& newloc) { this->loc = newloc; }
        // GLG NOTE : better use the Get/SetUserData instead
        inline void set_state(stateType s) { this->state = s; }

        double mean_min_path();

        // if network edge lengths can be assumed to be 1, use min_unweighted_paths()
        vector<double> min_unweighted_paths(vector<LabNode*> node_set); // infinite distances == -1
        vector<double> min_paths(vector<LabNode*> node_set); // infinite distances == -1

        void add_stubs(int deg);

        LabEdge* get_rand_edge();   // get a random outbound edge
        vector<LabNode*> get_neighbors ();
        bool is_neighbor(LabNode* node2);
                                 // a->connect_to(b) == b->connect_to(a) for undirected networks
        void connect_to (LabNode* end);
        bool change_neighbors(LabNode* old_neighbor, LabNode* new_neighbor);
        bool operator==( const LabNode& n2 );
        friend ostream& operator<< (ostream &out, LabNode* node);
        void dumper();

        double min_path(LabNode* dest);

        LabEdge* add_stub_out();
        string get_name_or_id();
        int deg();

        // GLG HACK : moved from private to protected to allow classes
        //            derived from LabNetwork to access a LabNode
        // GLG HACK : Then moved to public to allow usage from Cython
        LabNode();
        LabNode(LabNetwork* network, string name, stateType state);
        ~LabNode();
//        // GLG HACK : for use in Cython (Get a string representation of the LabNode)
//    	string ThisToString();


        /////////////////////////////////////////////////////////////////
        //			GLG : FOR SHORT (only externally useful features)
        /////////////////////////////////////////////////////////////////

        // GLG HACK : added access routines for handling user additional data
    	/**
    	 * Get user's data embedded in the LabNode
    	 * (usually a Site - See LabSiteBase and LabSiteEvent).
    	 * @return Any kind of object derived from LabUserData.
    	 */
        LabUserData * GetUserData();
        /**
         * Set the user's data to be embedded in the LabNode
    	 * (usually a Site - See LabSiteBase and LabSiteEvent).
         * @param user_data Any kind of object derived from LabUserData.
         */
        void SetUserData(LabUserData * user_data);

        //// Cython !!
#ifndef NOPYTHON
        /**
         * Cython utility. DO NOT USE !
         */
        PyObject *CyGetPyObj();
        /**
         * Cython utility. DO NOT USE !
         */
    	void CySetPyObj(PyObject *obj);

    	/**
         * Cython utility. DO NOT USE !
         */
        PyObject *CyGetUserData();
        /**
         * Cython utility. DO NOT USE !
         */
        void CySetUserData(PyObject *user_pydata);
#endif


        /**
         * Get the direct neighbors of the current LabNode.
         * @return A list of nodes.
         */
        vector<LabNode*> GetNeighbors();
        /**
         * Get a pointer on the LabNetwork this LabNode is connected to.
         * @return The parent network of this node.
         */
    	LabNetwork *GetNetwork();
    	/**
    	 * Get the distance between this LabNode and each other in the given group.
    	 * @param nodes The group of nodes to be confronted to this one.
    	 * @return The distances list.
    	 */
    	vector<double> MinPaths(vector<LabNode*>& nodes);


    protected:
        //

    private:

        // GLG HACK : ...
        //// Cython !!
#ifndef NOPYTHON
        PyObject *m_obj;
        PyObject *m_data;
#endif

        int id;                  //unique id
        string name;
        LabNetwork* network;        //pointer to network
        vector<LabEdge*> edges_in;  //vector of pointers coming in
        vector<LabEdge*> edges_out; //vector of pointers going out
        vector<double> loc;
        stateType state;
        void _add_inbound_edge (LabEdge* edge);
        void _del_inbound_edge (LabEdge* inbound);
        void _add_outbound_edge (LabEdge* edge);
        void _del_outbound_edge (LabEdge* outbound);

        // GLG HACK : user additional data that might be carried by a node
        LabUserData *userData;


};


/**
 * Those classes have a natural hierarchy of LabNetwork > LabNode > LabEdge.<br/>
 * A LabEdge defines a link between two LabNodes.
 */
class LabEdge
{
    friend class LabNetwork;
    friend class LabNode;

    public:
        /***************************************************************************
         * Constructor and Destructor
         **************************************************************************/
        ~LabEdge();
        void delete_edge();      //destroys edge (A to B), leaves complement (B to A)
        void disconnect_nodes(); //destroys edge & its complement

        inline int get_id() { return id; };
        inline double get_cost() { return cost; };
        inline LabNode* get_start() { return start; };
        inline LabNode* get_end() { return end; };
        inline LabNetwork* get_network() {return network; };

        void set_cost(double c);

        LabEdge* get_complement();
        void swap_ends (LabEdge* other_edge);
        void break_end ();
        void define_end (LabNode* end_node);
        bool is_stub();
        bool operator==( const LabEdge& e2 );
        friend ostream& operator<< (ostream &out, LabEdge* edge);
        void dumper();

    private:
        LabEdge(LabNode* start , LabNode* end);
        void _move_edge(LabNode* new_start_node);

        int id;
        double cost;
        LabNode* start;
        LabNode* end;
        LabNetwork* network;

};

}

#endif
