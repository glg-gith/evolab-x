/*
 * main.cpp
 *
 *  Created on: Oct 2, 2013
 *      Author: golgauth
 */

/*
 * main.cpp
 *
 *  Created on: 23 sept. 2013
 *      Author: golgauth
 */



#include "DiffEq_Sim.h"

#include "main.h"


// Files
char filename[FNAMELEN];
char rpath[FNAMELEN];


// State matrix
double VTCmtx[3][4][2];

//// Vector containing the virulence for
//// treated and non-treated pathogens
//double *vir, *virT;
//// Vector containing the transmission for
//// treated and non-treated pathogens
//double *beta, *betaT;
//// Vector containing the clearance for
//// treated and non-treated pathogens
//double *clear, *clearT;

void set_globals(double mu)
{
	// Loci-specific rates
	// Default state matrix
	/*
	 * 							  Default     A effect     B effect     Epistasis
	 * 						 	 ___________________________________________________
	 * 	Death			[I,T]	| [d, dT]     [ddA, ddAT]  [ddB, ddBT]  [dde, ddeT]
	 * 	Virulence 		[I,T]	| [a, aT]     [daA, daAT]  [daB, daBT]  [dae, daeT]
	 * 	Transmission	[I,T]	| [b, bT]     [dbA, dbAT]  [dbB, dbBT]  [dbe, dbeT]
	 * 	Clearance		[I,T]	| [c, cT]     [dcA, dcAT]  [dcB, dcBT]  [dce, dceT]
	 *
	 */
	double dmatrix[3][4][2] = {
			{ {0.01, 	0.01}, 		{0.0, 		0.0}, 		{0.00, 		0.00}, 		{0.0,	0.0} },
			{ {.00007, 	.00003}, 	{-.00002, 	.00002}, 	{-.00002, 	.00002}, 	{0.0,	0.0} },
			{ {0.0, 	0.0}, 		{0, 		0.0}, 		{0, 		0.0}, 		{0.0,	0.0} }
	};
	memcpy(VTCmtx, dmatrix, sizeof(dmatrix));
}



//void set_state_matrix(double mtx[3][4][2])
//{
//
//	// Virulence				[ex : treatment lowers virulence]
//	// Not-Treated / vir[Iab, IAb, IaB, IAB]
//	vir[0] = mtx[0][0][0];
//	vir[1] = vir[0] + mtx[0][1][0];
//	vir[2] = vir[0] + mtx[0][2][0];
//	vir[3] = vir[0] + mtx[0][1][0] + mtx[0][2][0] + mtx[0][3][0];
//	// Treated / virT[Tab, TAb, TaB, TAB]
//	virT[0] = mtx[0][0][1];
//	virT[1] = virT[0] + mtx[0][1][1];
//	virT[2] = virT[0] + mtx[0][2][1];
//	virT[3] = virT[0] + mtx[0][1][1] + mtx[0][2][1] + mtx[0][3][1];
//
//	// Transmission				[ex : treatment lowers transmission]
//	// Not-Treated / beta[Iab, IAb, IaB, IAB]
//	beta[0] = mtx[1][0][0];
//	beta[1] = beta[0] + mtx[1][1][0];
//	beta[2] = beta[0] + mtx[1][2][0];
//	beta[3] = beta[0] + mtx[1][1][0] + mtx[1][2][0] + mtx[1][3][0];
//	// Treated / betaT[Tab, TAb, TaB, TAB]
//	betaT[0] = mtx[1][0][1];
//	betaT[1] = betaT[0] + mtx[1][1][1];
//	betaT[2] = betaT[0] + mtx[1][2][1];
//	betaT[3] = betaT[0] + mtx[1][1][1] + mtx[1][2][1] + mtx[1][3][1];
//
//	// Clearance				[ex : treatment uppers clearance]
//	// Not-Treated / clear[Iab, IAb, IaB, IAB]
//	clear[0] = mtx[2][0][0];
//	clear[1] = clear[0] + mtx[2][1][0];
//	clear[2] = clear[0] + mtx[2][2][0];
//	clear[3] = clear[0] + mtx[2][1][0] + mtx[2][2][0] + mtx[2][3][0];
//	// Treated / clearT[Tab, TAb, TaB, TAB]
//	clearT[0] = mtx[2][0][1];
//	clearT[1] = clearT[0] + mtx[2][1][1];
//	clearT[2] = clearT[0] + mtx[2][2][1];
//	clearT[3] = clearT[0] + mtx[2][1][1] + mtx[2][2][1] + mtx[2][3][1];
//
//	//for(int i=0;i<ngeno;i++) vir[i]=(virmin+(virmax-virmin)*i/(ngeno-1));
//	//for(int i=0;i<ngeno;i++) {beta[i]=((b0*(pow((vir[i]),coef)))/N);};	//// TRADEOFF SHAPE
//
//}




