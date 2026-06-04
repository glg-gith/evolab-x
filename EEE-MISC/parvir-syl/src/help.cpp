/*
 * help.cpp
 *
 *  Created on: 8 juin 2012
 *      Author: Gauthier Boaglio
 */

#include <stdio.h>
#include <stdlib.h>

#include <cstring>

#include "parvir.h"


/*
 * Handling the parvir's help display
 */
void Usage()
{

	//pv_header();

fprintf(stdout, "\n\
Usage:\n\
\n\
parvir [-h] [-f]\n\
       [-v minimum virulence] [-V maximum virulence]\n\
       [-d tradeoff function] [-b b0 transmission factor]\n\
       [-c coef transmission factor] [-x w0 transmission vir coef]\n\
       [-y coef transmission virulence opt] [-N ind number]\n\
       [-T maximum sim time] [-w writing step]\n\
       [-r random algo number] [-s random seed]\n\
       [-o output filename] [-g genotypes number] [-m mutation factor]\n\
       [-i repetition number] [-a virulence step] [-A virulence delta]\n\
       [-t mutant pop time]\n\
       \n");

fprintf(stdout, "\n\
Base options :\n\n\
  -h, --help         - get some help.\n\
  -f, --fix-mode     - use 'fixation' simulation mode instead of 'mutation'\n\
                       (the default).\n\
\nCommon options (to both Mutation and Fixation modes) :\n\n\
  -v, --vir-min      - minimum virulence (default is 0.0).\n\
  -V, --vir-max      - maximum virlence (default is 6.0).\n\
  -d, --trade-off    - transmission tradeoff function (default is 0).\n\
  -b, --b0           - b0 transmission factor - give a b0 transmission\n\
                       factor (default is 6.0).\n\
  -c, --coef         - transmission coefficient (default is 0.5).\n\
  -x, --w0           - transmission virulence coef (default is 0.1).\n\
                       (trade-off=1 only).\n\
  -y, --vir-opt      - transmission virulence opt (default is 3.0).\n\
                       (trade-off=1 only).\n\
  -N, --nb-ind       - number of individuals in the population \n\
                       (default is 100).\n\
  -T, --time-max     - maximum duration of the simulation\n\
                       (default is 10000000).\n\
  -w, --write-step   - rate for writing the output results\n\
                       (default is 1000).\n\
                       Mutation mode : the time step in the discrete\n\
                       expression of time (fraction of --time-max).\n\
                       OR\n\
                       Fixation mode: the repetitions writing period\n\
                       (fraction of --rep-max).\n\
  -r, --rand-algo    - chosen algorithm for random numbers generation\n\
                       (default is 0).\n\
                       Available algorithms are mt19937 [0], taus [1]\n\
                       and gfsr4 [2].\n\
  -s, --rand-seed    - the random seed for generating numbers \n\
                       (default is -1 = not fixed = auto-generated).\n\
  -o, --output       - file path where to write the simulation output \n\
                       informations.\n\
\nMutation mode options :\n\n\
  -g, --nb-geno      - give a number of intermediate possible genotypes\n\
                       (default is 100).\n\
  -m, --mut-factor   - give a number for mutation factor\n\
                       (default is 0.001).\n\
\nFixation mode options :\n\n\
  -i, --rep-max      - number of repetitions to loop through\n\
                       (default is 100000).\n\
  -a, --vir-step     - virulence increase step (default is 1).\n\
  -A, --vir-delta    - virulence delta (default is 0.1).\n\
  -t, --time-mut     - mutant pop time in the simulation process\n\
                       (default is 0.1).\n");
fprintf(stdout, "\n\
\n\
SCIENTIFIC NOTES :\n\n\
[1]     Several evolutionary processes influence virulence, the amount \n\
    of damage a parasite causes to its host. For example, parasites are \n\
    favored to exploit their hosts prudently to prolong infection and \n\
    avoid killing the host. Parasites also need to use some host \n\
    resources to reproduce and transmit infections to new hosts. Thus \n\
    parasites face a tradeoff between prudent exploitation and rapid \n\
    reproduction-a life history tradeoff between longevity and fecundity. \n\
    Other tradeoffs among components of parasite fitness also influence \n\
    virulence. For example, competition among parasite genotypes favors \n\
    rapid growth to achieve greater relative success within the host. \n\
    Rapid growth may, however, lower the total productivity of the local \n\
    group by overexploiting the host, which is a potentially renewable \n\
    food supply. This is a problem of kin selection and group selection. \n\
    And this is the purpose of this program.\n");

}




