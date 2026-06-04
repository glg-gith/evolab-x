/*
 * version.cpp
 *
 *  Created on: 8 juin 2012
 *      Author: Gauthier Boaglio
 */


#include <stdio.h>
#include <stdlib.h>

#include <cstring>

#include "parvir.h"



#define LINE_LENGTH 80


/*
 * Handling the parvir's version display
 */

void pv_header()
{

	int i,half,version_length;

	version_length = strlen(QUOTEME(PV_VERSION));

	fprintf(stdout,"#=========================================================================#\n");
	fprintf(stdout,"#");
	/* Pad the front */
	half = (LINE_LENGTH - 48 - version_length - 2)/2;
	for(i=0;i<half;i++){
		fprintf(stdout," ");
	}
	fprintf(stdout,"parvir version %s Copyright 2012 Sylvain GANDON",QUOTEME(PV_VERSION));
	/* Pad the rear */
	half = LINE_LENGTH - 52 - version_length - half;
	for(i=0;i<half;i++){
		fprintf(stdout," ");
	}
	fprintf(stdout,"#\n");
	fprintf(stdout,"#=========================================================================#\n");

}

void pv_version()
{
	fprintf(stdout,"%s\n",QUOTEME(PV_VERSION));
}