class DrugRes : public DiffEq_Sim {

public:
	ofstream _file;


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

	double p1;
	double pA;

	double rp;
	double mut;

	/*double alpha;
	double alphaT;
	double beta;
	double betaT;
	double gamma;
	double gammaT;*/

	double alphaB2, alphaB1, alphaA2, alphaA1;
	double alphaB2T, alphaB1T, alphaA2T, alphaA1T;
	double betaB2, betaB1, betaA2, betaA1;
	double betaB2T, betaB1T, betaA2T, betaA1T;
	double gammaB2, gammaB1, gammaA2, gammaA1;
	double gammaB2T, gammaB1T, gammaA2T, gammaA1T;

public:
	int tot_steps;

public:
	DrugRes(double t, double tmax, double h, double hmin) : DiffEq_Sim(t, tmax, h, hmin) { nbins = 9; };
	~DrugRes() {};

	void initialize( double S, double I, double T, double r) {


		y = new double[nbins];
		y[0] = S; y[1] = I; y[2] = T;

		// General rates
		//tmax = 1200;

		theta = 10.0;
		mu = 0.01;
		tho = 0.8;
		sigma = 1.0;

		rp = r;
		mut = 0.01;///0.01;

		p1 = .025;
		pA = .025;

		I = 80;
		T = 20;
		S = 270;

		double IA1init, IA2init, IB1init, IB2init;
		double TA1init, TA2init, TB1init, TB2init;

		IA1init = p1 * pA * I; IA2init = (1 - p1) * pA * I; IB1init = (1 - pA) * p1 * I; IB2init = (1 - p1) * (1 - pA) * I;
		TA1init = p1 * pA * T; TA2init = (1 - p1) * pA * T; TB1init = (1 - pA) * p1 * T; TB2init = (1 - p1) * (1 - pA) * T;


		double alpha, deltaAlpha1, deltaAlphaA, deltaAlphaE;
		double alphaT, deltaAlpha1T, deltaAlphaAT, deltaAlphaET;
		double beta, deltaBeta1, deltaBetaA, deltaBetaE;
		double betaT, deltaBeta1T, deltaBetaAT, deltaBetaET;
		double gamma, deltaGamma1, deltaGammaA, deltaGammaE;
		double gammaT, deltaGamma1T, deltaGammaAT, deltaGammaET;



		alpha = 0.01; deltaAlpha1 = 0.0; deltaAlphaA = 0.0; deltaAlphaE = 0.0;
		alphaT = 0.01; deltaAlpha1T = 0.0; deltaAlphaAT = 0.0; deltaAlphaET = 0.0;

		beta = 0.00007; deltaBeta1 = -0.00002; deltaBetaA = -0.00002; deltaBetaE = 0.0;
		betaT = 0.00003; deltaBeta1T = 0.00002; deltaBetaAT = 0.00002; deltaBetaET = 0.0;

		gamma = 0.0; deltaGamma1 = 0.0; deltaGammaA = 0.0; deltaGammaE = 0.0;
		gammaT = 0.0; deltaGamma1T = 0.0; deltaGammaAT = 0.0; deltaGammaET = 0.0;




		alphaB2 = alpha; alphaB1 = alpha + deltaAlpha1; alphaA2 = alpha + deltaAlphaA; alphaA1 = alpha + deltaAlpha1 + deltaAlphaA + deltaAlphaE;
		alphaB2T = alphaT; alphaB1T = alphaT + deltaAlpha1T; alphaA2T = alphaT + deltaAlphaAT; alphaA1T = alphaT + deltaAlpha1T + deltaAlphaAT + deltaAlphaET;

		betaB2 = beta; betaB1 = beta + deltaBeta1; betaA2 = beta + deltaBetaA; betaA1 = beta + deltaBeta1 + deltaBetaA + deltaBetaE;
		betaB2T = betaT; betaB1T = betaT + deltaBeta1T; betaA2T = betaT + deltaBetaAT; betaA1T = betaT + deltaBeta1T + deltaBetaAT + deltaBetaET;

		gammaB2 = gamma; gammaB1 = gamma + deltaGamma1; gammaA2 = gamma + deltaGammaA; gammaA1 = gamma + deltaGamma1 + deltaGammaA + deltaGammaE;
		gammaB2T = gammaT; gammaB1T = gammaT + deltaGamma1T; gammaA2T = gammaT + deltaGammaAT; gammaA1T = gammaT + deltaGamma1T + deltaGammaAT + deltaGammaET;


	//	set_globals(this->mu);
	//	set_state_matrix(VTCmtx);

//	    s[0] = S; IA1[0] = IA1init; IA2[0] = IA2init; IB1[0] = IB1init; IB2[0] = IB2init;
//	              TA1[0] = TA1init; TA2[0] = TA2init; TB1[0] = TB1init; TB2[0] = TB2init;
		y[0] = S; y[1] = IA1init; y[2] = IA2init; y[3] = IB1init; y[4] = IB2init;
				  y[5] = TA1init; y[6] = TA2init; y[7] = TB1init; y[8] = TB2init;


		tot_steps = 0;
	}

