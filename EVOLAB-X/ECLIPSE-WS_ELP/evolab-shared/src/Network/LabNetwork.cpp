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

#include "LabNetwork.h"
#include "Utility.h"

using namespace std;

namespace elps {

////////////////////////////////////////////////////////////////////////////////
//
// Global Class Variables
//
////////////////////////////////////////////////////////////////////////////////

int LabNetwork::id_counter = 0;
MTRand LabNetwork::mtrand;          // single instance of random number generator for entire program run
// the rng can be seeded at any time by calling mtrand->seed(your_seed);

////////////////////////////////////////////////////////////////////////////////
//
// LabNetwork Class Functions
//
////////////////////////////////////////////////////////////////////////////////


LabNetwork::LabNetwork() {
	Init("NetWork Name", LabNetwork::Undirected);
}

//// Cython !!!
#ifndef NOPYTHON
LabNetwork::LabNetwork(PyObject *obj) {
	Init("NetWork Name", LabNetwork::Undirected);
	this->CySetPyObj(obj);
}
#endif


LabNetwork::LabNetwork( string name, netType directed) {
//    id = LabNetwork::id_counter++;
//    this->name = name;
//    this->directed = directed;
//    this->unit_edges = 1;
//    this->node_id_counter = 0;
//    this->edge_id_counter = 0;
//    this->_topology_altered=false;
//    this->mtrand = mtrand;
//    this->process_stopped = false;
//    this->known_nodes = 0;

	Init(name, directed);
}

// GLG HACK
void LabNetwork::Init(string name, netType directed) {
    id = LabNetwork::id_counter++;
    this->name = name;
    this->directed = directed;
    this->unit_edges = 1;
    this->node_id_counter = 0;
    this->edge_id_counter = 0;
    this->_topology_altered=false;
    this->mtrand = mtrand;
    this->process_stopped = false;
    this->known_nodes = 0;
}


LabNetwork* LabNetwork::duplicate() {
    LabNetwork* dup = new LabNetwork( name, directed );
    dup->unit_edges = unit_edges;
    dup->node_id_counter = node_id_counter;
    dup->edge_id_counter = edge_id_counter;
    dup->mtrand = mtrand;

    // Make copies of all nodes
    for (int i = 0; i < size(); i++) {
        LabNode* node = node_list[i];
        LabNode* node_copy = new LabNode();
        node_copy->id   = node->get_id();
        node_copy->set_network(dup);

        // Create a stub for each outbound edge (since not all nodes have been created, we can't connect everything yet)
        node_copy->add_stubs( node->deg() );
        dup->node_list.push_back(node_copy);
    }

    // Loop through nodes again, this time defining edges
    for (int i = 0; i < size(); i++) {
        LabNode* node = node_list[i];
        LabNode* node_copy = dup->get_node( node->get_id() );

        vector<LabEdge*> edges = node->get_edges_out();
        vector<LabEdge*> edge_copies = node_copy->get_edges_out();

        for (unsigned int j = 0; j < edges.size(); j++) {
            edge_copies[j]->id    = edges[j]->get_id();
            edge_copies[j]->cost  = edges[j]->get_cost();
            edge_copies[j]->end   = edges[j]->get_end();
        }
    }
    return dup;
}


LabNetwork::~LabNetwork() {
    for (unsigned int i = 0; i < node_list.size(); i++) {
        delete node_list[i];
    }
    //delete mtrand;
}


void LabNetwork::populate( int n ) {
    for (int i = 0; i < n; i++) {
        add_new_node();
    }
}


LabNode* LabNetwork::add_new_node() {
    LabNode* node = new LabNode();     //allocate memory for new node
    node->id = node_id_counter++;
    node->set_network(this);     //set the network
    node_list.push_back(node);   //add new node to the back
    set_topology_altered(true);
    return(node);
}


void LabNetwork::delete_node(LabNode* node) {
    vector<LabNode*>::iterator itr;
    itr = find(node_list.begin(), node_list.end(), node);
    node_list.erase(itr);
    delete node;
    set_topology_altered(true);
}


LabNode* LabNetwork::get_node(int node_id) {
    assert((unsigned) node_id < node_list.size() );

    LabNode* lucky_node = node_list[node_id];
    if (lucky_node->id == node_id) {
        return lucky_node;
    }

    vector<LabNode*>::iterator itr;
    for (itr = node_list.begin(); itr < node_list.end(); itr++) {
        if ((*itr)->id == node_id) return *itr;
    }
    cout << "Couldn't find a node with id  " << node_id << endl;
    return NULL;
}


bool LabNetwork::ring_lattice(int N, int K) {
    if (K > (N - 1) / 2) {
        cout << "Cannot construct a ring lattice with K-nearest neighbors where K > (network size - 1) / 2\n";
        return false;
    }
    clear_nodes();
    populate(N);
    for (unsigned int i = 0; i < node_list.size(); i++) {
        for (int j = 1; j <= K; j++) {
            int dest = (i+j) % node_list.size();
            node_list[i]->connect_to(node_list[dest]);
        }
    }
    return true;
}


// Assumes undirected network
bool LabNetwork::square_lattice(int R, int C, bool diag) {
    if (R < 1 or C < 1) {
        cout << "Square lattice must have at least one row and one column.\n";
        return false;
    }
    clear_nodes();
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            LabNode* node = add_new_node();
            int id = node->get_id();
                                 // if we're not in the first row or column
            if (i > 0 && j > 0) {
                int up = id - C; // connect to node above
                LabNode * up_node = get_node(up);
                node->connect_to( up_node );
                int left = id - 1;
                LabNode * left_node = get_node(left);
                                 // & connect to node on left
                node->connect_to( left_node );
                if (diag) {
                    node->connect_to( get_node(up-1) );
                    left_node->connect_to( up_node );
                }
            }
            else if (i > 0) {
                int up = id - C;
                node->connect_to( get_node(up) );
            }                    // we must be in the first row
            else if (j > 0) {
                int left = id - 1;
                node->connect_to( get_node(left) );
            }
        }
    }
    return true;
}


bool LabNetwork::small_world(int N, int K, double beta) {
    if ( ring_lattice(N, K) ) {
        vector<LabNode*> nodes = get_nodes();
        for (int i=0; i<size(); i++) {
            LabNode* node = nodes[i];
            int degree = node->deg();
            // How many of node's edges will be shuffled?
            int m = rand_binomial( degree, beta, &mtrand );
            // Which edges will be shuffled?
            vector<int> edge_indeces(m);
            rand_nchoosek( degree, edge_indeces, &mtrand );
            
            vector<LabEdge*> edges = node->get_edges_out();
            for (unsigned int e=0; e<edge_indeces.size(); e++) {
                // Get the current neighbor associated with this edge
                LabNode* neighbor = edges[e]->get_end();
                
                // Make sure that there are nodes that can be connected to
                int prospective_neighborhood = N - 1 - degree;
                if (prospective_neighborhood > 0) {
                    bool success = false;
                    int attempts = 0;
                    while (! success) {
                        // Grab a node from the network
                        LabNode* prospective = get_rand_node();
                        // If it's not this node, and not a current neighbor
                        if ( prospective != node and ! node->is_neighbor(prospective) ) {
                            // then connect to it, and ditch the old neighbor
                            success = node->change_neighbors( neighbor, prospective );
                        }
                        if (++attempts > 1000) {
                            // Give up if we've tried to rewire this edge 1000 times
                            cout << "Failed to find a prospective neighbor after trying 1000 times in small world generator.\n";
                            return false;
                        }
                    }
                }
            }
        }
    } else {
        return false; // making ring lattice failed
    }
    return true;
}

