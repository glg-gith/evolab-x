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
#define PV_VERSION 0.0.5

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


/**********  Common  **********/
extern int fix_mode;				// 0 : mutation | 1 : fixation
// Virulence
extern double virmin, virmax;		// alpha bounds
// Transmission
extern int tradeoff;
extern double b0;
extern double coef;
extern double w0;
extern double viropt;
// Initial population size
extern int N;
// Simulation parameters
extern double tmax;					// Time we end the simulations
extern double wstep;				// Time step in the discrete expression of time (fix_mode = 0)
									// for writing results OR the repetitions step (fix_mode = 1)

// Others
extern int ralgo;					// Chosen algorithm for random computing
extern long int rseed;				// Seed for random initialization
extern char filename[FNAMELEN];		// Output filename


/*******  Mutation mode  *******/
// Number of genetypes
extern int ngeno;
// Mutation factor
extern double mut;


/*******  Fixation mode  *******/
// Number of repetitions
extern int repmax;
extern double virstep;				// alpha step
extern double virdelta;				// alpha delta
// Time we introduce a new mutant
extern double tmut;


/*
 *========================================================================
 *							Subroutine Prototypes
 *========================================================================
 */

// Round double to closest integer
#define RND(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

// Utils
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
void timeval_print(struct timeval *tv);
char *remove_ext (char* mystr, char dot, char sep);
void remove_delimiters(char *pcBuffer, char* pcDelimiters);

// Version
void pv_header();
void pv_version();

// Help
void Usage();

// Parse command line
void parsecl(int argc, char **argv);



#endif /* PARVIR_H_ */
