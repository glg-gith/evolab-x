/*
 * DrugRes0.h
 *
 *  Created on: Sep 25, 2013
 *      Author: golgauth
 */

#ifndef DrugRes0_H_
#define DrugRes0_H_


#include "DiffEq_Sim.h"
#include <cstring>


class DrugRes0 : public DiffEq_Sim {

//    private:
//        double _t;      //initial time
//        double _h;      //time step
//        double _tmax;   //max time
//        double _hmin;

private:
	// Individuals
//	int S = 270;			// TODO : Replace it by endemic equilibrium law
//	int I = 80;				/// 80
//	int T = 20;				/// 20

	// General rates
	double theta;
	double mu;
	double tho;
	double sigma;
	double rp;
	double mut;

	double alpha;
	double alphaT;
	double beta;
	double betaT;
	double gamma;
	double gammaT;


public:
	//DrugRes0(double t, double tmax, double h, double hmin) : t(t), tmax(tmax), h(h), hmin(hmin)  { nbins=3;}
	DrugRes0(double t, double tmax, double h, double hmin) : DiffEq_Sim(t, tmax, h, hmin) { nbins=3; };
	//DrugRes0(double b, double g): beta(b), gamma(g) { nbins=3; }
	~DrugRes0() {};

	void initialize( double S, double I, double T) {
		y = new double[nbins];
		y[0] = S; y[1] = I; y[2] = T;

		// General rates
		theta = 10.0;
		mu = .01;///.01;
		tho = 0.8;///0.8;
		sigma = 1;
		rp  = 0.2;
		mut = 0.01;///0.001;

	//	{0.005, 	0.005},
	//	{.00007, 	.00003},
	//	{0.005, 	0.005},

		alpha = 0.005;
		alphaT = 0.005;
		beta = .00007;
		betaT = .00003;
		gamma = 0.005;
		gammaT = 0.005;
	}

	void derivative(double const y[], double dydt[]) {
		dydt[0] = theta - mu*y[0] - (beta*y[1] + betaT*y[2])*y[0] + (gamma*y[1] + gammaT*y[2]);
		dydt[1] = (1 - tho)*(beta*y[1] + betaT*y[2])*y[0] - (mu + alpha + gamma)*y[1];
		dydt[2] = tho*(beta*y[1] + betaT*y[2])*y[0] - (mu + alphaT + gammaT)*y[2];

	}

};

#endif /* DrugRes0_H_ */
