/*
 * LabFileUtils.h
 *
 *  Created on: 21 sept. 2012
 *      Author: golgauth
 */

#ifndef LABFILEUTILS_H_
#define LABFILEUTILS_H_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <dirent.h>
#include <algorithm>
#include <errno.h>

#include <sstream>
#include <cstring>


#include <LabConsts.h>

using namespace std;

namespace elps {

/**
 * Facility for dealing with files (and directories)
 */
class LIB_CLASS LabFileUtils {
public:
	LabFileUtils();
	virtual ~LabFileUtils();

	/**
	 * List the files found in a given directory.
	 * @param dir The full path of the directory to search
	 * @param files The resulting list of files
	 * @param skip_dot Skip the current and the backward directories (aka : '.' and '..')
	 * @param filter String that the searched files must contain to appear in the resulting list
	 * @param filter_ext_only Apply the "filter" only on the extension of the files
	 * @return
	 */
	static int ListDir(string dir, vector<string> &files, bool skip_dot = false,
											string filter = "", bool filter_ext_only = false);
	/**
	 * Read the content of the given file and return it into a string.
	 * @param filename The file to be converted
	 * @return The content of the file
	 */
	static string FileToString(string filename);
	/**
	 * Write a string to a given file
	 * @param filename The file to write to
	 * @param content The string to write
	 */
	static void StringToFile(string filename, string content);

};

} /* namespace elps */
#endif /* LABFILEUTILS_H_ */
