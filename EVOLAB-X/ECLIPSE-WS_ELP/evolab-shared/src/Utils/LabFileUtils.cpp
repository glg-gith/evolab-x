/*
 * LabFileUtils.cpp
 *
 *  Created on: 21 sept. 2012
 *      Author: golgauth
 */

#include "LabFileUtils.h"

namespace elps {

LabFileUtils::LabFileUtils() {
	// TODO Auto-generated constructor stub

}

LabFileUtils::~LabFileUtils() {
	// TODO Auto-generated destructor stub
}

bool compare_filenames(string a, string b)
{
	stringstream ss;
	istringstream iss, iss2;
	string s_id;
	int a_id, b_id;
	int dot_pos;

	dot_pos = a.find_last_of('.');
	s_id = a.substr(dot_pos + 1, a.length() - dot_pos);
	iss.str(s_id); iss >> a_id;

	dot_pos = b.find_last_of('.');
	s_id = b.substr(dot_pos + 1, b.length() - dot_pos);
	/*iss.str("");*/ iss2.str(s_id); iss2 >> b_id;

	//cout << "Prec : " << a_id << " || Next : " << b_id << endl;
	return (a_id < b_id);
}

int LabFileUtils::ListDir (string dir, vector<string> &files, bool skip_dot, string filter, bool filter_ext_only)
{
//	try {
		string a_name;
		DIR *dp;
		struct dirent *dirp;
		if((dp  = opendir(dir.c_str())) == NULL) {
			cerr << "Error(" << errno << ") opening " << dir << endl;
			return errno;
		}

		while ((dirp = readdir(dp)) != NULL) {
			a_name = string(dirp->d_name);
			if ((filter == "" || (!filter_ext_only && a_name.find(filter) != string::npos)
					|| (filter_ext_only && (a_name.find(filter) == a_name.length() - filter.length())))
					&& (!skip_dot || (a_name != "." && a_name != ".." )))
				files.push_back(a_name);
		}
		closedir(dp);
		std::sort(files.begin(), files.end(), compare_filenames);
		for (vector<string>::iterator it=files.begin(); it<files.end(); it++)
			cout << (*it) << endl;
//	} catch ( char * str ) {
//		cout << "'ListDir' exception raised : " << str << endl;
//	}
	return 0;
}

string LabFileUtils::FileToString(string filename)
{
	string content = "";
	ifstream in_mk;
	in_mk.open(filename.c_str(), ifstream::in);
	string temp;
	while (in_mk.good() && !in_mk.eof()) {
		getline(in_mk, temp);
		content += temp + "\n";
	}
	in_mk.close();
	return content;
}

void LabFileUtils::StringToFile(string filename, string content)
{
	ofstream out_mk;
	out_mk.open(filename.c_str(), ofstream::out);
	out_mk << content << std::flush;
	out_mk.close();
}

} /* namespace elps */
