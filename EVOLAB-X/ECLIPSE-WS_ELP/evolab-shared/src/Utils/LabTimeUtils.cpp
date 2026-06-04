/*
 * LabTimeUtils.cpp
 *
 *  Created on: 14 sept. 2012
 *      Author: golgauth
 */

#include "LabTimeUtils.h"

using namespace std;


namespace elps {


LabTimeUtils::LabTimeUtils() {
	// TODO Auto-generated constructor stub

}

LabTimeUtils::~LabTimeUtils() {
	// TODO Auto-generated destructor stub
}


//-------------------  Handle time in milliseconds  ----------------------//

/*
 * Return 1 if the difference is negative, otherwise 0.
 */
int LabTimeUtils::TimevalSubtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
	long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
	result->tv_sec = diff / 1000000;
	result->tv_usec = diff % 1000000;

	return (diff<0);
}

void LabTimeUtils::TimevalPrint(char *sout, struct timeval *tv)
{
	char buffer[30];
	time_t curtime;

	sprintf(sout, "%ld.%06ld", tv->tv_sec, tv->tv_usec);
	curtime = tv->tv_sec;
	strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
	sprintf(sout, "%s = %s.%06ld\n", sout, buffer, tv->tv_usec);
}

// usage :
/*

    struct timeval tvBegin, tvEnd, tvDiff;

    // begin
    gettimeofday(&tvBegin, NULL);
    timeval_print(&tvBegin);

    // lengthy operation
    int i,j;
    for(i=0;i<999999L;++i) {
        j=sqrt(i);
    }

    //end
    gettimeofday(&tvEnd, NULL);
    timeval_print(&tvEnd);

    // diff
    timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
    printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);


 */


LabTimer::LabTimer() {
}
LabTimer::~LabTimer() {
}

void LabTimer::Start()
{
	// Get begin time
	//char *str = (char *) malloc (80 * sizeof(char));
	//str[0] = '\0';
	gettimeofday(&tvBegin, NULL);
	//LabTimeUtils::TimevalPrint(str, &tvBegin);
	//cout << str;
}
void LabTimer::Stop()
{
	gettimeofday(&tvEnd, NULL);
	//char *str = (char *) malloc (80 * sizeof(char));
	//str[0] = '\0';
	//LabTimeUtils::TimevalPrint(str, &tvEnd);
	//cout << str;
	LabTimeUtils::TimevalSubtract(&tvDiff, &tvEnd, &tvBegin);
}
void LabTimer::Print()
{
	cout << "TIMER TOOK : ";
	cout << (tvDiff.tv_sec / 60) << " min. " << (tvDiff.tv_sec % 60) << " sec. " << (tvDiff.tv_usec) << " usec." << endl;
}


}  // namespace elps
