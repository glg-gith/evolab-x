
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>

#include <stdlib.h>
//#include <blitz/blitz.h>
//#include <blitz/array.h>

//using namespace blitz;
using namespace std;

#ifndef _TIME_FEATURES

	#define _TIME_FEATURES

	int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
	void timeval_print(struct timeval *tv);

#endif