	void derivative(double const y[], double dydt[]) {
//		dydt[0] = theta - mu*y[0] - (beta*y[1] + betaT*y[2])*y[0] + (gamma*y[1] + gammaT*y[2]);
//		dydt[1] = (1 - tho)*(beta*y[1] + betaT*y[2])*y[0] - (mu + alpha + gamma)*y[1];
//		dydt[2] = tho*(beta*y[1] + betaT*y[2])*y[0] - (mu + alphaT + gammaT)*y[2];

//        double y_err = 1e-10;
//        gsl_odeiv_control_hadjust(c, s, y, &y_err, dydt, &_h);


		double r = this->rp;


		dydt[0] = theta - (mu*y[0]) - ((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] + betaB2*y[4]) +
	                                  (betaA1T*y[5] + betaA2T*y[6] + betaB1T*y[7] + betaB2T*y[8])) * y[0] -
	                                 ((gammaA1*y[1] + gammaA2*y[2] + gammaB1*y[3] + gammaB2*y[4]) +
	                                  (gammaA1T*y[5] + gammaA2T*y[6] + gammaB1T*y[7] + gammaB2T*y[8]));

		dydt[1] = (1 - tho)*(betaA1*y[1] + betaA1T*y[5])*y[0] - (mu + alphaA1 + gammaA1)*y[1] -
	               sigma*((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] + betaB2*y[4]) +
	                      (betaA1T*y[5] + betaA2T*y[6] + betaB1T*y[7] + betaB2T*y[8]))*y[1] +
	               sigma*(
	                      (sigma/2)*(y[1]*((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] + betaB2*y[4]) +
	                                         (betaA1T*y[5] + betaA2T*y[6] + betaB1T*y[7] + betaB2T*y[8])) +
	                                (y[1] + y[2] + y[3] + y[4])*(betaA1*y[1] + betaA1T*y[5]))
	                    + (sigma*r/2)*(y[2]*(betaB1*y[3] + betaB1T*y[7]) +
	                                   y[3]*(betaA2*y[2] + betaA2T*y[6]) -
	                                   y[1]*(betaB2*y[4] + betaB2T*y[8]) -
	                                   y[4]*(betaA1*y[1] + betaA1T*y[5]))
	                     );

		dydt[2] = (1 - tho)*(betaA2*y[2] + betaA2T*y[6])* y[0] - (mu + alphaA2 + gammaA2)*y[2] -
	               sigma*((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] + betaB2*y[4]) +
	                      (betaA1T*y[5] + betaA2T*y[6] + betaB1T*y[7] + betaB2T*y[8]))*y[2] +
	               sigma*(
	                      (sigma/2)*(y[2]*((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] + betaB2*y[4]) +
	                                         (betaA1T*y[5] + betaA2T*y[6] + betaB1T*y[7] + betaB2T*y[8])) +
	                                (y[1] + y[2] + y[3] + y[4])*(betaA2*y[2] + betaA2T*y[6]))
	                    - (sigma*r/2)*(y[2]*(betaB1*y[3] + betaB1T*y[7]) +
	                                   y[3]*(betaA2*y[2] + betaA2T*y[6]) -
	                                   y[1]*(betaB2*y[4] + betaB2T*y[8]) -
	                                   y[4]*(betaA1*y[1] + betaA1T*y[5]))
	                     );

		dydt[3] = (1 - tho)*(betaB1*y[3] + betaB1T*y[7])*y[0] - (mu + alphaB1 + gammaB1)*y[3] -
	               sigma*((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] + betaB2*y[4]) +
	                      (betaA1T*y[5] + betaA2T*y[6] + betaB1T*y[7] + betaB2T*y[8]))*y[3] +
	               sigma*(
	                      (sigma/2)*(y[3]*((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] + betaB2*y[4]) +
	                                         (betaA1T*y[5] + betaA2T*y[6] + betaB1T*y[7] + betaB2T*y[8])) +
	                                (y[1] + y[2] + y[3] + y[4])*(betaB1*y[3] + betaB1T*y[7]))
	                    - (sigma*r/2)*(y[2]*(betaB1*y[3] + betaB1T*y[7]) +
	                                   y[3]*(betaA2*y[2] + betaA2T*y[6]) -
	                                   y[1]*(betaB2*y[4] + betaB2T*y[8]) -
	                                   y[4]*(betaA1*y[1] + betaA1T*y[5]))
	                     );

		dydt[4] = (1 - tho)*(betaB2*y[4] + betaB2T*y[8])*y[0] - (mu + alphaB2 + gammaB2)*y[4] -
	               sigma*((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] + betaB2*y[4]) +
	                      (betaA1T*y[5] + betaA2T*y[6] + betaB1T*y[7] + betaB2T*y[8]))*y[4] +
	               sigma*(
	                      (sigma/2)*(y[4]*((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] + betaB2*y[4]) +
	                                         (betaA1T*y[5] + betaA2T*y[6] + betaB1T*y[7] + betaB2T*y[8])) +
	                                (y[1] + y[2] + y[3] + y[4])*(betaB2*y[4] + betaB2T*y[8]))
	                    + (sigma*r/2)*(y[2]*(betaB1*y[3] + betaB1T*y[7]) +
	                                   y[3]*(betaA2*y[2] + betaA2T*y[6]) -
	                                   y[1]*(betaB2*y[4] + betaB2T*y[8]) -
	                                   y[4]*(betaA1*y[1] + betaA1T*y[5]))
	                     );


		dydt[5] = (tho)*(betaA1*y[1] + betaA1T*y[5])*y[0] - (mu + alphaA1T + gammaA1T)*y[5] -
	               sigma*((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] + betaB2*y[4]) +
	                      (betaA1T*y[5] + betaA2T*y[6] + betaB1T*y[7] + betaB2T*y[8]))*y[5] +
	               sigma*(
	                      (sigma/2)*(y[5]*((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] + betaB2*y[4]) +
	                                         (betaA1T*y[5] + betaA2T*y[6] + betaB1T*y[7] + betaB2T*y[8])) +
	                                (y[5] + y[6] + y[7] + y[8])*(betaA1*y[1] + betaA1T*y[5]))
	                    + (sigma*r/2)*(y[6]*(betaB1*y[3] + betaB1T*y[7]) +
	                                   y[7]*(betaA2*y[2] + betaA2T*y[6]) -
	                                   y[5]*(betaB2*y[4] + betaB2T*y[8]) -
	                                   y[8]*(betaA1*y[1] + betaA1T*y[5]))
	                     );

		dydt[6] = (tho)*(betaA2*y[2] + betaA2T*y[6])*y[0] - (mu + alphaA2T + gammaA2T)*y[6] -
	               sigma*((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] + betaB2*y[4]) +
	                      (betaA1T*y[5] + betaA2T*y[6] + betaB1T*y[7] + betaB2T*y[8]))*y[6] +
	               sigma*(
	                      (sigma/2)*(y[6]*((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] + betaB2*y[4]) +
	                                         (betaA1T*y[5] + betaA2T*y[6] + betaB1T*y[7] + betaB2T*y[8])) +
	                                (y[5] + y[6] + y[7] + y[8])*(betaA2*y[2] + betaA2T*y[6]))
	                    - (sigma*r/2)*(y[6]*(betaB1*y[3] + betaB1T*y[7]) +
	                                   y[7]*(betaA2*y[2] + betaA2T*y[6]) -
	                                   y[5]*(betaB2*y[4] + betaB2T*y[8]) -
	                                   y[8]*(betaA1*y[1] + betaA1T*y[5]))
	                     );

		dydt[7] = (tho)*(betaB1*y[3] + betaB1T*y[7])*y[0] - (mu + alphaB1T + gammaB1T)*y[7] -
	               sigma*((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] + betaB2*y[4]) +
	                      (betaA1T*y[5] + betaA2T*y[6] + betaB1T*y[7] + betaB2T*y[8]))*y[7] +
	               sigma*(
	                      (sigma/2)*(y[7]*((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] +betaB1T*y[7] + betaB2T*y[8])) +
	                                (y[5] + y[6] + y[7] + y[8])*(betaB1*y[3] + betaB1T*y[7]))
	                    - (sigma*r/2)*(y[6]*(betaB1*y[3] + betaB1T*y[7]) +
	                           y[7]*(betaA2*y[2] + betaA2T*y[6]) -
	                           y[5]*(betaB2*y[4] + betaB2T*y[8]) -
	                           y[8]*(betaA1*y[1] + betaA1T*y[5]))
	                     );

		dydt[8] = (tho)*(betaB2*y[4] + betaB2T*y[8])*y[0] - (mu + alphaB2T + gammaB2T)*y[8] -
	               sigma*((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] + betaB2*y[4]) +
	                      (betaA1T*y[5] + betaA2T*y[6] + betaB1T*y[7] + betaB2T*y[8]))*y[8] +
	               sigma*(
	                      (sigma/2)*(y[8]*((betaA1*y[1] + betaA2*y[2] + betaB1*y[3] + betaB2*y[4]) +
	                                         (betaA1T*y[5] + betaA2T*y[6] + betaB1T*y[7] + betaB2T*y[8])) +
	                                (y[5] + y[6] + y[7] + y[8])*(betaB2*y[4] + betaB2T*y[8]))
	                    + (sigma*r/2)*(y[6]*(betaB1*y[3] + betaB1T*y[7]) +
	                                   y[7]*(betaA2*y[2] + betaA2T*y[6]) -
	                                   y[5]*(betaB2*y[4] + betaB2T*y[8]) -
	                                   y[8]*(betaA1*y[1] + betaA1T*y[5]))
	                     );

		tot_steps++;
        //printFile();

	}


    void printToFile() {
    	_file << setw(15) << this->_t << setw(15);
    	for (int i=0; i<10; i++)
    	{
    		_file << y[i] << setw(15);
    	}
    	_file << endl;
    	_file.flush();
    }

};


