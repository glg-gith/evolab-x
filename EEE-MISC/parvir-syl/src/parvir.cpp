/*
 * parvir.cpp
 *
 *  Created on: 8 juin 2012
 *      Author: Sylvain Gandon, Gauthier Boaglio
 */


/*************************************************************************************/
/*                  SI MODEL WITH CONSTANT HOST POPULATION SIZE                      */
/*            MUTATION AND SELECTION CHANGE THE DISTRIBULTION OF GENOTYPES           */
/*************************************************************************************/

#include <iostream>	// For cin, cout, cerr, clog
#include <fstream>	// For ofstream
#include <iomanip>	// For setw
#include <cmath>	// For log, pow
#include <cstring>	// For strcomp
#include <string.h>

#include <sstream>

#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#if defined (__WIN32__)
#include <windows.h>
#endif


#include "gsl_rng/gsl_rng.h"
//#include <gsl/gsl_rng.h>
#include "parvir.h"

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

/**********  Common  **********/
int fix_mode;
double virmin, virmax;	// alpha bounds
int tradeoff;
double b0;
double coef;
double w0;
double viropt;
int N;
double tmax;
double wstep;
int ralgo;
long int rseed;
char filename[FNAMELEN];
/*******  Mutation mode  *******/
int ngeno;
double mut;
/*******  Fixation mode  *******/
int repmax;
double virstep;			// alpha step
double virdelta;		// alpha delta
double tmut;



//----------------------------------------------//
//-------------  MAIN VARIABLES  ---------------//
//----------------------------------------------//

// Natural death
double mu = 1;

// Size of temporary table in memory
int PRINTSTEP = 1;

// STORAGE
// y is the vector containing the state variables
int *y;
// g is the vector containing the density of genotypes
int *g;
// f is the vector containing the propensy function
double *f;
// vir is the vector containing the virulence
double *vir;
// beta is the vector containing the transmission
double *beta;

// mean transmission and virulence
double betaMOY, virMOY;




//----------------------------------------------//
//----------  Old Mersenne Twister  ------------//
//----------------------------------------------//

/** from Numerical Recipes in C, Price et al. **/

// Old Mersenne Twister (use GSL mt19937 instead -
// which is a lot more stable and faster with a low
// iterations number), where 'stable' means the
// computation time is quite constant whatever
// the random seed is.
double AR1= 36575.0;
double AR2=196831;
double ARandom(void)
{
	long   i;
	double r, z;

	r = AR1 / 53668;
	i = (long)r;
	r = (double)i;
	AR1 = 40014 * (AR1 - r * 53668) - r * 12211;
	if (AR1 < 0)
		AR1 += 2147483563;
	r = AR2 / 52774;
	i = (long)r;
	r = (double)i;
	AR2 = 40692 * (AR2 - r * 52774) - r * 3791;
	if (AR2 < 0)
		AR2 += 2147483399;
	z = AR1 - AR2;
	if (z < 1)
		z += 2147483562;
	return z * 4.656613e-10;
}



//----------------------------------------------//
//-------------  INPUT / OUTPUT  ---------------//
//----------------------------------------------//


// Build output filename
char *build_filename()
{
	char *fname;
	char *of_name;

	remove_delimiters(filename, (char*)"\"");

#if defined (__WIN32__)
	fname = remove_ext((char*)filename, '.', '\\');
#else
	fname = remove_ext((char*)filename, '.', '/');
#endif

	of_name = (char*)malloc(FNAMELEN);

	if (fix_mode == 0)
		sprintf(of_name, "%s_N%d_mut%g_b%g_coef%g.txt", fname, N, mut, b0, coef);
	else
		sprintf(of_name, "%s_N%d_mut%g_b%g_coef%g.txt", fname, N, tmut, b0, coef);

	free(fname);
	return of_name;
}

