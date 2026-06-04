/*
 * utils.cpp
 *
 *  Created on: 8 juin 2012
 *      Author: Gauthier Boaglio
 */

#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <sys/time.h>
#include <sys/types.h>

#include <cstring>



//-------------------  Handle time in milliseconds  ----------------------//

/*
 * Return 1 if the difference is negative, otherwise 0.
 */
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
	long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
	result->tv_sec = diff / 1000000;
	result->tv_usec = diff % 1000000;

	return (diff<0);
}

void timeval_print(struct timeval *tv)
{
	char buffer[30];
	time_t curtime;

	printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
	curtime = tv->tv_sec;
	strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
	printf(" = %s.%06ld\n", buffer, tv->tv_usec);
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


//--------------------------  String utils  ------------------------------//

/*
 * Remove the "extension" from a file spec.
 *    srcstr is the string to process.
 *    dot is the extension separator.
 *    sep is the path separator (0 means to ignore).
 * Returns an allocated string identical to the original but
 *    with the extension removed. It must be freed when you're
 *    finished with it.
 * If you pass in NULL or the new string can't be allocated,
 *    it returns NULL.
 */
char *remove_ext(char* srcstr, char dot, char sep) {
	char *retstr, *lastdot, *lastsep;

	// Error checks and allocate string.
	if (srcstr == NULL)
		return NULL;
	if ((retstr = (char*)malloc (strlen (srcstr) + 1)) == NULL)
		return NULL;

	// Make a copy and find the relevant characters.
	strcpy (retstr, srcstr);
	lastdot = strrchr (retstr, dot);
	lastsep = (sep == 0) ? NULL : strrchr (retstr, sep);

	// If it has an extension separator.
	if (lastdot != NULL) {
		// and it's before the extension separator.
		if (lastsep != NULL) {
			if (lastsep < lastdot) {
				// then remove it.
				*lastdot = '\0';
			}
		} else {
			// Has extension separator with no path separator.
			*lastdot = '\0';
		}
	}
	return retstr;
}

/*
 * Remove all chars in the delimiters list from target string
 */
void remove_delimiters(char *target, char* delimiters)
{
	int i = 0;
	int iLen = strlen(target);
	for(i = 0 ; i < iLen; i++)
	{
		if( strchr(delimiters, target[i]) != NULL )
		{
			memcpy(&target[i], &target[i + 1], iLen - i);
			iLen--;
			i--;
		}
	}
	return;
}