// generates a poisson network vi the Erdos & Renyi algorithm
bool LabNetwork::erdos_renyi(double lambda) {
    int n = size();
    if (lambda > n-1) return false; // mean degree can't be bigger than network size - 1 
    double p = lambda / (n-1);
    vector<LabNode*> nodes = get_nodes();
    for (int a = 0; a < n - 1; a++) {
        if (is_stopped() ) { return false; }
        for (unsigned int b = a + 1; b < nodes.size(); b++) {
            if ( mtrand.rand() < p) {
                nodes[a]->connect_to(nodes[b]);
            }
        }
        //set_progress( (double) a / n );
        PROG( (int) 100 * (1 - (double)(n-a)*(n-a-1) / (n*(n-1))) );
        //cout <<  1 - (double)(n-a)*(n-a-1) / (n*(n-1))  << endl;
    }
    return true;
}


// generates a poisson network.  Faster than Erdos-Renyi for sparse graphs
bool LabNetwork::sparse_random_graph(double lambda) {
    int n = size();
    long double p = lambda / (n-1);
    long double sd = sqrtl(n*lambda*(1-p));
    //sqrtl(n*(n-1)*p*(1-p)); // sometimes yields -nan (e.g. n=50000,lambda=5)
                                 // randNorm(mean, variance)
    double edge_ct = mtrand.randNorm(lambda * n, sd);
                                 // we're increasing the degree of 2 nodes!
    for (int i = 0; i < edge_ct; i += 2) {
        int a = mtrand.randInt(n - 1);
        int b = mtrand.randInt(n - 1);
                                 // for undirected graphs, this makes
        node_list[a]->connect_to(node_list[b]);
                                 // an undirected edge
        PROG( (int) 50 * i/edge_ct );
    }
    return lose_loops();
}

bool LabNetwork::fast_random_graph(double lambda) {
    // Testing networks of 10e3, 10e4, and 10e5 nodes, I found that the break-even point
    // where the algorithms performance was comparable was when nodes were connected to
    // between 1 and 2% of the network
    if (lambda/size() > 0.01) {
        return erdos_renyi(lambda);
    } else {
        return sparse_random_graph(lambda);
    }
}

bool LabNetwork::rand_connect_poisson(double lambda) {
    int min = 0;                 // min and max are INCLUSIVE, i.e. the lowest and highest possible degrees
    int max = node_list.size() - 1;
    vector<double> dist = gen_trunc_poisson(lambda, min, max);
    return rand_connect_user(dist);
}


bool LabNetwork::rand_connect_powerlaw(double alpha, double kappa) {
    int min = 1;                 // min and max are INCLUSIVE, i.e. the lowest and highest possible degrees
    int max = node_list.size() - 1;
    vector<double> dist = gen_trunc_powerlaw(alpha, kappa, min, max);
    return rand_connect_user(dist);
}


bool LabNetwork::rand_connect_exponential(double lambda) {
    int min = 1;                 // min and max are INCLUSIVE, i.e. the lowest and highest possible degrees
    int max = node_list.size() - 1;
    vector<double> dist = gen_trunc_exponential(lambda, min, max);
    return rand_connect_user(dist);
}


/*
// This might be used if the degree distribution had only very large values or was otherwise very weird
void LabNetwork::rand_connect_user(map<int,double>) {

}*/

bool LabNetwork::rand_connect_explicit(vector<int> degree_series) {
    assert(degree_series.size() == node_list.size());
    assert(sum(degree_series) % 2 == 0);
    for (unsigned int i = 0; i < degree_series.size(); i++ ) {
        node_list[i]->add_stubs(degree_series[i]);
    }
    return rand_connect_stubs( get_edges() );
}


bool LabNetwork::rand_connect_user(vector<double> dist) {
    if (is_stopped()) return false;
    gen_deg_dist = dist;
    return _rand_connect();
}


// use this only if the generating degree dist has already been stored
bool LabNetwork::_rand_connect() {
    if (_assign_deg_series()) {
        return rand_connect_stubs( get_edges() );
    } else {
        return false;
    }
}


bool LabNetwork::rand_connect_stubs(vector<LabEdge*> stubs) {
    if ( is_stopped() ) return false;
    if ( stubs.size() == 0 ) return true;
                                 //get all edges in network
    LabEdge* m;
    LabEdge* n;

    //shuffle the vector
    shuffle(stubs, &mtrand);

    //connect stubs
    for (unsigned int i = 0; i < stubs.size() - 1; i += 2 ) {
        m  = stubs[i];
        n  = stubs[i  + 1];
        m->define_end(n->start);
        n->define_end(m->start);
        PROG( 25 + (int) (25 * i / stubs.size()) );
    }
    // if lose_loops() isn't successful, return false
    if (! lose_loops()) { clear_edges(); return false; }
    return true;
}


// This method removes self loops (edges with the same node as start and end)
// and multi-edges (e.g. pairs of edges which have identical starts and ends)
// Returns true on success, false if network could not be rewired
bool LabNetwork::lose_loops() {
    if ( is_stopped() ) return false;
                                 //all (outbound) edges in the network
    vector<LabEdge*> edges = get_edges();

    int m, n;
    int failed_attempts = 0;

    LabNode* start1;
    LabNode* start2;
    LabNode* end1;
    LabNode* end2;

    vector<LabEdge*> self_loops, multiedges;

    get_bad_edges( self_loops, multiedges);
    vector<LabEdge*> bad_edges;

    bad_edges.insert(bad_edges.begin(), self_loops.begin(), self_loops.end());
    bad_edges.insert(bad_edges.end(), multiedges.begin(), multiedges.end());
    //cout << "Bad edge count: " << bad_edges.size() << endl;

    //shuffle the vector
    int max = bad_edges.size() - 1;
    for (int i = max; i >= 0; i-- ) swap(bad_edges[i], bad_edges[ mtrand.randInt(i) ]);

    while ( bad_edges.size() > 0 ) {
        PROG( 50 + (int) (50 * (max - bad_edges.size()) / max) );
        m = bad_edges.size() - 1;
        n = mtrand.randInt(  edges.size() - 1 );
        if ( failed_attempts > 99 ) {
            cout    << "It may be impossible to equilibriate a network with these parameters--"
                << "couldn't get rid of any self-loops or multi-edges in the last 100 attempts"
                << endl;
            return false;
        }
        if ( is_stopped() ) return false;

        LabEdge* edge1 = bad_edges[m];
        LabEdge* edge2 = edges[n];

        start1 = edge1->start;
        end1   = edge1->end;
        start2 = edge2->start;
        end2   = edge2->end;
        /*
        Rule 1: End of edge 1 must not equal start of edge 2, and vice versa.  At worst, this prevents synonymous changes, and at best, it prevents some self-loops and multiedges.

        Rule 2: Start of edge 1 must not equal start of edge 2, and end of edge 1 must not equal end of edge 2.  This prevents synonymous (i.e. non-structural, edge re-numbering) changes.

        Rule 3: End of edge 1 must not have an edge that goes to start of edge 2, and vice versa.  This may be expensive to enforce because it requires looking around two nodes for every proposed change. It prevents some multiedges.
        */
        if ( end1 == start2  || end2 == start1 || start1 == start2 || end1 == end2 ) {
            failed_attempts++;
            continue;
        }

        // Rule 3
        vector<LabNode*> neighbors1 = end1->get_neighbors();
        vector<LabNode*> neighbors2 = end2->get_neighbors();
        vector<LabNode*>::iterator result1, result2;
        result1 = find( neighbors1.begin(), neighbors1.end(), start2 );
        result2 = find( neighbors2.begin(), neighbors2.end(), start1 );

        if( result1 != neighbors1.end() || result2 != neighbors2.end() ) {
            //            cout << "Broke Rule 3 " << endl ;
            failed_attempts++;
            continue;
        }

        LabEdge* edge1_comp = edge1->get_complement();
        bad_edges.pop_back();

        for ( unsigned int i = 0; i < bad_edges.size(); i++ ) {
            if ( bad_edges[i]==edge1_comp ) {
                bad_edges.erase(bad_edges.begin() + i);
                //                cout << "spliced out complement " << endl;
                break;
            }
        }

        //        cout << "swapping edges: " << edge1->id  << " " << edge2->id << endl;
        failed_attempts = 0;
        edge1->swap_ends(edge2);
    }

    //    self_loops.clear();
    //    multiedges.clear();
    //    get_bad_edges( self_loops, multiedges);
    return true;
}


