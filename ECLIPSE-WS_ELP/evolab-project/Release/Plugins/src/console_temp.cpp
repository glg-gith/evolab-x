/*
 * plugin-template.cpp
 *
 *  Created on: 17 juil. 2012
 *      Author: golgauth
 */


#include <iostream>
#include <stdlib.h>

//#include <blitz/blitz.h>          // Basic Blitz++ includes
//#include <blitz/array.h>          // Dummy array object (_dummyArray)
//#include <blitz/indexexpr.h>      // Tensor index objects

#include <Utility.h>
#include <Network.h>

#include <string>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>


#include "plugins_utils.h"


//using namespace blitz;
using namespace std;



class Class_console_temp {
public:
void Call() {
	
	//~ Network net("name", Network::Undirected);
    //~ net.populate(10000);
	//~ net.dumper();
	//~ ...
	
/******************************************************/

cout<<("test !")<<endl;
/******************************************************/

};
} inst_console_temp;



extern "C" void console_temp(bool show_time) {

struct timeval tvBegin, tvEnd, tvDiff;

if (show_time)
{
	// Get begin time
	gettimeofday(&tvBegin, NULL);
	timeval_print(&tvBegin);
}


/******************************************************/
inst_console_temp.Call();
/******************************************************/

	
if (show_time)
{
	// Get end time
	gettimeofday(&tvEnd, NULL);
	timeval_print(&tvEnd);

	// Write diff time
	timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
	std::cout << "TOTAL SIMULATION TIME : " << std::endl;
	std::cout << (tvDiff.tv_sec / 60) << " min. " << (tvDiff.tv_sec % 60) << " sec. " << (tvDiff.tv_usec) << " usec." << std::endl << std::flush;
}

}


