/*
 * main.h
 *
 *  Created on: Oct 4, 2013
 *      Author: golgauth
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <string.h>

#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>



const long int FNAMELEN = 4096;
extern char filename[FNAMELEN];     // Output filename
extern char rpath[FNAMELEN];		// R executable full path


// Utils
char *remove_ext (char* mystr, char dot, char sep);
void remove_delimiters(char *pcBuffer, char* pcDelimiters);
char *str_replace(const char *string, const char *substr, const char *replacement);
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
void timeval_print(struct timeval *tv);

// R
void to_r_script(char *out_rs_name, char *in_txt_name, char *out_img_name);


#endif /* MAIN_H_ */