void LabNetwork::get_bad_edges(vector<LabEdge*> &self_loops, vector<LabEdge*> &multiedges) {
    vector<LabEdge*> edges = get_edges();

    map< int, map <int, int> > seen_edges;

    LabEdge* edge;
    LabNode* start;
    LabNode* end;
    for(unsigned int i=0; i < edges.size(); i++ ) {
        edge = edges[i];
        start = edge->start;
        end = edge->end;
        if ( start == end ) {
            self_loops.push_back( edge );
        }
        else {
            if ( seen_edges[ start->id ][ end->id ] > 0  ) {
                multiedges.push_back(edge);
            }
            seen_edges[ start->id ][ end->id ]++;
        }
    }

    //cout <<  "get_bad_edges() " << seen_edges.size() <<  " " << self_loops.size() <<  " " << multiedges.size() << endl;
}


vector<LabNode*> LabNetwork::get_biggest_component() {
    vector<LabNode*> big_comp(0);
    vector< vector<LabNode*> > all_comp = get_components();

    for (unsigned int i = 0; i<all_comp.size(); i++) {
        if (all_comp[i].size() > big_comp.size()) big_comp = all_comp[i];
    }
    
    return big_comp;
}


vector< vector<LabNode*> > LabNetwork::get_components() {
    vector< vector<LabNode*> > components;
    vector<LabNode*> temp_comp(0);
    map<LabNode*,bool> remaining_nodes;
    PROG(0);
    for (int i = 0; i<size(); i++) remaining_nodes.insert( make_pair(get_nodes()[i], true) );

    known_nodes = 0;
    while ( known_nodes < size() ) {
        if (is_stopped()) {
            vector< vector<LabNode*> > empty;
            return empty;
        }

        map<LabNode*, bool>::iterator it;

        for (it = remaining_nodes.begin(); it != remaining_nodes.end(); ++it){
            if (it->second == true){
                LabNode* next = it->first;

                temp_comp = get_component( next );
                components.push_back(temp_comp);
                known_nodes += temp_comp.size();

                for (unsigned int i = 0; i<temp_comp.size(); i++) remaining_nodes[ temp_comp[i] ] = false;
            }
        }
    }
    return components;
}


vector<LabNode*> LabNetwork::get_component(LabNode* node) {
    vector<LabNode*> hot_nodes;
    vector<LabNode*> cold_nodes;
    map<int,int> state;          // 1 means "hot", 2 means "cold", 0 means we haven't looked at it yet

    hot_nodes.push_back(node);
    state[node->id] = 1;

    while (hot_nodes.size() > 0) {
        if (process_stopped) return cold_nodes;
        
        vector<LabNode*> new_hot_nodes;
        for (unsigned int i = 0; i < hot_nodes.size(); i++) {
            vector<LabNode*> neighbors = hot_nodes[i]->get_neighbors();
            for (unsigned int j = 0; j < neighbors.size(); j++) {
                                 // maybe you've already looked at this node
                if ( state[neighbors[j]->id] > 0 ) continue;
                state[neighbors[j]->id] = 1;
                new_hot_nodes.push_back( neighbors[j] );
            }
            state[hot_nodes[i]->id] = 2;
            cold_nodes.push_back( hot_nodes[i] );
            PROG((int) 100*((float) (known_nodes + cold_nodes.size())/size() ));
        }
        hot_nodes = new_hot_nodes;
    }
    return cold_nodes;
}


bool LabNetwork::_assign_deg_series() {
    int n = this->node_list.size();
    vector<int> deg_series(n);

    if ( ! gen_deg_series(deg_series) ||  is_stopped() ) return false;

    for (int i = 0; i < n; i++ ) {
        this->node_list[i]->add_stubs(deg_series[i]);
        PROG( (int) (25.0 * i / n));
    }
    return true;
}


bool LabNetwork::gen_deg_series(vector<int> &deg_series) {
    double dist_sum = sum(gen_deg_dist);
    if (dist_sum < 1 - 1e-14 || dist_sum > 1 + 1e-14) {
        //cout << "Sum: " << setprecision(50) << (double) sum(gen_deg_dist) << endl;
        cout << "Degree distribution does not sum to 1\n";
        return false;
    }

    for (unsigned int i = 0; i < deg_series.size(); i++ ) {
        deg_series[i] = rand_nonuniform_int(gen_deg_dist, &mtrand);
    }

    while ( sum(deg_series) % 2 == 1 ) {
        int idx = mtrand.randInt( deg_series.size() - 1 );
        deg_series[idx] = rand_nonuniform_int(gen_deg_dist, &mtrand);
    }
    return true;
}


vector<int> LabNetwork::get_deg_series () {
    vector<int> deg_series( size() );
    for (int i = 0; i < size(); i++) deg_series[i] = node_list[i]->deg();
    return deg_series;
}


vector<int> LabNetwork::get_deg_dist () {
    vector<int> series = get_deg_series();
    vector<int> deg_dist(max_element(series) + 1);
    for (unsigned int i = 0; i < series.size(); i++) deg_dist[ series[i] ]++;
    return deg_dist;
}


vector<double> LabNetwork::get_gen_deg_dist () {
    // generating (idealized) degree distribution, if one exists
    if (gen_deg_dist.size() == 0) {
        vector<int> deg_dist = get_deg_dist();
        int Sum = sum(deg_dist);
        gen_deg_dist = normalize_dist(deg_dist, Sum);
    }
    return gen_deg_dist;
}


double LabNetwork::mean_deg () {
    return mean(get_deg_series() );
}


vector<int> LabNetwork::get_states() {
    vector<int> states(size());
    vector<LabNode*> nodes = get_nodes();
    for (unsigned int i = 0; i < nodes.size(); i++) {
        states[i] = nodes[i]->get_state();
    }
    return states;
}


// For each degree, get the series of states
// E.g. states_by_degree[4] would be the vector of states for nodes w/ deg=4
vector< vector<int> > LabNetwork::get_states_by_degree() {
    vector<int> deg_dist = get_deg_dist();
    vector< vector<int> > states_by_degree(deg_dist.size());
    for (unsigned int i=0; i<states_by_degree.size(); i++ ) {
        vector<int> states(deg_dist[i], 0);
        states_by_degree[i] = states;
    }
    vector<int> counter(deg_dist.size(), 0);
    vector<LabNode*> nodes = get_nodes();
    for ( unsigned int i = 0; i < nodes.size(); i++ ) {
        int deg   = nodes[i]->deg();
        states_by_degree[deg][counter[deg]] = nodes[i]->get_state();
        counter[deg]++;
    }
    return states_by_degree;
}


