#ifndef  CHAIN_SIMULATOR_H
#define  CHAIN_SIMULATOR_H

#include "Simulator.h"
#include <queue>

namespace study {

class CB_Event {

    public:
        LabNode* sink_node;
        int time;
        LabNode* source_node;
        CB_Event(const CB_Event& o) {  sink_node=o.sink_node; time = o.time; source_node = o.source_node; }
        CB_Event(LabNode* sink, int t, LabNode* source) {  sink_node=sink; time=t; source_node=source; }
        CB_Event& operator=(const CB_Event& o) {
            if (this != &o) {
                sink_node=o.sink_node; time = o.time; source_node = o.source_node;
            }
            return *this;
        }

};

class CB_compTime {
    public:
        bool operator() (const CB_Event* lhs, const CB_Event* rhs) const {
            return (lhs->time>rhs->time);
        }

        bool operator() (const CB_Event& lhs, const CB_Event& rhs) const {
            return (lhs.time>rhs.time);
        }
};


class ChainBinomial_Sim: public Simulator
{
    protected:
        list<LabNode*> infected;
        vector<LabNode*> recovered;
        vector<double> time_dist; // Probability mass function for day of transmission
        priority_queue<CB_Event, vector<CB_Event>, CB_compTime > transmissionQ;

    public:
        double T;                // transmissibiltiy per time step
        int infectious_period;

        ChainBinomial_Sim() { this->time = 0; };
        ChainBinomial_Sim(LabNetwork* net, int infectious_period, double T):Simulator(net) { this->infectious_period=infectious_period; this->T=T; define_time_dist();};

        void set_infectious_period(int d) { this->infectious_period = d; }
        void set_transmissibility(double t) { this->T = t; }
        vector<double> define_time_dist() {
            for (int i = 0; i < infectious_period; i++) {
                time_dist.push_back( pow(1-T, i) * T );
            }
            time_dist.push_back( pow(1-T, infectious_period) );
            return time_dist;
        }

        int    get_infectious_period() { return this->infectious_period; }
        double get_transmissibility()  { return this->T; }

        vector<LabNode*> rand_infect (int n) { // randomly infect n nodes
            assert(n > 0);
            vector<LabNode*> patients_zero = rand_choose_nodes(n);//set_nodes_to_state(n, 1);
            for (int i = 0; i < n; i++) infect_node(patients_zero[i]);
            return patients_zero;
        }

        void infect_node(LabNode* node) {
            if (node->get_state() != 0) return; //already infected or recovered
            node->set_state(1);
            infected.push_back(node);
            vector<LabNode*> neighbors = node->get_neighbors();
            for (unsigned int i = 0; i<neighbors.size(); i++) {
                if (neighbors[i]->get_state() == 0) {
                    int t = rand_nonuniform_int(time_dist, mtrand) + 1;
                    if (t > infectious_period) {
                        continue;
                    } else {
                        transmissionQ.push( CB_Event( neighbors[i], time + t, node ) );
                    }
                }
            }
        }

        void step_simulation () {
            // States: 0 (default) is susceptible
            //         1 is infectious day 1
            //         2 is infectious day 2
            //         ... up to the infectious period
            //         -1 is recovered
            int inf_size = infected.size();
            assert(inf_size > 0);
            time++;
            list<LabNode*>::iterator inode;
            for (inode=infected.begin(); inode!=infected.end(); inode++) {
                // Increment node states through the infectious period
                (*inode)->set_state( (*inode)->get_state()+1 );
            }

            // Some nodes may be reaching the end of their infectious period ...

            while (inf_size-- > 0) {
                LabNode* first = infected.front();
                if (first->get_state() <= infectious_period) break;

                first->set_state(-1); // -> recovered
                recovered.push_back( first );
                infected.pop_front();
            }

            while (not transmissionQ.empty() and transmissionQ.top().time <= time) {
                CB_Event CB_Event = transmissionQ.top();
                transmissionQ.pop();
                infect_node( CB_Event.sink_node );
            }
        }

        void run_simulation() {
            assert(infectious_period > 0 && T >= 0 && T <= 1);
            
            // As long as someone's still infected, step simulation
            while (infected.size() > 0)  step_simulation();
        }

        void add_CB_Event( LabNode* sink_node, int time, LabNode* source_node) {
            transmissionQ.push( CB_Event(sink_node,time,source_node) );
            return;
        }


        int count_infected() {
            return infected.size();
        }

        int epidemic_size() {
            return recovered.size();
        }

        void reset() {
            reset_time();

            while (! infected.empty()) {
                infected.front()->set_state(0);
                infected.pop_front();
            }

            set_these_nodes_to_state(recovered, 0);
            recovered.clear();
        }

        void summary() {
            cerr << "LabNetwork size: " << net->size();
            cerr << "\tTransmissibility: " << T;
            cerr << "\tEpidemic size: " << recovered.size() << "\n\n";

        }
};

}
#endif
