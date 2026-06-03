#include "Deterministic_Network_SIR_Sim.h"

#include "Network.h"

int main() { 

    //int N        = 1000000;
    //double R0    = 1.5;
    double R  = 0.2;
    double MU = 0.2;

    Network net("name", Network::Undirected);
    net.populate(10000);
    net.erdos_renyi(5);
    vector<double> deg_dist_param = net.get_gen_deg_dist();

    double theta = 1.0;
    double I = 1.0/1000;
    double S = 1.0 - I;

    Deterministic_Network_SIR_Sim sim(R, MU, deg_dist_param);
    sim.initialize(theta, S, I);
    sim.run_simulation();

    return 0;
}