double LabNetwork::transitivity (vector<LabNode*> node_set) {
    if (node_set.size() == 0) node_set = node_list;
    int triangles = 0;
    int tripples  = 0;
    LabNode *a, *b, *c;

    for (unsigned int i = 0; i < node_list.size(); i++) {
        if (is_stopped()) return -1 * std::numeric_limits<float>::max();
        a = node_list[i];
        vector<LabNode*> neighborhood_a = a->get_neighbors();
        for (unsigned int j = 0; j < neighborhood_a.size(); j++) {
            b = neighborhood_a[j];
            vector<LabNode*> neighborhood_b = b->get_neighbors();
            for (unsigned int k = 0; k < neighborhood_b.size(); k++) {
                c = neighborhood_b[k];
                if ( c == a ) continue;
                if ( c->is_neighbor(a) ) triangles++;
                tripples++;
            }
        }
        PROG(100*i/size());
    }
    return (double) triangles / (double) tripples ;
}


double LabNetwork::mean_dist(vector<LabNode*> node_set) {    // average distance between nodes in network
    if (node_set.size() == 0) node_set = node_list;
    vector< vector<double> > distance_matrix = calculate_distances(node_set);
    double grand_total = 0;
    for( unsigned int i=0; i < distance_matrix.size(); i++ ) {
        grand_total += sum(distance_matrix[i]);
    }
    double mean = grand_total / ( size()*( size()-1 ) ); // don't consider distance from nodes to themselves
    return mean;
}

// if node_set is not provided, default is all nodes.  node_set would generally be
// all nodes within a single component
vector< vector<double> > LabNetwork::calculate_unweighted_distances(vector<LabNode*> node_set) {
    if (node_set.size() == 0) node_set = node_list;
    vector< vector<double> > dist( node_set.size() );
    for(unsigned int i = 0; i < node_set.size(); i++ ) {
        if (is_stopped() ) {
            vector< vector<double> > empty;
            return empty;
        }
        PROG(100*(i-1)/node_set.size());
            dist[i] = node_set[i]->min_unweighted_paths(node_set);
    }
    return dist;
}



// if node_set is not provided, default is all nodes.  node_set would generally be
// all nodes within a single component
vector< vector<double> > LabNetwork::calculate_distances(vector<LabNode*> node_set)  {
    if (node_set.size() == 0) node_set = node_list;
    vector< vector<double> > dist( node_set.size() );
    for(unsigned int i = 0; i < node_set.size(); i++ ) {
        if (is_stopped() ) {
            vector< vector<double> > empty;
            return empty;
        }
        PROG(100*(i-1)/node_set.size());
            dist[i] = node_set[i]->min_paths(node_set);
    }
    return dist;
}


LabEdge* LabNetwork::get_edge(int edge_id) {
    for (unsigned int i=0; i< node_list.size(); i++ ) {
        vector<LabEdge*> edges = node_list[i]->edges_out;
        for(unsigned int j=0; j < edges.size(); j++ ) {
            if ( edges[j]->id == edge_id ) {
                return(edges[j]);
            }
        }
    }
    cout << "Coundn't find an edge with id " << edge_id << endl;
    return NULL;
}


vector<LabEdge*> LabNetwork::get_edges() {
    vector<LabEdge*> all_edges;
    for (unsigned int i=0; i< node_list.size(); i++ ) {
        vector<LabEdge*> edges = node_list[i]->edges_out;
        for(unsigned int j=0; j < edges.size(); j++ ) {
            all_edges.push_back(edges[j]);
        }
    }
    return(all_edges);
}


LabNode* LabNetwork::get_rand_node() {
    int max = node_list.size() - 1;
    return node_list[ mtrand.randInt(max) ];
}


// Unpopulate the network.
void LabNetwork::clear_nodes() {
    for (int i = 0; i < size(); i++) delete node_list[i];
    node_list.clear();
    set_topology_altered(true);
}


// Completely disconnect network.  Degree of every node goes to zero, but things like
// the node's id, name, state stay the same.
void LabNetwork::clear_edges() {
    for (int i = 0; i < size(); i++) {
        vector<LabEdge*> edges = node_list[i]->edges_out;
        for (unsigned int j = 0; j < edges.size(); j++ ) delete edges[j];
        node_list[i]->edges_out.clear();
        node_list[i]->edges_in.clear();
    }
    set_topology_altered(true);
}


// Disconnect network, but don't delete edges--leave them as stubs (edges
// with starting nodes but no ending nodes).  You'll probably want to
// follow a call to disconnect_edges() with rand_connect_stubs() to
// reconnect those edges in a new, randomized way.
void LabNetwork::disconnect_edges() {
    for (int i = 0; i < size(); i++) {
        vector<LabEdge*> edges = node_list[i]->edges_out;
        for (unsigned int j=0; j< edges.size(); j++ ) edges[j]->break_end();
    }
    set_topology_altered(true);
}


bool LabNetwork::shuffle_edges(double frac) {
    vector<LabEdge*> stubs;
    vector<LabEdge*> edges = get_edges();
    if (is_directed()) {
        cout << "Shuffling edges not implemented for directed networks.\n";
        exit(1);
    } else {
        vector< pair <LabEdge*, LabEdge*> > edge_pairs;

        for (unsigned int e = 0; e < edges.size(); e++) {
            LabEdge* edge = edges[e];
            LabEdge* comp = edge->get_complement();
            if ( edge->id < comp->id ) {
                pair<LabEdge*, LabEdge*> edge_pair(edge, comp);
                edge_pairs.push_back(edge_pair);
            }
        }
        int num_pairs_to_shuffle = (int) (frac * edges.size()/2 + 0.5); // rounding instead of truncating
        vector<int> sample(num_pairs_to_shuffle);
        rand_nchoosek((int) edge_pairs.size(), sample, &mtrand);

        for (unsigned int i = 0; i < sample.size(); i++) {
            LabEdge* edge1 = edge_pairs[ sample[i] ].first;
            LabEdge* edge2 = edge_pairs[ sample[i] ].second;
            edge1->break_end();
            edge2->break_end();
            stubs.push_back(edge1);
            stubs.push_back(edge2);
        }
    }
    return rand_connect_stubs( stubs );
}


void LabNetwork::set_node_states(vector<stateType> &states) {
    if ((signed) states.size() != size()) {
        cout << "Error in LabNetwork::set_node_states(): Vector of node states has size " << states.size() << " but there are " << size() << " nodes in network.\n";
    }
    else {
        for (int i = 0; i < size() ; i++) {
            node_list[i]->set_state( states[i] );
        }
    }
}


vector<stateType> LabNetwork::get_node_states() {
    vector<stateType> states;
    for (int i = 0; i < size() ; i++) {
        states.push_back( node_list[i]->get_state() );
    }
    return states;
}


void LabNetwork::dumper() {
    cout << "Dumping network:\n";
    cout << "name => " << name << "\n";
    cout << "id => " << id << "\n";
    cout << "n => " << node_list.size() << endl;
    cout << "directed => " << directed  << endl;
    cout << "nodes => \n";
    vector <LabNode*>::iterator itr;
    for ( itr = node_list.begin(); itr != node_list.end(); itr++ ) (*itr)->dumper();
}


