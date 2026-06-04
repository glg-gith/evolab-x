/*
 * parsecl.cpp
 *
 *  Created on: 8 juin 2012
 *      Author: Gauthier Boaglio
 */


#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include <cstring>
#include <limits.h>

#include "parvir.h"



char usedopt[32] = "";

/*
 * Checking input values consistency
 */
void validatecl()
{
	char s[255];

	// Check incompatibilities with simulation mode
	if (fix_mode == 0)
	{
		if (strrchr(usedopt, 'i') != NULL) {fprintf(stderr, "The option '-i' is not compatible with 'Mutation mode' !");exit(0);}
		if (strrchr(usedopt, 'a') != NULL) {fprintf(stderr, "The option '-a' is not compatible with 'Mutation mode' !");exit(0);}
		if (strrchr(usedopt, 'A') != NULL) {fprintf(stderr, "The option '-A' is not compatible with 'Mutation mode' !");exit(0);}
		if (strrchr(usedopt, 't') != NULL) {fprintf(stderr, "The option '-t' is not compatible with 'Mutation mode' !");exit(0);}
	}
	else
	{
		if (strrchr(usedopt, 'g') != NULL) {fprintf(stderr, "The option '-g' is not compatible with 'Fixation mode' !");exit(1);}
		if (strrchr(usedopt, 'm') != NULL) {fprintf(stderr, "The option '-m' is not compatible with 'Fixation mode' !");exit(0);}
	}
	if (tradeoff == 0 && strrchr(usedopt, 'x') != NULL) {fprintf(stderr, "The option '-x' is not compatible with tradeoff=0 !");exit(1);}
	if (tradeoff == 0 && strrchr(usedopt, 'y') != NULL) {fprintf(stderr, "The option '-y' is not compatible with tradeoff=0 !");exit(1);}

	// Check values consistency
	if (virmin > virmax) 						{fprintf(stderr, "'virmin' must be < 'virmax'");exit(0);}
	if (virmin < 0.0f || virmin > 100.0f) 		{fprintf(stderr, "'virmin' must be between 0.0 and 100.0");exit(0);}
	if (virmax < 0.0f || virmax > 100.0f) 		{fprintf(stderr, "'virmax' must be between 0.0 and 100.0");exit(0);}
	if (tradeoff != 0 && tradeoff != 1)			{fprintf(stderr, "'tradeoff' must be 0 or 1");exit(0);}
	if (b0 < 0.0f || virmax > 100.0f) 			{fprintf(stderr, "'b0' must be between 0.0 and 100.0");exit(0);}
	if (coef < 0.0f || coef > 100.0f) 			{fprintf(stderr, "'coef' must be between 0.0 and 100.0");exit(0);}
	if (w0 < 0 || w0 > 100.0f) 				{fprintf(stderr, "'w0' must be between 0.01 and 100.0");exit(0);}
	if (viropt < 0.1f || viropt > 100.0f) 		{fprintf(stderr, "'viropt' must be between 0.1 and 100.0");exit(0);}
	if (N < 0 || N > 100000000) 				{fprintf(stderr, "'N' must be between 0 and 100000000");exit(0);}
	if (tmax < 0.0f || tmax > 1000000000.0f) 	{fprintf(stderr, "'tmax' must be between 0.0 and 100000000.0");exit(0);}
	if (wstep < 0.0f || wstep > 10000000.0f) 	{fprintf(stderr, "'wstep' must be between 0 and 10000000");exit(0);}
	if (ralgo < 0 || ralgo > 3) 				{fprintf(stderr, "'ralgo' must be between 0 and 3");exit(0);}
	if (rseed < -1 || rseed > LONG_MAX) 		{sprintf(s, "'rseed' must be between -1 and %ld", LONG_MAX);fprintf(stderr, s);exit(0);}
	if (ngeno < 0 || ngeno > 10000000) 			{fprintf(stderr, "'mut' must be between 0.0 and 10000000.0");exit(0);}
	if (mut < 0.0f || mut > 10000000.0f) 		{fprintf(stderr, "'mut' must be between 0.0 and 10000000.0");exit(0);}
	if (repmax < 0 || repmax > 10000000) 		{fprintf(stderr, "'repmax' must be between 0 and 10000000");exit(0);}
	if (virstep < 0.0f || virstep > 1000.0f) 	{fprintf(stderr, "'virstep' must be between 0.0 and 1000.0");exit(0);}
	if (virdelta < 0.0f || virdelta > 100.0f) 	{fprintf(stderr, "'virdelta' must be between 0.0 and 100.0");exit(0);}
	if (tmut < 0.0f || virdelta > 100.0f) 		{fprintf(stderr, "'virdelta' must be between 0.0 and 100.0");exit(0);}

}


/*
 * Parsing / Handling the input values from command line
 */
