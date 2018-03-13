/*
 * Utils.cpp
 *
 *  Created on: 17 juil. 2012
 *      Author: golgauth
 */

#include "LabUtils.h"


namespace elp {

LabUtils::LabUtils() {
	// TODO Auto-generated constructor stub

}

LabUtils::~LabUtils() {
	// TODO Auto-generated destructor stub
}


//-------------------  Handle files  ----------------------//

int LabUtils::ListDir (string dir, vector<string> &files, bool skip_dot, string filter)
{
	string a_name;
	DIR *dp;
	struct dirent *dirp;
	if((dp  = opendir(dir.c_str())) == NULL) {
		cerr << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		a_name = string(dirp->d_name);
		if ((filter == "" || a_name.find(filter) == a_name.length() - filter.length())
				&& (!skip_dot || (a_name != "." && a_name != ".." )))
			files.push_back(a_name);
	}
	closedir(dp);
	return 0;
}

string LabUtils::FileToString(string filename)
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

void LabUtils::StringToFile(string filename, string content)
{
	ofstream out_mk;
	out_mk.open(filename.c_str(), ofstream::out);
	out_mk << content << std::flush;
	out_mk.close();
}


//-------------------  Handle time  ----------------------//

/*
 * Return 1 if the difference is negative, otherwise 0.
 */
int LabUtils::TimevalSubtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
	long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
	result->tv_sec = diff / 1000000;
	result->tv_usec = diff % 1000000;

	return (diff<0);
}

/*
 * Print time as day + min:sec.usec format
 */
void LabUtils::TimevalPrint(struct timeval *tv)
{
	char buffer[30];
	time_t curtime;

	printf("%ld:%ld.%06ld", tv->tv_sec / 60, tv->tv_sec % 60, tv->tv_usec);
	curtime = tv->tv_sec;
	strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
	printf(" = %s.%06ld\n", buffer, tv->tv_usec);
}

// usage :
/*

    struct timeval tvBegin, tvEnd, tvDiff;

    // begin
    gettimeofday(&tvBegin, NULL);
    TimevalPrint(&tvBegin);

    // lengthy operation
    int i,j;
    for(i=0;i<999999L;++i) {
        j=sqrt(i);
    }

    //end
    gettimeofday(&tvEnd, NULL);
    TimevalPrint(&tvEnd);

    // diff
    TimevalSubtract(&tvDiff, &tvEnd, &tvBegin);
    printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);


 */

//-------------------  Handle Strings  ----------------------//


//template < class ContainerT >
void LabUtils::Tokenize(const std::string& str, std::vector<std::string>& tokens,
		const std::string& delimiters, const bool trimEmpty)
{
	std::string::size_type pos, lastPos = 0;
	while(true)
	{
		pos = str.find_first_of(delimiters, lastPos);
		if(pos == std::string::npos)
		{
			pos = str.length();

			if(pos != lastPos || !trimEmpty)
				tokens.push_back(std::vector<std::string>::value_type(str.data()+lastPos, pos-lastPos ));

			break;
		}
		else
		{
			if(pos != lastPos || !trimEmpty)
				tokens.push_back(std::vector<std::string>::value_type(str.data()+lastPos, pos-lastPos ));
		}

		lastPos = pos + 1;
	}
};


std::string LabUtils::RemoveChars(std::basic_string<char>& Str, const char *CharsToRemove)
{
	std::basic_string<char>::size_type pos = 0;
	while (( pos = Str.find_first_of( CharsToRemove, pos )) != std::basic_string<char>::npos )
	{
		Str.erase( pos, 1 );
	}
	return Str;
}

string LabUtils::StringReplace( string src, string const& target, string const& repl)
{
	// handle error situations/trivial cases

	if (target.length() == 0) {
		// searching for a match to the empty string will result in
		//  an infinite loop
		//  it might make sense to throw an exception for this case
		return src;
	}

	if (src.length() == 0) {
		return src;  // nothing to match against
	}

	size_t idx = 0;

	for (;;) {
		idx = src.find( target, idx);
		if (idx == string::npos)  break;

		src.replace( idx, target.length(), repl);
		idx += repl.length();
	}

	return src;
}

string LabUtils::ColorString(string str, int color, bool bold)
{
#if defined(_WIN32) || defined(_RUN_UNDER_ECLIPSE)
	return str;
#else
	stringstream ss;
	ss << color;
	if (bold)
		return "\033[1;" + ss.str() + "m" + str + "\033[0m";
	else
		return "\033[0;" + ss.str() + "m" + str + "\033[0m";
#endif
}

} /* namespace elp */