//----------------------------------------------//
//-------------  INPUT / OUTPUT  ---------------//
//----------------------------------------------//


// Build output filename
char *build_filename()
{
	char *fname;
	char *of_name;

//#if defined (__WIN32__)
//	fname = remove_ext((char*)filename, '.', '\\');
//#else
//	fname = remove_ext((char*)filename, '.', '/');
//#endif

	of_name = (char*)malloc(FNAMELEN);

	//sprintf(of_name, "%s_N%d_mut%g.txt", fname, S+I+T, mut);
	///sprintf(of_name, "%s", fname);
	sprintf(of_name, "%s", filename);

	///free(fname);
	return of_name;
}

// Check output path
void check_dirname(char *fname)
{
	char dname[FNAMELEN];
	char *c_last;

	strcpy(dname, fname);
	c_last = strrchr(dname, '/');        // Linux path
	if (c_last != NULL) *c_last = '\0';
	else
	{
		c_last = strrchr(dname, '\\');    // Win path
		if (c_last != NULL) *c_last = '\0';
		c_last = strrchr(dname, '\\');    // Win path (case paths of form : my\\double\\slashed\\path)
		if (c_last != NULL) *c_last = '\0';
	}
	struct stat st;
	if (c_last != NULL && stat(dname,&st) != 0) // The given output path is not valid (directory doesn't exist)
	{
		cout << dname << " : output directory doesn't exist !";
		exit(0);
	}
}