bool LabNetwork::validate() {
    int net_error = 0;
    for (int i = 0; i < size(); i++) {
        LabNode* node = node_list[i];
        if (node == NULL) { cout << "node with index " << i << " is undefined (NULL)" << endl; net_error = true; continue;}
        vector<LabEdge*> in = node->edges_in;
        vector<LabEdge*> out = node->edges_out;
        if (in.size() != out.size() && directed == false) cout << "node" << node << ": Number of edges in does not match number out: " << in.size() << ", " << out.size() << endl;
        map<int, int> seen;
        int error_ct = 0;
        for (unsigned int j = 0; j < in.size(); j++) {
            LabEdge* edge = in[j];
            LabNode* start = edge->start;
            LabNode* end = edge->end;
            if (edge->is_stub())  {cout << "node" << node << ": Found edge in edges_in vector that has NULL endpoint: edge" << edge << endl; error_ct++;}
            if (start == NULL)  {cout << "node" << node << ": Found edge in edges_in vector that has NULL startpoint: edge" << edge << endl; error_ct++;}
            if (end != NULL && end->id != node->id) {cout << "node" << node << ": Found edge in edges_in vector that does not end at this node: edge" << edge << ", end: " << end << endl; error_ct++;}
            if (start != NULL && end != NULL && end == start) {cout << "node" << node << ": Found self-loop in edges_in vector: edge" << edge << endl; error_ct++;}
            if (start != NULL && seen[start->id]++ != 0) {cout <<  "node" << node << ": Found " << seen[start->id]  << " multi-edges from node" << start << endl; error_ct++;}
        }
        for (unsigned int j = 0; j < out.size(); j++) {
            LabEdge* edge = out[j];
            LabNode* start = edge->start;
            LabNode* end = edge->end;
            if (start == NULL)  {cout << "node" << node << ": Found edge in edges_out vector that has NULL startpoint: edge" << edge << endl; error_ct++; continue;}
            if (start->id != node->id) {cout << "node" << node << ": Found edge in edges_out vector that does not start at this node: edge" << edge << ", start: " << start << endl; error_ct++;}
            if (edge->is_stub())  {cout << "node" << node << ": Found stub in edges_out vector that has NULL endpoint: edge" << edge << endl; error_ct++;}
            if (start != NULL && end != NULL && end == start) {cout << "node" << node << ": Found self-loop in edges_in vector: edge" << edge << endl; error_ct++;}
            if (end != NULL && --seen[end->id] < 0) {cout <<  "node" << node << ": Found edge in edges_out vector that is not a complement of an edge in edges_in vector: edge" << edge << endl; error_ct++;}
        }
        if (error_ct > 0) {
            node->dumper(); cout << "node" << node << ": Found " << error_ct << " error(s).\n\n";
            net_error += error_ct;
        }
    }
    if ( net_error == 0) {
        cout << "LabNetwork PASSED validation.\n";
        return true;
    }
    cout << "LabNetwork FAILED validation.\n";
    return false;
}


// read_edgelist currently supports only undirected networks
void LabNetwork::read_edgelist(string filename, char sep) {

    //cout << "Loading " << filename << endl;
    ifstream myfile(filename.c_str());
    std::stringstream ss;
    map<string,LabNode*> idmap;

    if (myfile.is_open()) {
        string line;

        while ( getline(myfile,line) ) {
            //split string based on "," and store results into vector
            vector<string> fields;
            split(line, sep, fields);
            const char whitespace[] = " \n\t\r";

            //format check
            if (fields.size() > 2 ) {
                cout << "Skipping line: too many fields: " << line << endl;
                continue;
            } else if (fields.size() == 1) {
                LabNode* node = this->add_new_node();
                string name1 = strip(fields[0],whitespace);
                cout << "Found single node " << name1 << endl;
                node->name = name1;
                idmap[name1] = node;
                continue;
            } else if (fields.size() < 1) {
                continue;
            } else { // there are exactly 2 nodes

                string name1 = strip(fields[0],whitespace);
                string name2 = strip(fields[1],whitespace);

                //cout << line << endl;
                //if(idmap.count(name1)) cout << name1 << " " << idmap[name1] << endl ;
                //if(idmap.count(name2)) cout << name2 << " " << idmap[name2] << endl ;
                //cout << "---" << endl;

                //new node;
                if(idmap.count(name1)==0) {
                    //allocate memory for new node
                    LabNode* node = this->add_new_node();
                    node->name = name1;
                    idmap[name1] = node;
                }

                //new node;
                if(idmap.count(name2)==0) {
                    //allocate memory for new node
                    LabNode* node = this->add_new_node();
                    node->name = name2;
                    idmap[name2]=node;
                }

                idmap[name1]->connect_to(idmap[name2]);
                //LabNode *n1 = idmap[name1];
                //LabNode *n2 = idmap[name2];
                //n1->connect_to(n2);
            }
        }
    }
    //dumper();
    //cout << "finished dumping network\n";
    //validate();
}


void LabNetwork::write_edgelist(string filename) {
    if (filename == "") filename = "edgelist.out";

    ofstream pipe(filename.c_str(), ios::out);
    //vector<LabEdge*> edges = get_edges();
    //for (unsigned int i = 0; i < edges.size(); i++) {
    vector<LabEdge*> edges;
    for (unsigned int i = 0; i < node_list.size(); i++) {
        edges = node_list[i]->get_edges_out();
        for (unsigned int e = 0; e < edges.size(); e++) {
            int start_id = edges[e]->start->id;
            int end_id   = edges[e]->end->id;
            if (!is_directed() and start_id > end_id) continue;
            if (!is_directed() and start_id == end_id) {
                LabEdge* comp = edges[e]->get_complement();
                if (edges[e]->id > comp->id) continue;
            }
            pipe << start_id << "," << end_id << endl;
        }
        if (node_list[i]->deg() == 0) pipe << node_list[i]->id << endl;
    }
    pipe.close();

}

/*
void LabNetwork::read_adj_matrix(string filename, char sep) {

    cout << "Loading " << filename << endl;
    ifstream myfile(filename.c_str());
    std::stringstream ss;
    map<string,LabNode*> idmap;

    if (myfile.is_open()) {
        string line;

        int net_size = 0;
        int row_id = 0;

        while ( getline(myfile,line) ) {
            //split string based on sep and store results into vector
            vector<string> fields;
            split(line, sep, fields);
            const char whitespace[] = " \n\t\r";
            
            if (net_size == 0) {
                net_size = fields.size();
                this->populate(net_size);
            }

            if ((unsigned) net_size != fields.size()) {
                cout << "Adjacenty matrix does not appear to be square.  Make sure matrix file has the same number of elements on each line.\n";
                exit(1);
            }

            for ( int col_id = 0; col_id<(signed) fields.size(); col_id++) {
                if (not is_directed() and row_id > col_id) continue;
                string val = strip(fields[col_id], whitespace);

                if (val == "1") {
                    get_node(row_id)->connect_to( get_node(col_id) );
                } else if (val == "0") {
                    continue;
                } else {
                    cout << "Warning: unknown value found in adjacency matrix file (not 0 or 1): " << val << endl;
                }
            }
            row_id++;
        }
    }
    dumper();
    cout << "finished dumping network\n";
    validate();
}*/

