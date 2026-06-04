/*
 * drugres.cpp
 *
 *  Created on: 8 juin 2012
 *      Author: Sylvain Gandon, Gauthier Boaglio
 */


/*************************************************************************************/
/*                  SI MODEL WITH CONSTANT HOST POPULATION SIZE                      */
/*            MUTATION AND SELECTION CHANGE THE DISTRIBULTION OF GENOTYPES           */
/*************************************************************************************/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <string.h>

#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>


#if defined (__WIN32__)
#include <windows.h>
#endif


#include "gsl/gsl_rng.h"
#include "drugres.h"

using namespace std;



//----------------------------------------------//
//-----------  GLOBALS DECLARATION  ------------//
//----------------------------------------------//

/*
 * Those variables are initialized via the 'set_globals()'
 * function and possibly user customized via the command line
 * options.
 * [See : 'parvir.h' for further description of those variables]
 * [See : 'parsecl.cpp' for more details about the way they are
 * overridden]
 */


/********  Common  *********/
double tmax;
double wstep;
int ralgo;
long int rseed;
char filename[FNAMELEN];
char rpath[FNAMELEN];

/******  Individuals  ******/
int S;			// TODO : Replace it by endemic equilibrium law
int I;			//
int T;			//

/*****  General rates  *****/
double theta; 				// Global birth rate
double mu;					// Natural per capita death rate
double tho;					// Probability to be treated
double sigma;				// Superinfection susceptibility
double rp; 					// Recombination probability
double mut;					// Mutation rate

/**  Loci-specific rates  **/
// State matrix
double VTCmtx[3][4][2];


//----------------------------------------------//
//-------------  MAIN VARIABLES  ---------------//
//----------------------------------------------//

// Size of temporary table in memory
const int PRINTSTEP = 1; //1000;

// STORAGE
int nstate, nevent, ngeno;
// Vector containing the state variables
int *y;
// TODO : replace those vectors by gI[] and gT[]
// Vector containing the density of genotypes
int *gI, *gT;
// Vector containing the propensy function
double *f;
// Vector containing the virulence for
// treated and non-treated pathogens
double *vir, *virT;
// Vector containing the transmission for
// treated and non-treated pathogens
double *beta, *betaT;
// Vector containing the clearance for
// treated and non-treated pathogens
double *clear, *clearT;
// Mean transmission, virulence and clearance
double betaMOY, virMOY, clearMOY;
double virMOY_I, virMOY_T, clearMOY_I, clearMOY_T;
double fsum;
// Frequencies
double sumI, sumT;
double ldI, ldT;




// binary (3 bits) representation of a pathogen genotype
// [c, b, a] where :
// c = t or T
// b = a or A
// a = b or B
typedef char int3;
int3 g3;


//----------------------------------------------//
//-------------  INPUT / OUTPUT  ---------------//
//----------------------------------------------//


