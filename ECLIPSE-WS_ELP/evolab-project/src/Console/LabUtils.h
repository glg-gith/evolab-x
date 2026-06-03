/*
 * Utils.h
 *
 *  Created on: 17 juil. 2012
 *      Author: golgauth
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include <iostream>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sstream>
#include <vector>

#include <fstream>


using namespace std;

namespace elp {

class LabUtils {
public:
	LabUtils();
	virtual ~LabUtils();

	static int ListDir(string dir, vector<string> &files, bool skip_dot = false, string filter = "");
	static string FileToString(string filename);
	static void StringToFile(string filename, string content);
	static int TimevalSubtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
	static void TimevalPrint(struct timeval *tv);
	static string StringReplace( string src, string const& target, string const& repl);
	static void Tokenize(const std::string& str, std::vector<std::string>& tokens,
            const std::string& delimiters, const bool trimEmpty);
	static string RemoveChars(std::basic_string<char>& Str, const char *CharsToRemove);
	static string ColorString(string str, int color, bool bold = false);
};

} /* namespace elp */
#endif /* UTILS_H_ */
