/*
 * LabTimeUtils.h
 *
 *  Created on: 14 sept. 2012
 *      Author: golgauth
 */

#ifndef LABTIMEUTILS_H_
#define LABTIMEUTILS_H_


#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <sys/time.h>
#include <sys/types.h>

#include <string>


#include <LabConsts.h>

using namespace std;


namespace elps {

/**
 * Facility class for dealing with time (benchmarking).
 */
class LIB_CLASS LabTimeUtils {
public:
	LabTimeUtils();
	virtual ~LabTimeUtils();

	/**
	 * Subtract two given times (works on C timeval types)
	 * @param result Storage of the result of the subtraction
	 * @param t2 Time to be subtracted from
	 * @param t1 Time to subtract from t2
	 * @return 1 if the difference is negative, otherwise 0
	 */
	static int TimevalSubtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
	/**
	 * Print a time into a string
	 * @param sout The resulting string
	 * @param tv the time to print
	 */
	static void TimevalPrint(char *sout, struct timeval *tv);

};

/**
 * Utility class for dealing with time (benchmarking).
 */
class LIB_CLASS LabTimer {
public:
	LabTimer();
	virtual ~LabTimer();

	/**
	 * Start timer. Reset if already started.
	 */
	void Start();
	/**
	 * Stop timer.
	 */
	void Stop();
	/**
	 * Print the elapsed time between Start() and Stop() to the standard output.
	 */
	void Print();

private:

	/** Times to work on - used to compute the elapsed time. */
	struct timeval tvBegin, tvEnd, tvDiff;

};


}  // namespace elps

#endif /* LABTIMEUTILS_H_ */