// Get absolute path (OS specific)
void os_realpath(char *of_name, char *resolved_path)
{
#if defined (__WIN32__)
	char * lpPart[FNAMELEN] = { NULL };
	GetFullPathName(of_name, FNAMELEN, resolved_path, lpPart);
#else
	realpath(of_name, resolved_path);
#endif
}

int main(int argc, char *argv[])
{

	// Prepare files
	strcpy(filename, "./output1.txt");
	strcpy(rpath, "/usr/bin/r");

	char *of_name;// = output filename
    char rs_name[FNAMELEN] = "", ri_name[FNAMELEN] = "";
	ofstream myfile;

	// Build output filename
	of_name = build_filename();
	// Stop all if given output directory is a wrong one
	check_dirname(of_name);

	// Display the absolute output file path
	char resolved_path[FNAMELEN];
	os_realpath(of_name, resolved_path);
	printf("\nOutput location :\n%s\n\n", resolved_path);



	//    _t    = 0.0;      // initial time
	//    _tmax = 1200;		// time max
	//    _h    = 0.1;      // time step
	//    _hmin = 0.2;
    DrugRes *sim = new DrugRes(0.0, 1200, 0.1, 0.2);

	// Open output file
	sim->_file.open(of_name);


    struct timeval tvBegin, tvEnd, tvDiff;
    // Get begin time
    gettimeofday(&tvBegin, NULL);
    timeval_print(&tvBegin);

	sim->initialize(270, 80, 20, 0.5);
	cout << "nbins = " << sim->nbins << endl;
	sim->run_simulation();
	cout << "tot_steps = " << sim->tot_steps << endl;

	// Get end time
    gettimeofday(&tvEnd, NULL);
    timeval_print(&tvEnd);

	// Write diff time
    timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
    cout << "TOTAL SIMULATION TIME : ";
    cout << (tvDiff.tv_sec / 60) << " min. " << (tvDiff.tv_sec % 60) << " sec. " << (tvDiff.tv_usec) << " usec." << endl;


	// Close output file
	sim->_file.flush();
	sim->_file.close();


	// Generate R-script
	sprintf(rs_name, "%s%s", str_replace(remove_ext(resolved_path, '.', '/'), "\\", "/"), ".r");
	cout << "R-script saved at : " << rs_name << endl;
	if (strcmp(rpath, (char*)"") != 0)
	{
		sprintf(ri_name, "%s%s", remove_ext(rs_name, '.', '/'), ".bmp");
		cout << "R-render saved at : " << ri_name << endl;
	}

	to_r_script(rs_name, of_name, ri_name);

	// Run R-script
	if (strcmp(rpath, "") != 0)
	{
		#if defined (__WIN32__)
			char rparams[FNAMELEN];
			char rexe[FNAMELEN];
			sprintf(rexe, "\"%s\"", rpath);
			sprintf(rparams, " --slave --no-Rconsole --file=\"%s\"", rs_name);
			if ((int)ShellExecute(GetDesktopWindow(), "open", rexe, rparams, NULL, SW_HIDE/*SW_SHOWNORMAL*/) == -1)
				fprintf(stderr, "\nFailure while running R on file :\n\"%s\" !\n", rs_name);
		#else
			if (execl(rpath, rpath, rs_name, (char *)NULL) == -1)			// "/usr/bin/r"
				fprintf(stderr, "\nFailure while running R on file :\n\"%s\" !\n", rs_name);
		#endif
	}


}






//class SIR : public DiffEq_Sim {
//
//    private:
//        const double beta;
//        const double gamma;
//
//    public:
//        SIR() : beta(0.0), gamma(0.0) { nbins=3;}
//        SIR(double b, double g): beta(b), gamma(g) { nbins=3; }
//        ~SIR() {};
//
//        void initialize( double S, double I, double R) {
//            y = new double[nbins];
//            y[0] = S; y[1] = I; y[2] = R;
//        }
//
//        void derivative(double const y[], double dydt[]) {
//            dydt[0] = -beta*y[0]*y[1];
//            dydt[1] = +beta*y[0]*y[1] - gamma*y[1];
//            dydt[2] = +gamma*y[1];
//        }
//
//};
//
//int main(int argc, char *argv[])
//{
//	SIR *sim = new SIR(0.01, 0.02);
//	sim->initialize(270, 80, 20);
//	cout << "nbins = " << sim->nbins << endl;
//	sim->run_simulation();
//}





