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
#include <sstream>
#include <limits.h>

#include "drugres.h"

using namespace std;


char usedopt[32] = "";

/*
 * Checking input values consistency
 */
void validatecl()
{
	char s[255];

	if (tmax < 0.0f || tmax > 100000000.0f) 	{fprintf(stderr, "'tmax' must be between 0 and 100000000");exit(0);}
	if (wstep < 0.0f || wstep > 10000000.0f) 	{fprintf(stderr, "'wstep' must be between 0 and 10000000");exit(0);}
	if (ralgo < 0 || ralgo > 3) 				{fprintf(stderr, "'ralgo' must be between 0 and 3");exit(0);}
	if (rseed < -1 || rseed > LONG_MAX) 		{sprintf(s, "'rseed' must be between -1 and %ld", LONG_MAX);fprintf(stderr, s);exit(0);}

	if (S < 0 || S > 10000000) 					{fprintf(stderr, "'S' must be between 0 and 10000000");exit(0);}
	if (I < 0 || I > 10000000) 					{fprintf(stderr, "'I' must be between 0 and 10000000");exit(0);}
	if (T < 0 || T > 10000000) 					{fprintf(stderr, "'T' must be between 0 and 10000000");exit(0);}

	if (theta < 0.0f || theta > 100.0f) 		{fprintf(stderr, "'mut' must be between 0 and 100");exit(0);}
	if (mu < 0.0f || mu > 1.0f) 				{fprintf(stderr, "'mu' must be between 0 and 1");exit(0);}
	if (tho < 0.0f || tho > 1.0f) 				{fprintf(stderr, "'tho' must be between 0 and 1");exit(0);}
	if (sigma < 0.0f || sigma > 10.0f) 			{fprintf(stderr, "'sigma' must be between 0 and 10");exit(0);}
	if (mut < 0.0f || mut > 10000000.0f) 		{fprintf(stderr, "'mut' must be between 0 and 10000000");exit(0);}

}


/*
 * Parsing / Handling the input values from command line
 */
const char* const short_options = "ht:w:r:s:o:e:x:y:z:l:d:X:S:R:m:a:b:c:";
static struct option long_options[] =
{
		// Common
		{"help", 		0,	NULL,	'h'},
		{"time-max", 	1,	NULL,	't'},
		{"write-step", 	1,	NULL,	'w'},
		{"rand-algo", 	1,	NULL,	'r'},
		{"rand-seed", 	1,	NULL,	's'},
		{"output", 		1,	NULL,	'o'},
		{"r-path", 		1,	NULL,	'e'},

		// Individuals
		{"nb-suscept", 	1,	NULL,	'x'},
		{"nb-infect", 	1,	NULL,	'y'},
		{"nb-treat", 	1,	NULL,	'z'},

		// General rates
		{"theta", 		1,	NULL,	'l'},
		{"mu", 			1,	NULL,	'd'},
		{"tho", 		1,	NULL,	'X'},
		{"sigma", 		1,	NULL,	'S'},
		{"p-recomb", 	1,	NULL,	'R'},
		{"mut",		 	1,	NULL,	'm'},

		// Loci-specific rates
		{"vir",			1,	NULL,	'a'},
		{"trans", 		1,	NULL,	'b'},
		{"clear", 		1,	NULL,	'c'},

		{ NULL,     	0, 	NULL,  	 0 }
};

/*
 * Parse matrix line (form : [[a, aT][daA, daAT][daB, daBT][dae, daeT]])
 * a:line 1, b:line 2, c:line 3
 */
void parse_mtx_line(char *optarg, int line_num)
{
	char *parsed;

	parsed = str_replace(optarg, (char*)"[", (char*)" ");
	parsed = str_replace(parsed, (char*)"]", (char*)" ");
	parsed = str_replace(parsed, (char*)",", (char*)" ");

	stringstream ss(parsed);
	ss >> VTCmtx[line_num][0][0] >> VTCmtx[line_num][0][1] >> VTCmtx[line_num][1][0] >> VTCmtx[line_num][1][1]
	   >> VTCmtx[line_num][2][0] >> VTCmtx[line_num][2][1] >> VTCmtx[line_num][3][0] >> VTCmtx[line_num][3][1];

	//fprintf(stdout, ">>>>>>>>>>>>>>>>>  %s", parsed);
}

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

		// Common
		case 'h':
			help_flag = 1;
			break;
		case 't':
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
			strcpy(filename,optarg);
			remove_delimiters(filename, (char*)"\"");
			break;
		case 'e':
			strcpy(rpath,optarg);
			remove_delimiters(rpath, (char*)"\"");
			break;

			// Individuals
		case 'x':
			S = strtol(optarg,(char **) NULL,10);
			break;
		case 'y':
			I = strtol(optarg,(char **) NULL,10);
			break;
		case 'z':
			T = strtol(optarg,(char **) NULL,10);
			break;

			// General rates
		case 'l':
			theta = strtod(optarg,(char **) NULL);
			break;
		case 'd':
			mu = strtod(optarg,(char **) NULL);
			break;
		case 'X':
			tho = strtod(optarg,(char **) NULL);
			break;
		case 'S':
			sigma = strtod(optarg,(char **) NULL);
			break;
		case 'R':
			rp = strtod(optarg,(char **) NULL);
			break;
		case 'm':
			mut = strtod(optarg,(char **) NULL);
			break;

			// Loci-specific rates
		case 'a':
			parse_mtx_line(optarg, 0);
			break;
		case 'b':
			parse_mtx_line(optarg, 1);
			break;
		case 'c':
			parse_mtx_line(optarg, 2);
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