void LabNetwork::graphviz (string filename) {
    /*    if (get_edges().size() > 200) {
            cout << "LabNetwork is too large (> 200 edges) to reasonably output with graphviz/DOT\n";
            return;
        }*/
    if (filename == "") filename = "tmp.dot";

    ofstream pipe(filename.c_str(), ios::out);

    string graph_type = directed ? "digraph" : "graph";

    map< int, map <int, int> > seen_edges;
    pipe << graph_type << " NETWORK {\n\t\trankdir=LR;\n";
    cout << graph_type << " NETWORK {\n\t\trankdir=LR;\n";

    vector<LabNode*>::iterator node_it;
    node_it = node_list.begin();

    while (node_it != node_list.end() ) {

        int start = (*node_it)->id;

        string start_name = to_string( (*node_it)->get_name_or_id() );
        pipe << start << " [label=\"" << start_name << "\"]\n";
        cout << start << " [label=\"" << start_name << "\"]\n";

        if ( (*node_it)->deg() == 0 ) {
            cout << "Encountered unconnected node when trying to draw network.  This is currently not supported for visualization: node will be ignored.\n";
            continue;
        }
        vector<LabEdge*>::iterator edge_it;
        edge_it = (*node_it)->edges_out.begin();

        while (edge_it != (*node_it)->edges_out.end() ) {

            int end = (*edge_it)->end->id;

            string end_name = to_string( (*edge_it)->end->get_name_or_id() );

            if ( seen_edges[end][start] > 0 ) {
                seen_edges[end][start]--;
                seen_edges[start][end]--;
                edge_it++;
                continue;
            }
            else {
                seen_edges[start][end]++;
            }
            string edge_op = directed ? " -> " : " -- ";
            int edge_id = (*edge_it)->id;

            cout << "\t\t" << start << edge_op << end << " [label=\"" << edge_id << "\"];\n";
            pipe << "\t\t" << start << edge_op << end << " [label=\"" << edge_id << "\"];\n";
            edge_it++;
        }
        node_it++;
    }

    cout << "}\n";
    pipe << "}\n";

    pipe.close();
}

bool LabNetwork::is_stopped() {

	// GLG HACK : Don't understand how process_stopped gets True (system stuffs ??)
	//				So I force it to be false...
	//				[Otherwise Cython fails for networks having more than 256 nodes]
	process_stopped = false;
	//
	//

	if (process_stopped) {
		cout << "Hardware processing interruption" << endl;
		flush(cout);
	}
	bool status = process_stopped;
    process_stopped = false;
    return status;
}



//// Cython !!!
#ifndef NOPYTHON

PyObject * LabNetwork::CyGetPyObj() {
	return this->m_obj;
}

void LabNetwork::CySetPyObj(PyObject *obj) {
	//if (obj == NULL) { this->m_obj = NULL; return; }
	//if (this->m_obj) Py_XDECREF(this->m_obj);
	this->m_obj = obj;
	//Py_XINCREF(this->m_obj);
}

#endif

/////////////////////////////////////////////////////////
//			GLG : FOR SHORT
/////////////////////////////////////////////////////////

int LabNetwork::GetSize() {
	return this->size();
}

void LabNetwork::Populate(int n) {
	this->populate(n);
}

vector<LabNode*>& LabNetwork::GetNodes() {
	return this->get_nodes();
}

LabNode *LabNetwork::GetNode(int node_id) {
	return this->get_node(node_id);
}


bool LabNetwork::ErdosRenyi(double lambda) {
	return erdos_renyi(lambda);
}

bool LabNetwork::SparseRandomGraph(double lambda) {
	return sparse_random_graph(lambda);
}

bool LabNetwork::FastRandomGraph(double lambda){
	return fast_random_graph(lambda);
}

bool LabNetwork::RingLattice(int N, int K) {
	return ring_lattice(N, K);
}

bool LabNetwork::SquareLattice(int R, int C, bool diag) {
	return square_lattice(R, C, diag);
}

bool LabNetwork::SmallWorld(int N, int K, double beta) {
	return small_world(N, K, beta);
}

bool LabNetwork::RandConnectPoisson(double lambda) {
	return rand_connect_poisson(lambda);
}

bool LabNetwork::RandConnectPowerlaw(double alpha, double kappa) {
	return rand_connect_powerlaw(alpha, kappa);
}

bool LabNetwork::RandConnectExponential(double lambda) {
	return rand_connect_exponential(lambda);
}

bool LabNetwork::RandConnectUser(vector<double>& dist) {
	return rand_connect_user(dist);
}

bool LabNetwork::RandConnectExplicit(vector<int>& deg_series) {
	return rand_connect_explicit(deg_series);
}


bool LabNetwork::RandConnectUserLinear(int con_deg) {
	if (( (int) con_deg * this->size()) % 2 == 1) {
		cout << ("The sum of all degrees must be even\nThis is not possible with the network parameters you have specified");
		return false;
	}
	vector<double> dist(con_deg+1, 0);
	dist[con_deg] = 1;
	this->rand_connect_user(dist);
	return true;
}

// GLG : added
//At each call period, (fraction_to_migrate * 100) % of all nodes "migrate". A different
//group is randomly selected each period. If a nodes is in the migrating
//group, all of its edges are broken; this means the node's neighbors also
//end up with at least one broken edge. These broken edges ("stubs") are
//then randomly reconnected. The reconnection process will make sure that
//no self-loops or parallel edges get created (it's possible that
//additional edges will get shuffled in order to get rid of these).
void LabNetwork::MigrateNodes(double fraction_to_migrate) {

	// Create variables to handle node migration
	vector<LabNode*> nodes = this->get_nodes();
	vector<LabEdge*> broken_edges;
	int num_to_migrate = fraction_to_migrate * this->size();

	shuffle(nodes, this->get_rng()); // Shuffle the nodes

	for (int i = 0; i < num_to_migrate; i++) { // Migrate the first 1000
		LabNode* node = nodes[i];
		vector<LabEdge*> edges_out = node->get_edges_out();
		vector<LabEdge*> edges_in = node->get_edges_in();

		// Break all outbound edges
		for (unsigned int j = 0; j < edges_out.size(); j++) {
			LabEdge* outbound_edge = edges_out[j];
			if (not outbound_edge->is_stub() ) {
				edges_out[j]->break_end();
				broken_edges.push_back(edges_out[j]);
			}
		}

		// Break all inbound edges
		for (unsigned int j = 0; j < edges_in.size(); j++) {
			LabEdge* inbound_edge = edges_in[j];
			if (not inbound_edge->is_stub() ) {
				edges_in[j]->break_end();
				broken_edges.push_back(edges_in[j]);
			}
		}
	}

	// Randomly reconnect all broken edges
	this->rand_connect_stubs(broken_edges);
	broken_edges.clear();

	// The following line is not necessary, but validate() will check
	// to make sure that the network structure is still valid. For example,
	// If Node A has an edge leading to Node B, Node B must know it has an
	// inbound edge from Node A. In undirected graphs, there must be a
	// complementary edge from Node B to Node A. (Technically, these are
	// arcs.)
	this->validate();
}




////////////////////////////////////////////////////////////////////////////////
//
// LabNode Class Functions
//
///////////////////////////////////////////////////////////////////////////////

LabNode::LabNode(LabNetwork* network, string name, stateType state) {
    this->network = network;
    this->id = network->node_id_counter++;
    this->name = name;
    this->state = state;
}


LabNode::LabNode() {                   //empty constructor
    this->network = NULL;
    this->id = -1;
    this->name= "";
    this->state = 0;
}


