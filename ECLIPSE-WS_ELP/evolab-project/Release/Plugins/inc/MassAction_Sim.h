#ifndef MASSIM_H
#define MASSIM_H

#include <stdlib.h>
#include <vector>
#include <iostream>
#include <queue>
#include "Utility.h"
//#include "Network.h"

using namespace std;
using namespace elpm;

namespace study {


class MA_Event {
    public:
        double time;
        char type;
        MA_Event(const MA_Event& o) {  time = o.time; type=o.type; }
        MA_Event(double t, char e) { time=t; type=e; }
        MA_Event& operator=(const MA_Event& o) { time = o.time; type=o.type; }
};

class MA_compTime {
    public:
        bool operator() (const MA_Event* lhs, const MA_Event* rhs) const {
            return (lhs->time>rhs->time);
        }

        bool operator() (const MA_Event& lhs, const MA_Event& rhs) const {
            return (lhs.time>rhs.time);
        }
};

class MassAction_Sim {
    public:
                                    // constructor
        MassAction_Sim( int n, double gamma, double beta) { N=n; GAMMA=gamma; BETA=beta; reset(); }

        int N;                      // population size
        double GAMMA;               // param for exponential recovery time
        double BETA;                // param for exponential transmission time

                                    // event queue
        priority_queue<MA_Event, vector<MA_Event>, MA_compTime > EventQ;
        vector<int> Compartments;   // S, I, R compartments, with counts for each
        //vector<float> Transmissions;
        double Now;                 // Current "time" in simulation

        MTRand mtrand;              // RNG

        void run_simulation() {
//            int day = -1;
            while (next_event()) {
//                if ((int) Now > day) {
//                    cout << Now << "\t" << Compartments[1] << endl;
//                    day = (int) Now;
//                }

                continue;
            }
        }

        int epidemic_size() {
            return Compartments[2]; // Recovered class
        }

        int reset() {
            Now = 0.0;

            Compartments.clear();
            Compartments.resize(3,0);
            Compartments[0] = N;
            
            EventQ = priority_queue<MA_Event, vector<MA_Event>, MA_compTime > ();
            //Transmissions.clear();
        }

        void rand_infect(int k) {   // randomly infect k people
            for (int i = 0; i < k; i++) {
                infect();
            }
            return;
        }

        void infect() {
            assert(Compartments[0] > 0);
            Compartments[0]--;      // decrement susceptibles
            Compartments[1]++;      // increment infecteds
                                    // time to recovery
            double Tr = rand_exp(GAMMA, &mtrand) + Now;
                                    // time to next contact
            double Tc = rand_exp(BETA, &mtrand) + Now;
            while ( Tc < Tr ) {     // does contact occur before recovery?
                add_event(Tc, 'c'); // potential transmission event
                Tc += rand_exp(BETA, &mtrand);
            }
            add_event(Tr, 'r' );
            //Transmissions.push_back(Now);
            return;
        }

        bool is_susceptible(int x) {
            if (Compartments[0] >= x) return true;
            else return false;
        }

        int next_event() {
            if ( EventQ.empty() ) return 0;
            MA_Event event = EventQ.top(); // get the element
            EventQ.pop();               // remove from Q

            Now = event.time;           // advance time
            if (event.type == 'r') {    // recovery event
                Compartments[1]--;      // decrement Infected class
                Compartments[2]++;      // increment Recovered class
            } else {                    // event type must be 'c'
                                 
                // N-2 because person can't self-infect, and because randint includes endpoints
                int contact = mtrand.randInt(N-2) + 1; // add 1 b/c there's no person 0
                if ( is_susceptible(contact) ) infect();

            }
            return 1;
        }

        void add_event( double time, char type) {
            EventQ.push( MA_Event(time,type) );
            return;
        }
/*
        int count_recent_events(double time, double window) {
            int ct = 0;
            for (int i = (signed) Transmissions.size() - 1; i != -1; i--) {
                if (Transmissions[i] > time - window) {
                    ct++;
                } else {
                    break;
                }
            }
            return ct;
        }
*/

};

}

#endif
