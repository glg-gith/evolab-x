#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <iostream>

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>


static inline std::string ReplaceAll(std::string str, const std::string& from, const std::string& to)
{
    //if(from.empty()) return 0;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
    return str;
}

// Optimized: does not copy the string !!!
static inline void ReplaceAll2(std::string &str, const std::string& from, const std::string& to)
{
    //if(from.empty()) return 0;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

// Trim from start
static inline std::string &LTrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// Trim from end
static inline std::string &RTrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// Trim from both ends
static inline std::string &Trim(std::string &s) {
        return LTrim(RTrim(s));
}

#endif // STRINGUTILS_H
