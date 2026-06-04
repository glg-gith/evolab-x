///*
// * main.cpp
// *
// *  Created on: 23 sept. 2013
// *      Author: golgauth
// */
//
//#include "DiffEq_Sim.h"
//#include <cstring>
//
//
//
//
//// State matrix
//double VTCmtx[3][4][2];
//
////// Vector containing the virulence for
////// treated and non-treated pathogens
////double *vir, *virT;
////// Vector containing the transmission for
////// treated and non-treated pathogens
////double *beta, *betaT;
////// Vector containing the clearance for
////// treated and non-treated pathogens
////double *clear, *clearT;
//
//void set_globals(double mu)
//{
//	// Loci-specific rates
//	// Default state matrix
//	/*
//	 * 							  Default     A effect     B effect     Epistasis
//	 * 						 	 ___________________________________________________
//	 * 	Death			[I,T]	| [d, dT]     [ddA, ddAT]  [ddB, ddBT]  [dde, ddeT]
//	 * 	Virulence 		[I,T]	| [a, aT]     [daA, daAT]  [daB, daBT]  [dae, daeT]
//	 * 	Transmission	[I,T]	| [b, bT]     [dbA, dbAT]  [dbB, dbBT]  [dbe, dbeT]
//	 * 	Clearance		[I,T]	| [c, cT]     [dcA, dcAT]  [dcB, dcBT]  [dce, dceT]
//	 *
//	 */
//	double dmatrix[3][4][2] = {
//			{ {mu + 0.1, 	mu + 0.1}, 	{0.0, 		0.0}, 		{0.0, 		0.0}, 		{0.0,	0.0} },
//			{ {0.005, 		0.005}, 	{0.0, 		0.0}, 		{0.0, 		0.0}, 		{0.0,	0.0} },
//			{ {.00007, 		.00003}, 	{-.00001, 	.00002}, 	{-.00001, 	.00002}, 	{0.0,	0.0} },
//			{ {0.005, 		0.005}, 	{0, 		0.00}, 		{0, 		0.00}, 		{0.0,	0.0} }
//	};
//	memcpy(VTCmtx, dmatrix, sizeof(dmatrix));
//}
//
//
////void set_state_matrix(double mtx[3][4][2])
////{
////
////	// Virulence				[ex : treatment lowers virulence]
////	// Not-Treated / vir[Iab, IAb, IaB, IAB]
////	vir[0] = mtx[0][0][0];
////	vir[1] = vir[0] + mtx[0][1][0];
////	vir[2] = vir[0] + mtx[0][2][0];
////	vir[3] = vir[0] + mtx[0][1][0] + mtx[0][2][0] + mtx[0][3][0];
////	// Treated / virT[Tab, TAb, TaB, TAB]
////	virT[0] = mtx[0][0][1];
////	virT[1] = virT[0] + mtx[0][1][1];
////	virT[2] = virT[0] + mtx[0][2][1];
////	virT[3] = virT[0] + mtx[0][1][1] + mtx[0][2][1] + mtx[0][3][1];
////
////	// Transmission				[ex : treatment lowers transmission]
////	// Not-Treated / beta[Iab, IAb, IaB, IAB]
////	beta[0] = mtx[1][0][0];
////	beta[1] = beta[0] + mtx[1][1][0];
////	beta[2] = beta[0] + mtx[1][2][0];
////	beta[3] = beta[0] + mtx[1][1][0] + mtx[1][2][0] + mtx[1][3][0];
////	// Treated / betaT[Tab, TAb, TaB, TAB]
////	betaT[0] = mtx[1][0][1];
////	betaT[1] = betaT[0] + mtx[1][1][1];
////	betaT[2] = betaT[0] + mtx[1][2][1];
////	betaT[3] = betaT[0] + mtx[1][1][1] + mtx[1][2][1] + mtx[1][3][1];
////
////	// Clearance				[ex : treatment uppers clearance]
////	// Not-Treated / clear[Iab, IAb, IaB, IAB]
////	clear[0] = mtx[2][0][0];
////	clear[1] = clear[0] + mtx[2][1][0];
////	clear[2] = clear[0] + mtx[2][2][0];
////	clear[3] = clear[0] + mtx[2][1][0] + mtx[2][2][0] + mtx[2][3][0];
////	// Treated / clearT[Tab, TAb, TaB, TAB]
////	clearT[0] = mtx[2][0][1];
////	clearT[1] = clearT[0] + mtx[2][1][1];
////	clearT[2] = clearT[0] + mtx[2][2][1];
////	clearT[3] = clearT[0] + mtx[2][1][1] + mtx[2][2][1] + mtx[2][3][1];
////
////	//for(int i=0;i<ngeno;i++) vir[i]=(virmin+(virmax-virmin)*i/(ngeno-1));
////	//for(int i=0;i<ngeno;i++) {beta[i]=((b0*(pow((vir[i]),coef)))/N);};	//// TRADEOFF SHAPE
////
////}
////
//
//
//
//
//class DrugRes : public DiffEq_Sim {
//
////    private:
////        double _t;      //initial time
////        double _h;      //time step
////        double _tmax;   //max time
////        double _hmin;
//
//private:
//	// Individuals
////	int S = 270;			// TODO : Replace it by endemic equilibrium law
////	int I = 80;				/// 80
////	int T = 20;				/// 20
//
//	// General rates
//	double theta;
//	double mu;
//	double tho;
//	double sigma;
//	double rp;
//	double mut;
//
//	double alpha;
//	double alphaT;
//	double beta;
//	double betaT;
//	double gamma;
//	double gammaT;
//
//
//public:
//	//DrugRes(double t, double tmax, double h, double hmin) : t(t), tmax(tmax), h(h), hmin(hmin)  { nbins=3;}
//	DrugRes(double t, double tmax, double h, double hmin) : DiffEq_Sim(t, tmax, h, hmin) { nbins=3; };
//	//DrugRes(double b, double g): beta(b), gamma(g) { nbins=3; }
//	~DrugRes() {};
//
//	void initialize( double S, double I, double T) {
//		y = new double[nbins];
//		y[0] = S; y[1] = I; y[2] = T;
//
//		// General rates
//		theta = 10.0;
//		mu = .01;///.01;
//		tho = 0.8;///0.8;
//		sigma = 1;
//		rp  = 0.2;
//		mut = 0.01;///0.001;
//
//	//	{0.005, 	0.005},
//	//	{.00007, 	.00003},
//	//	{0.005, 	0.005},
//
//		alpha = 0.005;
//		alphaT = 0.005;
//		beta = .00007;
//		betaT = .00003;
//		gamma = 0.005;
//		gammaT = 0.005;
//
//
//		set_globals(this->mu);
//	//	set_state_matrix(VTCmtx);
//
//
//	}
//
//	void derivative(double const y[], double dydt[]) {
//		dydt[0] = theta - mu*y[0] - (beta*y[1] + betaT*y[2])*y[0] + (gamma*y[1] + gammaT*y[2]);
//		dydt[1] = (1 - tho)*(beta*y[1] + betaT*y[2])*y[0] - (mu + alpha + gamma)*y[1];
//		dydt[2] = tho*(beta*y[1] + betaT*y[2])*y[0] - (mu + alphaT + gammaT)*y[2];
//
//	}
//
//};
//
//int main(int argc, char *argv[])
//{
////    _t    = 0.0;      //initial time
////    _h    = 0.1;     //time step
////    _tmax = 2000;
////    _hmin = 0.2;
//
//
//    DrugRes *sim = new DrugRes(0.0, 1200, 0.1, 0.2);
//	sim->initialize(270, 80, 20);
//	cout << "nbins = " << sim->nbins << endl;
//	sim->run_simulation();
//}
//
////class SIR : public DiffEq_Sim {
////
////    private:
////        const double beta;
////        const double gamma;
////
////    public:
////        SIR() : beta(0.0), gamma(0.0) { nbins=3;}
////        SIR(double b, double g): beta(b), gamma(g) { nbins=3; }
////        ~SIR() {};
////
////        void initialize( double S, double I, double R) {
////            y = new double[nbins];
////            y[0] = S; y[1] = I; y[2] = R;
////        }
////
////        void derivative(double const y[], double dydt[]) {
////            dydt[0] = -beta*y[0]*y[1];
////            dydt[1] = +beta*y[0]*y[1] - gamma*y[1];
////            dydt[2] = +gamma*y[1];
////        }
////
////};
////
////int main(int argc, char *argv[])
////{
////	SIR *sim = new SIR(0.01, 0.02);
////	sim->initialize(270, 80, 20);
////	cout << "nbins = " << sim->nbins << endl;
////	sim->run_simulation();
////}