LabNode::~LabNode() {                  //destructor
    //cout << "~LabNode() " << id << endl;
    for(unsigned int i=0; i< edges_out.size(); i++ ) delete edges_out[i];
    edges_out.clear();
}


//// GLG HACK : Cython related feature
//string LabNode::ThisToString() {
//	stringstream ss;
//	ss << (void *)this;
//	return ss.str();
//}



void LabNode::set_network(LabNetwork* network) {
    this->network = network;
}


void LabNode::add_stubs(int deg) {
    for (int i = 0; i < deg; i++) add_stub_out();
}


LabEdge* LabNode::add_stub_out () {
    LabEdge* stub = new LabEdge(this,NULL);
    edges_out.push_back(stub);
    network->set_topology_altered(true);
    return stub;
}


string LabNode::get_name_or_id () {
    return (name != "") ? name : to_string(id);
}


int LabNode::deg () {
    return edges_out.size();
}


LabEdge* LabNode::get_rand_edge() {
    assert(deg() > 0);
    return edges_out[ network->mtrand.randInt( deg() - 1 ) ];
}


vector<LabNode*> LabNode::get_neighbors () {
    vector<LabNode*> neighbors;
    for (unsigned int i = 0; i < edges_out.size(); i++) {
        neighbors.push_back(edges_out[i]->end);
    }
    return neighbors;
}


bool LabNode::is_neighbor (LabNode* node2) {
    vector<LabNode*> neighbors = get_neighbors();
    for (unsigned int i = 0; i < neighbors.size(); i++) if (neighbors[i] == node2) return true;
    return false;
}


// Move an edge going from this node from a "current" neighbor to a "future" neighbor
bool LabNode::change_neighbors (LabNode* current, LabNode* future) {
    // try to find an edge going to "current" node
    LabEdge* edge_in = NULL;
    LabEdge* edge_out = NULL;

    for (unsigned int i = 0; i < edges_out.size(); i++) {
        if ( current == edges_out[i]->end ) {
            edge_out = edges_out[i];
            break;
        }
    }
    
    if (edge_out == NULL) {
        return false;
    } else {
        edge_out->break_end();
        edge_out->define_end(future);
    }

    if (! get_network()->is_directed()) {
        for (unsigned int i = 0; i < edges_in.size(); i++) {
            if ( current == edges_in[i]->start ) {
                edge_in = edges_in[i];
                break;
            }
        }
        if (edge_in == NULL) {
            return false;
        } else {
            edge_in->_move_edge(future);
        }
    }

    return true;
}


void LabNode::connect_to (LabNode* end) {
    LabEdge* edge1 = add_stub_out();
    edge1->define_end(end);
    if ( ! network->is_directed()) {
        LabEdge* edge2 = end->add_stub_out();
        edge2->define_end(this);
    }
}


void LabNode::_add_inbound_edge (LabEdge* edge) {
    edges_in.push_back(edge);
    network->set_topology_altered(true);
}


void LabNode::_add_outbound_edge (LabEdge* edge) {
    edges_out.push_back(edge);
    network->set_topology_altered(true);
}


//this doesn't delete the edge object, it merely disconnects it from the node that it was going to.
void LabNode::_del_inbound_edge (LabEdge* inbound) {
    if (! inbound->end->id == this->id ) {
        cout << "The 'inbound' edge does not connect to the node provided." << endl;
        exit(100);
    }
    vector<LabEdge*>::iterator itr = find(edges_in.begin(), edges_in.end(), inbound);
    edges_in.erase(itr);
}


//this doesn't delete the edge object, it merely disconnects it from the node that it was coming from.
void LabNode::_del_outbound_edge (LabEdge* outbound) {
    if (! outbound->start->id == this->id ) {
        cout << "The 'outbound' edge does not start from the node provided." << endl;
        exit(101);
    }
    vector<LabEdge*>::iterator itr = find(edges_out.begin(), edges_out.end(), outbound);
    edges_out.erase(itr);
}


bool LabNode::operator==( const LabNode& n2 ) {
    if ( id == n2.id) return true;
    return false;
}


ostream& operator<< (ostream &out, LabNode* node) {
    out << node->id;
    return out;
}


void LabNode::dumper() {

    cout << "\tname => " << name << endl;
    cout << "\tid => "<< id << endl;
    cout << "\tdegree => " << deg() <<  endl;
    cout << "\tlocation => ";
    copy( loc.begin(), loc.end(), ostream_iterator<double>(cout, " "));
    cout << endl;

    vector<LabEdge*>::iterator itr;

    cout << "\tedges out => \n";
    for(itr = edges_out.begin(); itr != edges_out.end(); itr++ ) (*itr)->dumper();

    cout << "\tedges in => \n";
    for(itr = edges_in.begin();  itr != edges_in.end();  itr++ ) (*itr)->dumper();

    cout << "\n";
}

// Mean path length from this node to all others in same component
double LabNode::mean_min_path() {
    int component_size = 0;
    vector<LabNode*> empty;
    vector<double> distances = min_paths(empty);
    double sum = 0;
    for (int i = 0; i < (signed) distances.size(); i++) {
        if (distances[i] > -1 && id != i) {
            component_size++;
            sum += distances[i];
        }
    }
    double mean = (double) sum / (double) component_size;
                                 //quantum computing!! NAN != NAN is true
    if (mean != mean) cout << "Mean_minimum_path is not meaningful for one-node components.\n" << endl;
    return mean;
}


double LabNode::min_path(LabNode* dest) {
    vector<LabNode*> empty(0);
    vector<double> distances = this->min_paths(empty);
    return distances[dest->id];
}


vector<double> LabNode::min_unweighted_paths(vector<LabNode*> nodes) {
    if (nodes.size() == 0) nodes = get_network()->node_list;
    map <LabNode*, double> known_cost;
    queue<LabNode*> Q; // nodes to examine next
    vector<double> distances(nodes.size(), -1);

    map <LabNode*, int> hits; // checking for existence is faster with a map
    for (unsigned int i = 0; i < nodes.size(); i++) {
       hits[ nodes[i] ] = 1;
    }

    known_cost[this] = 0;  //We only know initially that there is no cost to get to the starting node
    Q.push(this);

    int j = hits.count(this); //How many shortest paths we know for nodes in 'nodes' variable
    while ( ! Q.empty() ) {
        if (get_network()->process_stopped) {vector<double> empty; return empty;}
        LabNode* known_node = Q.front();
        Q.pop();

        //Get the outbound edges for this known node
        vector <LabNode*> neighbors = known_node->get_neighbors();
        for (unsigned int i = 0; i < neighbors.size(); i++) {
            LabNode* v = neighbors[i];

            // if we've already done better, continue
            if (known_cost.count(v) == 1) {
                continue;
            } else {
                known_cost[v] = known_cost[known_node] + 1;
                Q.push(v);
                if (hits.count(v) == 1) {
                    j++;
                }
                if (j == (int) nodes.size()) { // we've found all the nodes we want
                    for ( unsigned int i=0; i<nodes.size(); i++) distances[i] = known_cost[nodes[i]];
                    return distances;
                }
            }
        }
    }

    for ( unsigned int i=0; i<nodes.size(); i++) {
        if (known_cost.count(nodes[i]) == 1) {
            distances[i] = known_cost[nodes[i]];
        }
    }
    return distances;
}


