/*
 * parvir.h
 *
 *  Created on: 8 juin 2012
 *      Author: Gauthier Boaglio
 */

#ifndef PARVIR_H_
#define PARVIR_H_

/*
 * parvir version used all over the program
 */
#define PV_VERSION 0.1.6

/*
 * This is how one gets a macro into quotes; an important one to keep
 * in all program templates.
 */
#define _QUOTEME(x) #x
#define QUOTEME(x) _QUOTEME(x)


/*
 *========================================================================
 *                           Global Variables
 *
 * The primary control variables, //in alphabetical order, with comments.
 *========================================================================
 */

const long int FNAMELEN = 4096;


/********  Common  *********/
extern double tmax;                 // Time we end the simulations
extern double wstep;                // Time step in the discrete expression of time (fix_mode = 0)
									// for writing results OR the repetitions step (fix_mode = 1)
extern int ralgo;                   // Chosen algorithm for random computing
extern long int rseed;              // Seed for random initialization
extern char filename[FNAMELEN];     // Output filename
extern char rpath[FNAMELEN];		// R executable full path

/******  Individuals  ******/
extern int S;			// TODO : Replace it by endemic equilibrium law
extern int I;			//
extern int T;			//

/*****  General rates  *****/
extern double theta; 				// Global birth rate
extern double mu;					// Natural per capita death rate
extern double tho;					// Probability to be treated
extern double sigma;				// Superinfection susceptibility
extern double rp; 					// Recombination probability
extern double mut;					// Mutation rate

/**  Loci-specific rates  **/
// State matrix
/*
 * 							  Default     A effect     B effect     Epistasis
 * 						 	 ___________________________________________________
 * 	Virulence 		[I,T]	| [a, aT]     [daA, daAT]  [daB, daBT]  [dae, daeT]
 * 	Transmission	[I,T]	| [b, bT]     [dbA, dbAT]  [dbB, dbBT]  [dbe, dbeT]
 * 	Clearance		[I,T]	| [c, cT]     [dcA, dcAT]  [dcB, dcBT]  [dce, dceT]
 *
 */
extern double VTCmtx[3][4][2];



/*
 *========================================================================
 *							Subroutine Prototypes
 *========================================================================
 */

// Round double to closest integer
#define RND(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

// Utils
char *remove_ext (char* mystr, char dot, char sep);
void remove_delimiters(char *pcBuffer, char* pcDelimiters);
char *str_replace(const char *string, const char *substr, const char *replacement);
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
void timeval_print(struct timeval *tv);

// R
////void to_r_script(char *out_rs_name, char *in_txt_name, char *out_img_name);
void to_r_script(char *in_txt_name_stoch, char *in_txt_name_ode, char *out_rs_name, char *out_ri_name);

// Version
void pv_header();
void pv_version();

// Help
void Usage();

// Parse command line
void parsecl(int argc, char **argv);



#endif /* PARVIR_H_ */