const char* const short_options = "hfv:V:d:b:c:x:y:N:T:w:r:s:o:g:m:i:a:A:t:";
static struct option long_options[] =
{
		{"help", 		0,	NULL,	'h'},
		{"fix-mode",	0,	NULL,	'f'},
		{"vir-min", 	1,	NULL,	'v'},
		{"vir-max", 	1,	NULL,	'V'},
		{"trade-off", 	1,	NULL,	'd'},
		{"b0", 			1,	NULL,	'b'},
		{"coef", 		1,	NULL,	'c'},
		{"w0", 			1,	NULL,	'x'},
		{"vir-opt", 	1,	NULL,	'y'},
		{"nb-ind", 		1,	NULL,	'N'},
		{"time-max", 	1,	NULL,	'T'},
		{"write-step", 	1,	NULL,	'w'},
		{"rand-algo", 	1,	NULL,	'r'},
		{"rand-seed", 	1,	NULL,	's'},
		{"output", 		1,	NULL,	'o'},
		{"nb-geno", 	1,	NULL,	'g'},
		{"mut-factor", 	1,	NULL,	'm'},
		{"rep-max", 	1,	NULL,	'i'},
		{"vir-step", 	1,	NULL,	'a'},
		{"vir-delta", 	1,	NULL,	'A'},
		{"time-mut", 	1,	NULL,	't'},
		{ NULL,     	0, 	NULL,  	 0 }
};

/*
 * Parsing the command line
 */
void parsecl(int argc, char **argv)
{
	/*
	 * Thus far, parvir has used getopt() to parse the CL.  However, this
	 * isn't as flexible as one might like.  To manage CLI-driven control
	 * flags and a few other things, we may need to parse out options the
	 * hard way in a future release.  The following variables are pretty much
	 * all to support getopt and would change if we change parsing tools or
	 * strategies.
	 */

	int c, errflg=0, help_flag=0;
	extern char *optarg;
	char ac[2] = "-";

	/*
	 * If parvir is executed by itself on a command line, print out help
	 * and then exit. parvir cannot be run without options, in other
	 * words.
	 */
	if(argc == 1){
		Usage();
		exit(1); /* count this as an error */
	}

	/*
	 * Let's get analyze the CL options
	 */
	while ((c = getopt_long(argc,argv, short_options, long_options, NULL)) != EOF) {

		switch (c) {

		// Common to all modes
		case 'h':
			help_flag = 1;
			break;
		case 'f':
			fix_mode = 1;
			break;
		case 'v':
			virmin = strtod(optarg,(char **) NULL);
			break;
		case 'V':
			virmax = strtod(optarg,(char **) NULL);
			break;
		case 'd':
			tradeoff = strtol(optarg,(char **) NULL,10);
			break;
		case 'b':
			b0 = strtod(optarg,(char **) NULL);
			break;
		case 'x':
			w0 = strtod(optarg,(char **) NULL);
			break;
		case 'y':
			viropt = strtod(optarg,(char **) NULL);
			break;
		case 'c':
			coef = strtod(optarg,(char **) NULL);
			break;
		case 'N':
			N = strtol(optarg,(char **) NULL,10);
			break;
		case 'T':
			tmax = strtod(optarg,(char **) NULL);
			break;
		case 'w':
			wstep = strtod(optarg,(char **) NULL);
			break;
		case 'r':
			ralgo = strtol(optarg,(char **) NULL,10);
			break;
		case 's':
			rseed = strtol(optarg,(char **) NULL,10);
			break;
		case 'o':
			strncpy(filename,optarg,FNAMELEN);
			break;

			// Only 'mutation' mode
		case 'g':
			ngeno = strtol(optarg,(char **) NULL,10);
			break;
		case 'm':
			mut = strtod(optarg,(char **) NULL);
			break;

			// Only 'fixation' mode
		case 'i':
			repmax = strtol(optarg,(char **) NULL,10);
			break;
		case 'a':
			virstep = strtod(optarg,(char **) NULL);
			break;
		case 'A':
			virdelta = strtod(optarg,(char **) NULL);
			break;
		case 't':
			tmut = strtod(optarg,(char **) NULL);
			break;

			// Unknown option
		case '?':
			errflg++;
			break;
		}

		// Keep track of the used options
		if (c != '?') { ac[0] = c; strcat(usedopt, ac); };
	}

	/*
	 * By the time we get here, all the command line options should have
	 * been processed, and we are ready to act on them.  There are a small
	 * set of options that we act on right away below, mostly associated with
	 * the autodocumenting features of the parvir CLI (only).
	 */

	/*
	 * Some option was set incorrectly.
	 */
	if(errflg){
		Usage();
		exit(0);
	}

	/*
	 * If there are no more arguments it is a legal command.  We execute
	 * some of the simpler, autodocumenting ones right here and then
	 * immediately exit.
	 */
	if(argc-optind == 0){
		/*
		 * If help was requested, call the help routine.  This routine does
		 * different things depending on the other flags and variables, so
		 * we defer calling it until they are all set.  help() will exit.
		 */
		if(help_flag) { Usage(); if (argc == 2) { exit(0); } }

		/*
		 * Some basic validity checking on the current parvir global variables
		 * right before processing to the next treatments.
		 */
		validatecl();

		/*
		 * If we get here, it is time to move on and execute parvir.
		 */
		return;
	}

	/*
	 * Leftover commands on the command line are errors and should trigger
	 * the standard Usage message and exit.
	 */

	Usage();

}