// Calculates length of the minimum path (if possible) between *this* and everything in *nodes*
// If *nodes* is empty, default is all nodes
vector<double> LabNode::min_paths(vector<LabNode*> nodes) {
    if (nodes.size() == 0) nodes = get_network()->node_list;
    map <LabNode*, double> known_cost; //Per Dijkstra's Algorithm, these are the two lists
    map <LabNode*, double>::iterator itr;
                                 //we need to keep track of
    map <LabNode*, double> uncertain_cost;

                                 //the following initializes the 'uncertain' set with undefined values,
                                 //since we have no information about these nodes yet.
    for (unsigned int i = 0; i < nodes.size(); i++) {
        if (this == nodes[i]) continue;
                                 // infinity =: max double value
        uncertain_cost[ nodes[i] ] = std::numeric_limits<double>::max();
    }
    known_cost[this] = 0;        //We only know initially that there is no cost to get to the starting node

    int j = 0;
                                 //As long as there are nodes with uncertain min costs
    while ( j++ < (signed) nodes.size() ) {
        if (get_network()->process_stopped) {vector<double> empty; return empty;}
                                 
        LabNode* min = NULL;
                                 //Loop through the nodes we know about.
        for ( itr = known_cost.begin(); itr != known_cost.end(); itr++) {
                                 //key of the map is the node
            LabNode* known_node = (*itr).first;
                                 //Get the outbound edges for this known node

            vector <LabEdge*> edges = known_node->edges_out;
            for (unsigned int i = 0; i < edges.size(); i++) {
                                 //Get this neighbor
                LabNode* neighbor = edges[i]->end;
                                 //Move on if this endpoint already has a known cost
                if ( known_cost.count(neighbor) > 0 ) continue;
                                 //Otherwise, calculate a cost using this path
                double cost = known_cost[known_node] + edges[i]->cost;

                //Store the new cost as an uncertain cost
                //if it's better than the others we've seen
                if (uncertain_cost.count(neighbor) > 0 && uncertain_cost[neighbor] < cost) {
                    continue;
                } else {
                    if ( min == NULL ) min = neighbor;
                    uncertain_cost[neighbor] = cost;
                    if ( uncertain_cost[neighbor] < uncertain_cost[min] ) min = neighbor;
                }
            }
        }

        //Now that we've looked at all the nodes we can get to from the known nodes,
        //find the least-cost uncertain node, and move that to the known group.
        //The two conditionals testing for undefs make sure that undefs are ranked
        //as higher costs than all numeric values
        int size = known_cost.size();

        if ( min != NULL) {
            known_cost[min] = (int) uncertain_cost[min];
            //            uncertain_cost.erase(min);
        }
        if ((unsigned) size == known_cost.size()) break;
    }                            //test  this->id = start;

    int n = nodes.size();
    vector<double> distances(n,-1);
    for ( int i=0; i<n; i++) {
        if ( known_cost.count(nodes[i]) ) {
            distances[i] = known_cost[nodes[i]];
        }
    }
    return distances;
}


// GLG HACK : added user data handlers

LabUserData *LabNode::GetUserData() {
	return userData;
}

void LabNode::SetUserData(LabUserData * user_data) {
	userData = user_data;
}

#ifndef NOPYTHON
PyObject *LabNode::CyGetPyObj() {
	return this->m_obj;
}

void LabNode::CySetPyObj(PyObject * obj) {
	this->m_obj = obj;
	//Py_XINCREF(this->m_obj);
}
PyObject *LabNode::CyGetUserData() {
	return this->m_data;
}

void LabNode::CySetUserData(PyObject * data) {
	this->m_data = data;
	//Py_XINCREF(this->m_data);
}
#endif

/////////////////////////////////////////////////////////
//			GLG : FOR SHORT
/////////////////////////////////////////////////////////
vector<LabNode*> LabNode::GetNeighbors() {
	return this->get_neighbors();
}

LabNetwork *LabNode::GetNetwork() {
	return this->get_network();
}

vector<double> LabNode::MinPaths(vector<LabNode*>& nodes) {
	return this->min_paths(nodes);
}



////////////////////////////////////////////////////////////////////////////////
//
// LabEdge Functions
//
////////////////////////////////////////////////////////////////////////////////

LabEdge::LabEdge(LabNode* start, LabNode* end) {
    this->network = start->network;
    this->id = this->network->edge_id_counter++;
    this->start = start;
    this->end = end;
    this->cost = 1;
}


LabEdge::~LabEdge() {  /*cout << "removing edge " << id << endl;*/ }

void LabEdge::delete_edge() {
    vector<LabEdge*>::iterator itr;

    if (end != NULL) {
        itr = find(end->edges_in.begin(), end->edges_in.end(), this);
        end->edges_in.erase(itr);
    }

    itr = find(start->edges_out.begin(), start->edges_out.end(), this);
    start->edges_out.erase(itr);

    network->set_topology_altered(true);
    delete this;
}


void LabEdge::disconnect_nodes() {
    LabEdge* comp = get_complement();

    delete_edge();
    comp->delete_edge();
}


void LabEdge::set_cost(double c) {
    this->cost=c;
}


LabEdge* LabEdge::get_complement () {
                                 // get the edges leaving the endpoint;
    vector<LabEdge*> edges = end->edges_out;
                                 // one is bound to be the complement
    for (unsigned int i = 0; i < edges.size(); i++) {
        //    cout << edges[i]->start->id << " " << edges[i]->end->id << endl;
        if (edges[i]==this) continue;
        if (edges[i]->end==start) return edges[i];
    }
    cout << "Failed to find a complement (antiparallel edge) to the edge provided.  There may be a problem with the network structure." << endl;
    exit(1);
}


void LabEdge::swap_ends (LabEdge* other_edge) {

    LabEdge* this_comp = get_complement();
    LabEdge* other_comp = other_edge->get_complement();

    break_end();
    define_end(other_comp->start);
    this_comp->break_end();
    this_comp->define_end(other_edge->start);
    other_edge->break_end();
    other_edge->define_end(this_comp->start);
    other_comp->break_end();
    other_comp->define_end(start);
}


/*
    sub swap_ends {
        my ( $self, $other_edge ) = @_;
        my @edges = ( $self, $self->get_complement(), $other_edge, $other_edge->get_complement() );
        my @rev_edges = reverse @edges;
        for my $i ( 0 .. 3 ) {
            my $edge = $edges[$i];
            $edge->break_end();
            $edge->define_end( $rev_edges[$i]->get_start() );
        }
        return;
    }
*/

void LabEdge::break_end () {
    if (end == NULL) return;
    end->_del_inbound_edge(this);
    end = NULL;
    network->set_topology_altered(true);
}


void LabEdge::define_end (LabNode* end_node) {
    end = end_node;
    end->_add_inbound_edge(this);
    network->set_topology_altered(true);
}


void LabEdge::_move_edge (LabNode* new_start_node) {
    if (start != NULL) {
        start->_del_outbound_edge(this);
        start = NULL;
    }
    start = new_start_node;
    start->_add_outbound_edge(this);
    network->set_topology_altered(true);
}


bool LabEdge::is_stub() {
    if (end == NULL) return true;
    return false;
}


bool LabEdge::operator==( const LabEdge& e2 ) {
    if ( id == e2.id) return true;
    return false;
}


ostream& operator<< (ostream &out, LabEdge* edge) {
    out << edge->id;
    return out;
}


void LabEdge::dumper () {
    string space = "\t\t";
    string start_name = start->get_name_or_id();
    string end_name = ( end != NULL ) ? end->get_name_or_id() : "undef";
    cout << space << "id => " << id << ", start => " << start_name << ", end => " << end_name << endl;
}

}