// Build output filename
char *build_filename()
{
	char *fname;
	char *of_name;

#if defined (__WIN32__)
	fname = remove_ext((char*)filename, '.', '\\');
#else
	fname = remove_ext((char*)filename, '.', '/');
#endif

	of_name = (char*)malloc(FNAMELEN);

	sprintf(of_name, "%s_N%d_mut%g.txt", fname, S+I+T, mut);

	free(fname);
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


/*
 * Print state matrix
 */
char *print_state_mtx()
{
	char *str = (char *) malloc (4096 * sizeof(char));
	str[0] = '\0';

	sprintf(str, "%s[", str);
	for (int i=0; i < 3; i++)
	{
		sprintf(str, "%s[", str);
		for (int j=0; j < 4; j++)
		{
			sprintf(str, "%s\t[", str);
			for (int k=0; k < 2; k++)
				sprintf(str, "%s%f, ", str, VTCmtx[i][j][k]);
			str[strlen(str)-2] = '\0';
			sprintf(str, "%s]\t", str);
		}
		sprintf(str, "%s]\n", str);
	}
	str[strlen(str)-1] = '\0';
	sprintf(str, "%s]", str);
	str = str_replace(str, "\t\t", "\t");

	//printf("%s", str);
	return str;
}

// Write initial/input simulation parameters at the very
// beginning of the output file
void write_params(ofstream *myfile)
{
	*myfile << "INITIAL PARAMETERS :" << '\n';
	*myfile << "------------------" << '\n';

	// Common
	*myfile << "tmax" << setw(15) << " = " << tmax << '\n';
	*myfile << "wstep" << setw(15) << " = " << wstep << '\n';
	*myfile << "ralgo" << setw(15) << " = " << ralgo << '\n';
	*myfile << "rseed" << setw(15) << " = " << rseed << '\n';

	// Individuals
	*myfile << "S" << setw(15) << " = " << S << '\n';
	*myfile << "I" << setw(15) << " = " << I << '\n';
	*myfile << "T" << setw(15) << " = " << T << '\n';

	// General rates
	*myfile << "theta" << setw(15) << " = " << theta << '\n';
	*myfile << "mu" << setw(15) << " = " << mu << '\n';
	*myfile << "tho" << setw(15) << " = " << tho << '\n';
	*myfile << "sigma" << setw(15) << " = " << sigma << '\n';
	*myfile << "rp" << setw(15) << " = " << rp << '\n';
	*myfile << "mut" << setw(15) << " = " << mut << '\n';

	// Loci-specific rates
	*myfile << "VTCmatrix" << setw(15) << " = " << '\n' << print_state_mtx() << '\n';

	*myfile << '\n';
}

// Initialize output file results table
void write_header(ofstream *myfile)
{
	*myfile << "COMPUTING RESULTS :" << '\n';
	*myfile << "-----------------" << '\n';

	*myfile << setw(15) << "time" << setw(15) << "S" << setw(15) << "I" << setw(15) << "T";
	*myfile << setw(15) << "g0" << setw(15) << "g1" << setw(15) << "g2" << setw(15) << "g3";
	*myfile << setw(15) << "LDI" << setw(15) << "LDT";
	*myfile << setw(15) << "virMOY" << setw(15) << "betaMOY" << setw(15) << "clearMOY";
}


//----------------------------------------------//
//--------------  MISC FEATURES  ---------------//
//----------------------------------------------//


void set_globals()
{
	// Common
	tmax = 100000;
	wstep = 1000;
	ralgo = 0;
	rseed = -1;
	strcpy(filename, (char*)"output");
	//strncpy(rpath, (char*)"", FNAMELEN);
	//rpath[0] = '\0';


	// Individuals
	S = 270;			// TODO : Replace it by endemic equilibrium law
	I = 80;				/// 80
	T = 20;				/// 20

	// General rates
	theta = 10.0;
	mu = .01;///.01;
	tho = 0.8;///0.8;
	sigma = 1;
	rp  = 0.2;
	mut = 0.01;///0.001;

	// Loci-specific rates
	// Default state matrix
	/*
	 * 							  Default     A effect     B effect     Epistasis
	 * 						 	 ___________________________________________________
	 * 	Virulence 		[I,T]	| [a, aT]     [daA, daAT]  [daB, daBT]  [dae, daeT]
	 * 	Transmission	[I,T]	| [b, bT]     [dbA, dbAT]  [dbB, dbBT]  [dbe, dbeT]
	 * 	Clearance		[I,T]	| [c, cT]     [dcA, dcAT]  [dcB, dcBT]  [dce, dceT]
	 *
	 */
	 //-b [[.00007,.00003],[-.000001,.000001],[-.000001,.000001],[0.0,0.0]]
	double dmatrix[3][4][2] = {
			{ {0.005, 	0.005}, 		{0.00, 		0.00}, 		{0.00, 		0.00}, 		{0.0,	0.0} },
			//////{ {.0007, 	.0003}, 		{.0001, 	.0002}, 	{.0001, 	.0002}, 	{0.0,	0.0} },
			{ {.00007, 	.00003}, 		{-.00001, 	.00002}, 	{-.00001, 	.00002}, 	{0.0,	0.0} },

			//{ {.000003, 	.000002}, 		{-.0000001, 	.0000001}, 	{-.0000001, 	.0000001}, 	{0.0,	0.0} },
			///{ {mu + .01, 	mu + .01}, 	{0, 		0.00}, 		{0, 		0.00}, 		{0.0,	0.0} }
			{ {0.005, 	0.005}, 	{0, 		0.00}, 		{0, 		0.00}, 		{0.0,	0.0} }
	};
	memcpy(VTCmtx, dmatrix, sizeof(dmatrix));
}


void set_state_matrix(double mtx[3][4][2])
{

	// Virulence				[ex : treatment lowers virulence]
	// Not-Treated / vir[Iab, IAb, IaB, IAB]
	vir[0] = mtx[0][0][0];
	vir[1] = vir[0] + mtx[0][1][0];
	vir[2] = vir[0] + mtx[0][2][0];
	vir[3] = vir[0] + mtx[0][1][0] + mtx[0][2][0] + mtx[0][3][0];
	// Treated / virT[Tab, TAb, TaB, TAB]
	virT[0] = mtx[0][0][1];
	virT[1] = virT[0] + mtx[0][1][1];
	virT[2] = virT[0] + mtx[0][2][1];
	virT[3] = virT[0] + mtx[0][1][1] + mtx[0][2][1] + mtx[0][3][1];

	// Transmission				[ex : treatment lowers transmission]
	// Not-Treated / beta[Iab, IAb, IaB, IAB]
	beta[0] = mtx[1][0][0];
	beta[1] = beta[0] + mtx[1][1][0];
	beta[2] = beta[0] + mtx[1][2][0];
	beta[3] = beta[0] + mtx[1][1][0] + mtx[1][2][0] + mtx[1][3][0];
	// Treated / betaT[Tab, TAb, TaB, TAB]
	betaT[0] = mtx[1][0][1];
	betaT[1] = betaT[0] + mtx[1][1][1];
	betaT[2] = betaT[0] + mtx[1][2][1];
	betaT[3] = betaT[0] + mtx[1][1][1] + mtx[1][2][1] + mtx[1][3][1];

	// Clearance				[ex : treatment uppers clearance]
	// Not-Treated / clear[Iab, IAb, IaB, IAB]
	clear[0] = mtx[2][0][0];
	clear[1] = clear[0] + mtx[2][1][0];
	clear[2] = clear[0] + mtx[2][2][0];
	clear[3] = clear[0] + mtx[2][1][0] + mtx[2][2][0] + mtx[2][3][0];
	// Treated / clearT[Tab, TAb, TaB, TAB]
	clearT[0] = mtx[2][0][1];
	clearT[1] = clearT[0] + mtx[2][1][1];
	clearT[2] = clearT[0] + mtx[2][2][1];
	clearT[3] = clearT[0] + mtx[2][1][1] + mtx[2][2][1] + mtx[2][3][1];

	//for(int i=0;i<ngeno;i++) vir[i]=(virmin+(virmax-virmin)*i/(ngeno-1));
	//for(int i=0;i<ngeno;i++) {beta[i]=((b0*(pow((vir[i]),coef)))/N);};	//// TRADEOFF SHAPE

}


char *get_geno_name(int g)
{
	char *ret;
	switch (g)
	{
	case 0: ret = (char*)"ab"; break;
	case 1: ret = (char*)"aB"; break;
	case 2: ret = (char*)"Ab"; break;
	case 3: ret = (char*)"AB"; break;
	}
	return ret;
}

/*
 * Update the propensy means
 */
void update_fmeans()
{
	double nb_infect = (double)(y[1] + y[2]);
	double nb_I = (double)(y[1]);
	double nb_T = (double)(y[2]);

	virMOY = 0;
	virMOY_I = 0;
	virMOY_T = 0;
	betaMOY = 0;
	clearMOY = 0;
	clearMOY_I = 0;
	clearMOY_T = 0;

	for(int i=0;i<ngeno;i++)
	{
		if (y[1] + y[2] != 0)
		{
			virMOY += (vir[i]*gI[i] + virT[i]*gT[i]) / nb_infect;
			betaMOY += (beta[i]*gI[i] + betaT[i]*gT[i]) / nb_infect;
			clearMOY += (clear[i]*gI[i] + clearT[i]*gT[i]) / nb_infect;
		}
		if (y[1] != 0)
		{
			virMOY_I += (vir[i]*gI[i]) / nb_I;
			clearMOY_I += (clear[i]*gI[i]) / nb_I;
		}
		if (y[2] != 0)
		{
			virMOY_T += (virT[i]*gT[i]) / nb_T;
			clearMOY_T += (clearT[i]*gT[i]) / nb_T;
		}
	}
}


// Compute propensies
void update_fprops()
{
	f[0] = theta;									// Birth (constant rate of immigration)
	f[1] = mu*y[0];	 								// Natural Death
	f[2] = betaMOY*(y[1]+y[2])*y[0];	 			// Infection
	f[3] = (mu + virMOY_I)*(y[1]);	 				// Death by infection
	f[4] = (mu + virMOY_T)*(y[2]);	 				// Death by infection
	f[5] = clearMOY_I*(y[1]);						// Clearance
	f[6] = clearMOY_T*(y[2]);						// Clearance
	f[7] = sigma*(y[1]+y[2])*betaMOY*(y[1]+y[2]);	// Strain change (by replacement or recombination)
	f[8] = mut*(y[1]+y[2]); 						// Mutation

	fsum = 0;
	for(int i=0; i<(nevent); i++) fsum += f[i];
}


void print_rates()
{
	cout << "[ " << f[0] << " " << f[1] + f[3] + f[4] << " " << f[2] << " " << f[5] + f[6] << " " << f[7] << " " << f[8] << " ]" << endl;
}

// Compute LD (Linkage Disequilibrium)
void update_ld()
{
	sumI = gI[0] + gI[1] + gI[2] + gI[3];
    sumT = gT[0] + gT[1] + gT[2] + gT[3];
    if (sumI == 0.0) ldI = 0.0;
    else ldI = ((gI[0] + gI[3]) / (double)(sumI*sumI)) - ((gI[1] + gI[2]) / (double)(sumI*sumI));
    if (sumT == 0.0) ldT = 0.0;
    else ldT = ((gT[0] + gT[3]) / (double)(sumT*sumT)) - ((gT[1] + gT[2]) / (double)(sumT*sumT));
}

/*int g_from_mask(int m) // Not working yet...
{
	int k = 4;
	do
	{
		if ((m & ( 1 << k )) >> k != 0) return k;
		else k--;
	} while (k > 0);
}
 */

// Full mask = 1111 = 2^3+2^2+2^1+2^0 = 8+4+2+1 = 15
// Ex : g=1 => 0010 = 2^1 = 2
int g_to_mask(int g)
{
	return 1 << g;
}

//----------------------------------------------//
//---------------  MAIN ROUTINE  ---------------//
//----------------------------------------------//
int main(int argc, char *argv[])
{
	try
	{

		// ----------------------------  Sim vars  -------------------------------- //

		// The time expressed continuously
		double t = 0;
		// The time expressed discretely
		double tout = 0;

		// ----------------------------  Misc vars  -------------------------------- //

		char *of_name;// = output filename

		int i, j;
		int g1, g2;
		int c;

		int evt;
		int k = 0;
		double p1, p2, p3, p4, p5, rp2;	// Probabilities
		double fprop, fgeno;
		int geno = 0;

	    struct timeval tvBegin, tvEnd, tvDiff;

	    char rs_name[FNAMELEN] = "", ri_name[FNAMELEN] = "";


	    //-------
	    int nb_fired_evt = 0;
	    double dt = 0.0;

		// ************************************************************************ //
		//                         LET'S GET INITIALIZED !                          //
		// ************************************************************************ //

		// Print program version
		pv_header();

		// ----------------------  Variables and storage  ------------------------- //

		// Initialize global variables
		set_globals();

		// Parsing input parameters
		parsecl(argc,argv);

		// Initialize storage vectors
		nstate 	= 3;			// Susceptible, infected, treated
		nevent 	= 9;			// Birth, infection, death by infection, clearance, superinfection, mutation
		ngeno 	= 4;			// Pathogen can be ab=0=>0001, Ab=1=>0010, aB=2=>0100, AB=3=>1000

		y = new int[nstate];
		gI = new int[ngeno];
		gT = new int[ngeno];
		f = new double[nevent];
		vir = new double[ngeno];
		virT = new double[ngeno];
		beta = new double[ngeno];
		betaT = new double[ngeno];
		clear = new double[ngeno];
		clearT = new double[ngeno];


		// -------------------------  Setup randomizer  --------------------------- //

		// Initialize randomizer
		gsl_rng * r;

		// Choose randomizer
		switch (ralgo)
		{
		case 0: r = gsl_rng_alloc(gsl_rng_mt19937); break;
		case 1: r = gsl_rng_alloc(gsl_rng_taus); break;
		case 2: r = gsl_rng_alloc(gsl_rng_gfsr4); break;
		}

		// Generate a seed if necessary
		if (rseed == -1)
		{
			// Get a different seed at each program run using the system current time
			// Otherwise : use the default fixed one
			srand(time(NULL));
			rseed = rand();
		}
		// Setup randomizer
		gsl_rng_set(r, rseed);



		// ************************************************************************ //
		//                               LET'S GO !                                 //
		// ************************************************************************ //


		// --------------------------  Prepare file  ------------------------------ //

		ofstream myfile;

		// Build output filename
		of_name = build_filename();
		// Stop all if given output directory is a wrong one
		check_dirname(of_name);

		// Display the absolute output file path
		char resolved_path[FNAMELEN];
		os_realpath(of_name, resolved_path);
		printf("\nOutput location :\n%s\n\n", resolved_path);

		// Open the file
		myfile.open(of_name);

		// Setup run informations
		write_params(&myfile);
		// Results header line
		write_header(&myfile);

		// Temporary tables to store data before writing to disk
		double* thetime;
		thetime = new double[PRINTSTEP];

		double **temp;
		temp = new double*[PRINTSTEP];
		for(i=0; i<=PRINTSTEP-1; i++) temp[i] = new double[nstate+5+ngeno];

		fprintf(stdout, "\n/*==========================  <Processed Data>  ==========================*/\n\n");

		// ---------------------------  PROCESSING  ------------------------------ //

	    // Get begin time
	    gettimeofday(&tvBegin, NULL);
	    timeval_print(&tvBegin);

		// Initialization of the state variables

		int killedI = 0, killedT = 0;
		int clearedI = 0, clearedT = 0;

		set_state_matrix(VTCmtx);

		y[0] = S; //RND((mu+vir[ngeno/2])/(beta[ngeno/2]));		// S at endemic equilibrium  /*** TODO ***/
		y[1] = I; //N-y[0];										// I at endemic equilibrium
		y[2] = T;


		// Initialization of the genotypes data we're going to start with
		for(i=0;i<ngeno;i++) gI[i]=0;				// Set geno count to 0
		for(i=0;i<ngeno;i++) gT[i]=0;				// Set geno count to 0
		gI[0] = RND((y[1] + y[2])*(1-tho));// / 8;
//		gI[1] = gI[0];
//		gI[2] = gI[0];
//		gI[3] = gI[0];
////		gT[0] = RND((y[1] + y[2])*tho) / 8;
		gT[0] = (y[1] + y[2]) - gI[0];
//		gT[1] = gT[0];
//		gT[2] = gT[0];
//		gT[3] = gT[0];

//		gI[0] = 4 * RND((y[1] + y[2])*(1-tho)) / 8;
//		gI[1] = gI[0] * 2;
//		gI[2] = gI[0] * 2;
//		gI[3] = 0;
//		gT[0] = 4 * RND((y[1] + y[2])*tho) / 8;
//		gT[1] = gT[0] * 2;
//		gT[2] = gT[0] * 2;
//		gT[3] = 0;

		// Initialization of the propensy stuffs
		update_fmeans();
		update_fprops();
		update_ld();

		// Print some outputs
		// Print very first line
		myfile << "\n";
		myfile << setw(15) << t << setw(15) << y[0] << setw(15) << y[1] << setw(15) << y[2];
		for (i=0; i<ngeno; i++) myfile << setw(15) << (gI[i] + gT[i]);
        myfile << setw(15) << ldI << setw(15) << ldT;
        myfile << setw(15) << virMOY << setw(15) << betaMOY << setw(15) << clearMOY;
		myfile << "\n";
		myfile.flush();


		//--------------------------------------------------------------------//
		//--------------------------  <Simulation>  --------------------------//
		//--------------------------------------------------------------------//

		t = 0;								// Reset time

		// Managing the change of state
		// Running the simulation until tmax
		do
		{
			nb_fired_evt++;

			// Generate 2 random numbers:
			p1 = gsl_rng_uniform(r);//ARandom(); 		//time
			p2 = gsl_rng_uniform(r);//ARandom(); 		//event

			// (1) Determine time to next event and update time:
			if (p1 == 0.0) p1 = 0.0000000001;

			dt = -log(p1)/fsum;

			if (fsum > 0.0) t += -log(p1)/fsum;
			else break;

			// (2) Determine the nature of next event:
			fprop = 0;
			for (i=0; i < (nevent); i++)
			{
				fprop += f[i];
				if (p2 < (fprop/fsum)) { evt = i; break; };
			}

			// (3) Update state variables and propensy function
			switch (evt)
			{

			case 0:		// ***  <Birth>  *** //

				// Birth of a new fresh healthy one
				////double pb = gsl_rng_uniform(r);
				////if (pb < (theta - theta * y[0]/(y[0]+y[1]))) y[0]++;	// Keep constant the hosts pop if no infection
				y[0]++;														// Non-constant hosts pop way of doing

				break;


			case 1:		// ***  <Natural Death>  *** //

				// Death of an healthy one
				if (y[0] > 0) y[0]--;

				break;


			case 2:		// ***  <Infection>  *** //

				if (y[0] > 0)
				{

					// Decrease by one the number of healthy ones
					y[0]--;

					// Choose the pathogen genotype
					p3 = gsl_rng_uniform(r);
					fgeno = 0;
					for (i=0; i<ngeno; i++)
					{
						//if (th)
						fgeno += (beta[i]*gI[i] + betaT[i]*gT[i]) / (betaMOY*(y[1]+y[2]));
						//else
						//	fgeno += (beta[i]*g[i])/(betaMOY*(y[1]+y[2]));
						if (p3 < (fgeno)) { geno = i; break; };
					}

					// Choose to work on a treated or untreated host
					p4 = gsl_rng_uniform(r);
					if (p4 < tho)					// T
					{
						y[2]++;																// Add a treated one
						gT[geno]++;
					}
					else							// I
					{
						y[1]++;																// Add a non-treated one
						gI[geno]++;
					}

				}


				break;


			case 3:		// ***  <Death by infection I>  *** //

				// Choose the pathogen genotype
				p4 = gsl_rng_uniform(r);
				fgeno = 0;
				for (i=0; i<ngeno; i++)
				{
					fgeno += (vir[i]*gI[i]) / (virMOY_I*(y[1]+y[2]));
					if (p4 < (fgeno)) { geno = i; break; };
				}


				// Update counts
				if (y[1] > 0 && gI[geno] > 0)
				{
					y[1]--;
					killedI++;																	// Add a non-treated one
					gI[geno]--;
				}


				break;


			case 4:		// ***  <Death by infection T>  *** //

				// Choose the pathogen genotype
				p4 = gsl_rng_uniform(r);
				fgeno = 0;
				for (i=0; i<ngeno; i++)
				{
					fgeno += (virT[i]*gT[i]) / (virMOY_T*(y[1]+y[2]));
					if (p4 < (fgeno)) { geno = i; break; };
				}


				// Update counts
				if (y[2] > 0 && gT[geno] > 0)
				{
					y[2]--;
					killedT++;																// Add a treated one
					gT[geno]--;
				}


				break;


			case 5:		// ***  <Clearance I>  *** //

				// Choose the pathogen genotype
				p4 = gsl_rng_uniform(r);
				fgeno = 0;
				for (i=0; i<ngeno; i++)
				{
					fgeno += (clear[i]*gI[i]) / (clearMOY_I*(y[1]));
					if (p4 < (fgeno)) { geno = i; break; };
				}

				// Update counts
				if (y[1] > 0 && gI[geno] > 0)
				{
					y[0]++; 				// New susceptible one (cleared goes back to susceptible state)
					y[1]--;					// Remove a treated one
					clearedI++;
					gI[geno]--;
				}


				break;


			case 6:		// ***  <Clearance T>  *** //

				// Choose the pathogen genotype
				p4 = gsl_rng_uniform(r);
				fgeno = 0;
				for (i=0; i<ngeno; i++)
				{
					fgeno += (clearT[i]*gT[i]) / (clearMOY_T*(y[2]));
					if (p4 < (fgeno)) { geno = i; break; };
				}

				// Update counts
				if (y[2] > 0 && gT[geno] > 0)
				{
					y[0]++; 				// New susceptible one (cleared goes back to susceptible state)
					y[2]--;					// Remove a treated one
					clearedT++;
					gT[geno]--;
				}


				break;


			case 7:		// ***  <Superinfection>  *** //

				// Choose the resident strain
				p3 = gsl_rng_uniform(r);
				fgeno = 0;
				for (i=0; i<ngeno; i++)
				{
					fgeno += (gI[i] + gT[i]) / (double)(y[1] + y[2]);
					if (p3 < (fgeno)) { g1 = i; break; };
				}
				// Choose the superinfecting strain (superinfection is beta-dependent)
				// TODO : ici il faudrait faire -1 sur le group (gI ou gT) quand c'est le group de l'individu choisi ci-dessus
				//        car un individu ne peut pas se superinfecter lui-meme...
				p4 = gsl_rng_uniform(r);
				fgeno = 0;
				for (i=0; i<ngeno; i++)
				{
					fgeno += (beta[i]*gI[i] + betaT[i]*gT[i]) / (betaMOY*(y[1]+y[2]));
					if (p4 < (fgeno)) { g2 = i; break; };
				}


				if (g1 != g2)
				{
					// Combine g1 and g2
					// Ex : g1=1=0010 and g2=3=1000 => c=1010=2^3+2^1=10
					c = g_to_mask(g1) | g_to_mask(g2);

					// Choose the winner strain
					geno = g1;
					p5 = gsl_rng_uniform(r);
					if (c == 3 || c == 5 || c == 10 || c == 12)				// c=0011=3 or c=0101=5 or c=1010=10 or c=1100=12
					{
						if (p5 < 0.5) geno = g2;
					}
					else
					{
						if (c == 9 || c == 6)								// c=1001=9 or c=0110=6
						{
							if (c == 6) rp2 = rp; else rp2 = (1-rp);		// Switch rp for the extremes (c=1001)
							if (p5 < rp2/2.0) 						geno = 0;
							else if (p5 < rp2/2.0 + (1-rp2)/2.0) 	geno = 1;
							else if (p5 < rp2/2.0 + (1-rp2)) 		geno = 2;
							else 						 			geno = 3;
						}
					}

					if (g1 != geno)
					{
						// Choose to work on a treated or untreated host
						if (gI[g1] + gT[g1] != 0)
						{
							p4 = gsl_rng_uniform(r);
							// Update treated ones proportions
							if (p4 < gT[g1] / (double)(gI[g1] + gT[g1]))		// T
							{
								if (gT[g1] > 0)
								{
									gT[g1]--;
									gT[geno]++;
								}
							}
							else												// I
							{
								if (gI[g1] > 0)
								{
									gI[g1]--;
									gI[geno]++;
								}
							}
						}
					}
				}

				break;

			case 8:		// ***  <Mutation>  *** //

				// Choose the pathogen genotype
				p3 = gsl_rng_uniform(r);
				fgeno = 0;
				for (i=0; i<ngeno; i++)
				{
					fgeno += (gI[i] + gT[i]) / (double)(y[1] + y[2]);
					if (p3 < (fgeno)) { g1 = i; break; };
				}

				// Choose the new mutation
				p4 = gsl_rng_uniform(r);
				if (g1 == 0 || g1 == 3)			// Extremes : choose one among the middles
				{
					if (p4 < 0.5) g2 = 1;
					else g2 = 2;
				}
				else if (g1 == 1 || g1 == 2)	// Middles : choose one among the extremes
				{
					if (p4 < 0.5) g2 = 0;
					else g2 = 3;
				}

				// Choose to work on a treated or untreated host
				if (gI[g1] + gT[g1] != 0)
				{
					p5 = gsl_rng_uniform(r);
					// Update treated ones proportions
					if (p5 <  gT[g1] / (double)(gI[g1] + gT[g1]))		// T
					{
						if (gT[g1] > 0)
						{
							gT[g1]--;
							gT[g2]++;
						}
					}
					else												// I
					{
						if (gI[g1] > 0)
						{
							gI[g1]--;
							gI[g2]++;
						}
					}
				}

				break;

			}

			// (4) Update the propensy stuffs
			update_fmeans();

			update_fprops();
//			f[0] = theta;
//			f[1] = mu*y[0];
//			f[2] = betaMOY*(y[1]+y[2])*y[0]; 				//f[1] = betaMOY*y[1]*y[0];
//			f[3] = (mu + virMOY_I)*(y[1]); 					//f[2] = (mu+virMOY)*y[1];
//			f[4] = (mu + virMOY_T)*(y[2]); 					//f[2] = (mu+virMOY)*y[1];
//			f[5] = clearMOY_I*(y[1]); 						//f[5] = clearMOY*y[1];
//			f[6] = clearMOY_T*(y[2]); 						//f[5] = clearMOY*y[1];
//			f[7] = sigma*(y[1]+y[2])+betaMOY*(y[1]+y[2]); 	//f[5] = sigm*y[1]*betaMOY*y[1];
//			f[8] = mut*(y[1]+y[2]); 						//f[4] = mut*y[1];
//
//			fsum = 0;
//			for (i=0; i<(nevent); i++) fsum += f[i];


			// Parasite extinction
			if (y[1]+y[2] == 0)
			{
				fprintf(stderr, "Extinction at t=%f", t);
				exit(0);
			}


			// Writing time and state variables values to file
			if (t > tout + wstep)
			{

			    update_ld();

			    // Write all
			    cout << "time:"  << setw(15) << t << setw(15) << "evts : " << nb_fired_evt << setw(15) << " | dt : " << dt << setw(15) << " | ter : " << fsum << setw(15) << betaMOY << setw(15) << "\n" << y[0] << setw(15)
								<< y[1] << "(" << killedI << ")" << "(" << clearedI << ")" << setw(15)
								<< y[2] << "(" << killedT << ")" << "(" << clearedT << ")" << setw(15)
								<< "(" << gI[0]+gT[0] << ", " << gI[1]+gT[1] << ", " << gI[2]+gT[2] << ", " << gI[3]+gT[3] << ")" << setw(15)
								<< virMOY << setw(20) << betaMOY << setw(20) << clearMOY << setw(20) << "\n";

//			    if (t > 10000.0) exit(0);
//			    print_rates();

			    flush(cout);

				if (k >= PRINTSTEP)
				{
					for (i=0; i<PRINTSTEP; i++)
					{
						myfile << setw(15) << thetime[i] << setw(15);
						myfile << RND(temp[i][0]) << setw(15) << RND(temp[i][1]) << setw(15) << RND(temp[i][2]);
						for (j=0; j<ngeno; j++) { myfile << setw(15) << RND(temp[i][nstate+5+j]); }
						myfile << setw(15) << temp[i][6]  << setw(15) << temp[i][7];
						myfile << setw(15) << temp[i][3] << setw(15) << temp[i][4] << setw(15) << temp[i][5];
						myfile << "\n";
						myfile.flush();
					}
					k = 0;
				}

				thetime[k] = t;
				for (i=0; i<nstate; i++) temp[k][i] = y[i];
				temp[k][3] = virMOY;
				temp[k][4] = betaMOY;
				temp[k][5] = clearMOY;
				temp[k][6] = ldI;
				temp[k][7] = ldT;
				for (j=0; j<ngeno; j++) temp[k][nstate+5+j] = (gI[j] + gT[j]);
				k++;

				tout += wstep;
			}

			//fprintf(stdout, "%f\t|\t%f\t|\t%d\n", t, tmax, (t < tmax));
			//			cout << "time:" << setw(10) << t << setw(10) << y[0] << setw(10) << y[1] << setw(10) << y[2] << setw(10)
			//												<< virMOY << setw(10) << betaMOY << setw(10) << clearMOY << setw(10) << "\n";

			//cout << t;
		} while (t < tmax);


		// Writing the time and state variables values left in memory to file before finishing
		for (i=0; i<k; i++)
		{
			myfile << setw(15) << thetime[i];
			myfile << setw(15) << RND(temp[i][0]) << setw(15) << RND(temp[i][1]) << setw(15) << RND(temp[i][2]);
			for (j=0; j<ngeno; j++) { myfile << setw(15) << RND(temp[i][nstate+5+j]); }
			myfile << setw(15) << temp[i][6]  << setw(15) << temp[i][7];
			myfile << setw(15) << temp[i][3] << setw(15) << temp[i][4] << setw(15) << temp[i][5];
			myfile << "\n";
		    myfile.flush();
		}

		// Free temp buffer
		for(i=0; i<=PRINTSTEP-1; i++) free(temp[i]);
		free(temp);

		// Get end time
	    gettimeofday(&tvEnd, NULL);
	    timeval_print(&tvEnd);

		// Write diff time
	    timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
	    cout << "TOTAL SIMULATION TIME : ";
	    cout << (tvDiff.tv_sec / 60) << " min. " << (tvDiff.tv_sec % 60) << " sec. " << (tvDiff.tv_usec) << " usec." << endl;

		// Close output file
	    myfile.flush();
		myfile.close();



		// Free randomizer
		gsl_rng_free(r);


		/*
		 * - Generate R-script
		 * - Run R-script if R-binary path was specified
		 *       (generating the resulting plot image).
		 */


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
	catch (char *stri)
	{
		cout << "Error : " << stri << '\n';
		exit(-1);
	}

	return 0;
}
