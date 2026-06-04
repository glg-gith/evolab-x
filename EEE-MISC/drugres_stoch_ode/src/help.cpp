/*
 * help.cpp
 *
 *  Created on: 8 juin 2012
 *      Author: Gauthier Boaglio
 */

#include <stdio.h>
#include <stdlib.h>

#include <cstring>

#include "drugres.h"


/*
 * Handling the parvir's help display
 */
void Usage()
{

	//pv_header();

fprintf(stdout, "\n\
Usage:\n\
\n\
drugres [-h]\n\
        [-t maximum sim time] [-w writing step]\n\
        [-r random algo number] [-s random seed]\n\
        [-o output filename] [-e R executable path]\n\
        [-x susceptible number] [-y infected number]\n\
        [-z treated number] [-l birth rate] [-d death rate]\n\
        [-X treatment probability] [-S superinfection propensy]\n\
        [-R recombination probability] [-m mutation rate]\n\
        [-a virulence rates] [-b transmission rates]\n\
        [-c clearance rates]\n\
        \n");

fprintf(stdout, "\n\
Base options :\n\n\
  -h, --help         - get some help.\n\
  -t, --time-max     - maximum duration of the simulation\n\
                       (default is 10000000).\n\
  -w, --write-step   - rate for writing the output results\n\
                       (default is 1000).\n\
                       This time step is expressed in the discrete\n\
                       expression of time (fraction of --time-max).\n\
  -r, --rand-algo    - chosen algorithm for random numbers generation\n\
                       (default is 0).\n\
                       Available algorithms are mt19937 [0], taus [1]\n\
                       and gfsr4 [2].\n\
  -s, --rand-seed    - the random seed for generating numbers \n\
                       (default is -1 = not fixed = auto-generated).\n\
  -o, --output       - file path where to write the simulation output \n\
                       informations.\n\
  -e, --r-path       - R executable path (Saves the resulting plot\n\
                       to a bitmap image using the output file \n\
                       naming convention).\n\
\nIndividuals :\n\n\
  -x, --nb-suscept   - number susceptible individuals at start.\n\
  -y, --nb-infect    - number infected individuals at start.\n\
  -z, --nb-treat     - number treated individuals at start.\n\
\nGeneral rates :\n\n\
  -l, --theta        - constant immigration rate of susceptible hosts.\n\
  -d, --mu           - death rate.\n\
  -X, --tho          - probability for a susceptible host to be treated.\n\
  -S, --sigma        - infected host's susceptibility to superinfection.\n\
  -R, --p-recomb     - recombination probability.\n\
  -m, --mut          - patogen genotype mutation rate.\n\
\nLoci-specific rates ([Infected,Treated] couples) :\n\n\
  -a, --vir          - description of the rates for virulence.\n\
                       Form : [a,aT] [daA,daAT] [daB,daBT] [dae,daeT].\n\
  -b, --trans        - description of the rates for transmission.\n\
                       Form : [b,bT] [dbA,dbAT] [dbB,dbBT] [dbe,dbeT].\n\
  -c, --clear        - description of the rates for clearance.\n\
                       Form : [c,cT] [dcA,dcAT] [dcB,dcBT] [dce,dceT].\n\
");

fprintf(stdout, "\n\
\n\
SCIENTIFIC NOTES :\n\n\
[1]     The evolution of resistance to drugs is a major public \n\
health concern as it erodes the efficacy of our therapeutic arsenal \n\
against bacterial, viral, and fungal pathogens. Increasingly, it is \n\
recognized that the evolution of resistance involves genetic changes \n\
at more than one locus, both in cases where multiple changes are \n\
required to obtain high-level resistance, and where compensatory \n\
changes at secondary loci ameliorate the costs of resistance. \n\
Similarly, multiple loci are often involved in the evolution of \n\
multidrug resistance. There has been widespread interest recently \n\
in understanding the evolutionary consequences of multilocus resistance,\n\
with many empirical studies documenting extensive patterns of genetic \n\
interactions (i.e., epistasis) among the loci involved.\n\
Currently, however, there are few general theoretical results \n\
available that bridge the gap between classical multilocus population\n\
genetics and mathematical epidemiology. Here, such theory is developed \n\
to shed new light on these previous studies, and to provide further \n\
guidance on the type of data required to predict the evolution of \n\
pathogens in response to drug pressure. Our results reveal the \n\
importance of feedbacks between the epidemiological and evolutionary \n\
dynamics, and illustrate how these feedbacks can be exploited to \n\
control resistance. In particular, we show how interventions such \n\
as social distancing and isolation can influence rates of \n\
recombination, and how this then can slow the spread of multilocus \n\
resistance and increase the likelihood of reversion to drug \n\
sensitivity once drug therapy has ceased. \n\
This program provides a flexible stochastic approach of such models.\n");

}