// Check output path
void check_dirname(char *fname)
{
	char dname[FNAMELEN];
	char *c_last;

	strncpy(dname, fname, FNAMELEN);
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

// Write initial/input simulation parameters at the very
// beginning of the output file
void write_params(ofstream *myfile)
{
	*myfile << "INITIAL PARAMETERS :" << '\n';
	*myfile << "------------------" << '\n';

	/**********  Common  **********/
	*myfile << "virmin" << setw(15) << " = " << virmin << '\n';
	*myfile << "virmax" << setw(15) << " = " << virmax << '\n';
	*myfile << "b0" << setw(15) << " = " << b0 << '\n';
	*myfile << "coef" << setw(15) << " = " << coef << '\n';
	*myfile << "N" << setw(15) << " = " << N << '\n';
	*myfile << "tmax" << setw(15) << " = " << tmax << '\n';
	*myfile << "wstep" << setw(15) << " = " << wstep << '\n';
	*myfile << "ralgo" << setw(15) << " = " << ralgo << '\n';
	*myfile << "rseed" << setw(15) << " = " << rseed << '\n';

	/*******  Mutation mode  *******/
	if (fix_mode == 0)
	{
		*myfile << "ngeno" << setw(15) << " = " << ngeno << '\n';
		*myfile << "mut" << setw(15) << " = " << mut << '\n';
	}
	/*******  Fixation mode  *******/
	else
	{
		*myfile << "repmax" << setw(15) << " = " << repmax << '\n';
		*myfile << "virstep" << setw(15) << " = " << virstep << '\n';
		*myfile << "virdelta" << setw(15) << " = " << virdelta << '\n';
		*myfile << "tmut" << setw(15) << " = " << tmut << '\n';
	}

	*myfile << '\n';
}

// Initialize output file results table
void write_header(ofstream *myfile)
{
	*myfile << "               \nCOMPUTING RESULTS :" << '\n';
	*myfile << "-----------------" << '\n';

	/*******  Mutation mode  *******/
	if (fix_mode == 0)
		*myfile << setw(15) << "time" << setw(15) << "S" << setw(15) << "I" << setw(15) << "virMOY" << "\n";
	/*******  Fixation mode  *******/
	else
		*myfile << setw(15) << "Virulence" << setw(15) << "fixation" << setw(15) << "fixation RES" << setw(15) << "Pfix Res" << setw(15) << "NO fixation" << setw(15) << "NO extinction" << setw(15) << "Res extinction" << setw(15) << "Mut extinction" << "\n";
}


//----------------------------------------------//
//--------------  MISC FEATURES  ---------------//
//----------------------------------------------//

void set_globals()
{
	/**********  Common  **********/
	fix_mode = 0;
	virmin = 0.0;
	virmax = 6.0;
	tradeoff = 0;
	b0 = 6;
	coef = 0.5;
	w0 = 1;
	viropt = 3.0;
	N = 100;
	tmax = 10000000;
	wstep = 1000;
	ralgo = 0;
	rseed = -1;
	strncpy(filename, (char*)"output", FNAMELEN);
	/*******  Mutation mode  *******/
	ngeno = 100;
	mut = 0.001;
	/*******  Fixation mode  *******/
	repmax = 100000;
	virstep = 1.0;
	virdelta = 0.1;
	tmut = 500;
}


// Function that updates the virMOY function:
double fvir()
{
	virMOY = 0;
	for(int i=0;i<ngeno;i++) virMOY += g[i]*vir[i]/(double)y[1];
	return(virMOY);
}

// Function that updates the betaMOY function:
double fbeta()
{
	betaMOY = 0;
	for(int i=0;i<ngeno;i++) betaMOY += g[i]*beta[i]/(double)y[1];
	return(betaMOY);
}


//----------------------------------------------//
//---------------  MAIN ROUTINE  ---------------//
//----------------------------------------------//
int main(int argc, char *argv[])
{
	try
	{

		// ----------------------------  Sim vars  -------------------------------- //

		int nstate, nevent;
		// t is the time expressed continuously
		double t = 0;
		// tout is the time expressed discretely
		double tout = -wstep;	// Always print the first line

		// ----------------------------  Misc vars  -------------------------------- //

		char *of_name;// = output filename

		int evt;
		int k = 1;
		double p1, p2, p3, p4, fprop, fgeno;	// p1 and p2 are probabilities
		double fsum = 0;
		int geno = 0;
		double virMOY, betaMOY;

		int rep;
		int indexfix = 0, index = 0, indexnofix = 0, indexnoext = 0, indexmutext = 0, indexresext = 0;
		double virres, virmut, probafix;
		const char *NAN_probafix = "-";
		bool mutant_introduced = false;

		char *s_warn = (char*)"";

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
		nstate = 2;				// Healthy, infected
		if (fix_mode == 0)
			nevent = 3;			// Infection, death by infection, mutation
		else {
			nevent = 2;			// Infection, death by infection
			ngeno = 2;			// Pathogen can only be the resident or the mutant one
		}
		y = new int[nstate];
		g = new int[ngeno];
		f = new double[nevent];
		vir = new double[ngeno];
		beta = new double[ngeno];

	    struct timeval tvBegin, tvEnd, tvDiff;

	    bool extinct = false;
	    int prev_g = 0;

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
		thetime=new double[PRINTSTEP];

		double** temp;
		temp=new double*[PRINTSTEP];
		for(int i=0;i<PRINTSTEP;i++)temp[i]=new double[nstate+1+ngeno];


		// ---------------------------  PROCESSING  ------------------------------ //

	    // Get begin time
	    gettimeofday(&tvBegin, NULL);
	    ////timeval_print(&tvBegin);

		// Initialization of the processing variables
		virres = virmin; // (fixation mode)

		myfile.close();

		do // Loop on 'alphares'						// Only one iteration in mutation mode (fix_mode = 0)
		{
			if (fix_mode != 0) // Fixation mode
			{
				virmut = virres + virdelta;
				indexfix = 0;
				index = 0;
				indexnofix = 0;
				indexnoext = 0;
				indexresext = 0;
				indexmutext = 0;

				cout << "<Virulence : " << virres << ">\n";
			}

			rep = 0;
			do // Loop on repetitions (rep)				// Only one iteration in mutation mode (fix_mode = 0)
			{
				rep++;

				// Initialization of the state variables
				if (fix_mode == 0)						// Mutation mode
				{
					for(int i=0; i<ngeno; i++) vir[i]=(virmin+(virmax-virmin)*i/(double)(ngeno-1));

					switch (tradeoff)	//// TRADEOFF SHAPE
					{
					case 0: for(int i=0; i<ngeno; i++) { beta[i]=((b0*(pow((vir[i]),coef)))/(double)N); }; break;
					case 1: for(int i=0; i<ngeno; i++) { beta[i]=(vir[i]+mu)*(b0-w0*(pow(abs(vir[i]-viropt),coef)))/(double)N; }; break;
					}

					y[0] = RND((mu+vir[ngeno/2])/(beta[ngeno/2]));		// S at endemic equilibrium
					y[1] = N-y[0];										// I at endemic equilibrium

					for(int i=0;i<ngeno;i++) g[i]=0;
					g[ngeno/2]=y[1];									// ngeno has to be an even number
				}
				else									// Fixation mode
				{
					y[0] = N/b0;  	// S at endemic equilibrium
					y[1] = N-y[0];	// I at endemic equilibrium
					g[0] = y[1];
					g[1] = 0;

					vir[0]=virres;
					vir[1]=virmut;
					switch (tradeoff)	//// TRADEOFF SHAPE
					{
					case 0: for(int i=0; i<ngeno; i++) { beta[i]=((b0*(pow((vir[i]),coef)))/(double)N); }; break;
					case 1: for(int i=0; i<ngeno; i++) { beta[i]=(vir[i]+mu)*(b0-w0*(pow(abs(vir[i]-viropt),coef)))/(double)N; }; break;
					}
				}


				// Initialization of the propensy stuffs
				virMOY = fvir();
				betaMOY = fbeta();
				f[0] = betaMOY*y[0]*y[1];	 			// Infection
				f[1] = (mu+virMOY)*y[1];	 			// Exposed death
				if (fix_mode == 0) f[2] = mut*y[1];		// Mutation of the pathogen (mutation mode only)
				for(int i=0;i<(nevent);i++) fsum += f[i];


				// Print some outputs
				if (fix_mode == 0)						// Mutation mode
				{
					// Print very first line
					myfile.open(of_name, fstream::in | fstream::out | fstream::app);
					myfile << setw(15) << t << setw(15) << y[0] << setw(15) << y[1] << setw(15) << virMOY;
					for(int i=0;i<ngeno;i++) myfile << setw(15) << g[i];
					myfile << "\n";
					myfile.flush();
					myfile.close();
				}
				else									// Fixation mode
				{
					// Print repetition header line every 'wstep' repetitions
					if(rep % RND(wstep) == 0) { cout << setw(15) << "Repetitions SELECTION : " << rep << "\n"; flush(cout); }
				}


				//--------------------------------------------------------------------//
				//--------------------------  <Simulation>  --------------------------//
				//--------------------------------------------------------------------//

				mutant_introduced = false;
				t = 0;								// Reset time

				// Managing the change of state
				// Running the simulation until tmax
				// + In fixation mode : Introducing a mutant at tmut
				do
				{
					// Fixation mode : Introducing a mutant
					// (We mutate one resident parasite to a mutant state)
					// Done only once at t = tmut
					if (fix_mode != 0 && !mutant_introduced && t >= tmut) {
						g[0]=y[1]-1;
						g[1]=1;
						virMOY = fvir();
						betaMOY = fbeta();
						f[0] = betaMOY*y[0]*y[1];
						f[1] = (mu+virMOY)*y[1];
						mutant_introduced = true;
					}

					// Generate 2 random numbers:
					p1 = gsl_rng_uniform(r);//ARandom(); //time
					p2 = gsl_rng_uniform(r);//ARandom(); //event
					// Mutation mode OR mutant has been introduced
					if (fix_mode == 0 || mutant_introduced)
						p3 = gsl_rng_uniform(r);//ARandom(); //genotype (for infection, virulence or mutation)

					// (1) Determine time to next event and update time:
					if (p1 <= 0.0) continue; 						// p1 MUST be > 0 for log to be computable !
					if(fsum > 0) t += -log(p1)/fsum;
					else break;

					// (2) Determine the nature of next event:
					fprop = 0;
					for(int i=0;i<(nevent);i++)
					{
						fprop += f[i];
						if(p2 < (fprop/fsum)) { evt = i; break; };
					}

					// (3) Update state variables and propensy function
					switch(evt)
					{

					case 0:		// ***  <Infection>  *** //

						// Decrease by one the number of healthy ones
						y[0]--;

						// Mutation mode OR mutant has been introduced
						if (fix_mode == 0 || mutant_introduced)
						{
							// Choose the pathogen genotype
							fgeno = 0;
							for(int i=0;i<ngeno;i++)
							{
								fgeno += (beta[i]*g[i])/(betaMOY*y[1]);
								if(p3 < (fgeno)) {geno = i; break;};
							}
							g[geno]++;
						}
						// Fixation mode AND no mutant introduced yet
						else { g[0]++; }

						y[1]++;

						break;

					case 1:		// ***  <Death by infection>  *** //

						if (y[1] > 0)
						{
							// Reintroduce a new fresh healthy one to keep N constant
							y[0]++;

							// Mutation mode OR mutant has been introduced
							if (fix_mode == 0 || mutant_introduced)
							{
								// Choose the pathogen genotype
								fgeno = 0;
								for(int i=0;i<ngeno;i++)
								{
									fgeno += ((mu+vir[i])*g[i])/((mu+virMOY)*y[1]);
									if(p3 < (fgeno)) {geno = i; break;};
								}
								g[geno]--;
							}
							// Fixation mode AND no mutant introduced yet
							else { g[0]--; }

							y[1]--;
						}

						break;


					case 2:		// ***  <Mutation>  *** //		[Note : never reached in 'fixation mode' (coz' nevent = 2)]

						// Choose the pathogen genotype
						fgeno = 0;
						for(int i=0;i<ngeno;i++)
						{
							fgeno += g[i]/(double)y[1];
							if(p3 < (fgeno)) {geno = i; break;};
						}

						if (g[geno]>0)
						{
							// Choose the new mutation
							p4 = gsl_rng_uniform(r);//ARandom();

							if(p4<0.5) { if (geno>0) { g[geno-1]++;g[geno]--; } }
							else { if (geno<(ngeno-1)) { g[geno+1]++;g[geno]--; } }
						}

						break;

					}

					// (4) Update the propensy stuffs
					if (y[1] != 0)
					{
						virMOY = fvir();
						betaMOY = fbeta();
						f[0] = betaMOY*y[0]*y[1];
						f[1] = (mu+virMOY)*y[1];
						if (fix_mode == 0 && evt != 2) f[2] = mut*y[1];

						fsum = 0;
						for(int i=0;i<(nevent);i++) fsum += f[i];
					}


					// Update fixation counters
					if (fix_mode != 0)
					{
						if(mutant_introduced && g[1]<1) {indexfix++; index++; break;};
						if(mutant_introduced && g[0]<1) {indexfix++; break;};
					}

					// Writing time and state variables values to file (Mutation mode only)
					if(fix_mode == 0 && !extinct && t > tout+wstep)
					{
						cout << "time:" << setw(10) << t << setw(10) << y[0] << setw(10) << y[1] << setw(10) << virMOY << setw(10) << "\n";
						flush(cout);

						if(k>PRINTSTEP)
						{
							for(int i=0;i<PRINTSTEP;i++)
							{
								myfile.open(of_name, fstream::in | fstream::out | fstream::app);
								myfile << setw(15) << thetime[i] << setw(15) << RND(temp[i][0]) << setw(15) << RND(temp[i][1]) << setw(15) << temp[i][2];
								for(int j=0;j<ngeno;j++) { myfile << setw(15) << RND(temp[i][3+j]); }
								myfile << "\n";
								myfile.flush();
								myfile.close();
							}
							k = 1;
						}
						thetime[k-1] = t;
						for(int i=0;i<nstate;i++) temp[k-1][i] = y[i];
						temp[k-1][2] = virMOY;
						for(int j=0;j<ngeno;j++) temp[k-1][3+j] = g[j];
						k++;
						tout += wstep;
					}

					// Check extinction
					if (y[1] == 0)
					{
						if (fix_mode == 0)
						{
							s_warn = (char*)"Stopped : Extinction of the infected hosts population ! ";
							cout << s_warn << " (t=" << t << ")" << endl;
							flush(cout);
							extinct = true;
						}

						// Leave t loop
						t = tmax;
					}
					else if (y[1] == 1) 		// Nearly extinction : save the last survivor's genotype
						prev_g = (g[1] > 0) ? 1 : 0;


				} while (t < tmax);


				// Update fixation & extinction counters
				if (fix_mode != 0)
				{
					if (g[0] > 0 && g[1] > 0) indexnofix++;
					if (y[1] > 0) indexnoext++;
					else
					{
						// Update geno extinction counts
						if (prev_g == 0) indexresext++;
						else indexmutext++;
					}
				}


				// Writing the time and state variables values left in memory to disk before finishing (Mutation mode only)
				if(fix_mode == 0)
				{
					for (int i=0;i<k-1;i++)
					{
						myfile.open(of_name, fstream::in | fstream::out | fstream::app);
						myfile << setw(15) << thetime[i] << setw(15) << temp[i][0] << setw(15) << temp[i][1] << setw(15) << temp[i][2];
						for(int j=0;j<ngeno;j++) {myfile << setw(15) << temp[i][3+j];}
						myfile<< "\n";
						myfile.close();
					}
				}

				if (extinct)
				{
					myfile.open(of_name, fstream::in | fstream::out | fstream::app);
					myfile << "\n" << s_warn << " (t=" << t << ")" << "\n";
					myfile.flush();
					myfile.close();
					exit(0);
				}


			} while (rep < repmax && fix_mode != 0);


			if (fix_mode != 0) // Fixation mode only
			{
				probafix = index / (double)indexfix;

				cout << setw(10) << virres << setw(10) << indexfix << setw(10) << index << setw(10);
				if (indexfix > 0) cout << probafix;
				else cout << NAN_probafix;
				cout << setw(10) << indexnofix << setw(10) << indexnoext;
				cout << setw(10) << indexresext << setw(10) << indexmutext << "\n";
				flush(cout);

				// Writing time and state variables values to file:
				myfile.open(of_name, fstream::in | fstream::out | fstream::app);
				myfile << setw(15) << virres << setw(15) << indexfix << setw(15) << index << setw(15);
				if (indexfix > 0) myfile << probafix;
				else myfile << NAN_probafix;
				myfile << setw(15) << indexnofix << setw(15) << indexnoext;
				myfile << setw(15) << indexresext << setw(15) << indexmutext << "\n";
				myfile.flush();
				myfile.close();

				virres = virres + virstep;
			}

		} while (virres < virmax && fix_mode != 0);


		// Get end time
	    gettimeofday(&tvEnd, NULL);
	    ////timeval_print(&tvEnd);

//		// Write diff time
//	    timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
//	    printf("TOTAL SIMULATION TIME : %ld min. %ld sec.", tvDiff.tv_sec / 60, tvDiff.tv_sec % 60);
		// Write diff time
		timeval_subtract2(&tvDiff, &tvEnd, &tvBegin);
		//printf("TOTAL SIMULATION TIME : %ld min. %ld sec.", tvDiff.tv_sec / 60, tvDiff.tv_sec % 60);
		long hours = tvDiff.tv_sec / 3600;
		long minutes = (tvDiff.tv_sec % 3600) / 60;
		long seconds = tvDiff.tv_sec - (hours * 3600) - (minutes * 60); //minutes % 60;
		stringstream ss;
		ss << "TOTAL SIMULATION TIME : "	<<	hours	<<	" hrs. "	<<	minutes	<<	" min. "	<<	seconds	<<	" sec. ";
		cout << ss.str();
		flush(cout);

//	    char s_time[255] = "";
//	    sprintf(s_time, "%ld min. %ld sec.", tvDiff.tv_sec / 60, tvDiff.tv_sec % 60);
//		myfile.open(of_name, fstream::in | fstream::out | fstream::app);
//	    myfile << "TOTAL SIMULATION TIME : " << s_time << endl;
		myfile.open(of_name, fstream::in | fstream::out | fstream::app);
		myfile << ss.str();
		myfile.close();


		// Free randomizer
		gsl_rng_free(r);

	}
	catch (char *str)
	{
		cout << "Error : " << str << '\n';
	}

	return 0;
}
