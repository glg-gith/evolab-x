/*
 * DynLibLoader.cpp
 *
 *  Created on: 17 juil. 2012
 *      Author: golgauth
 */

#include "LabLibLoader.h"

#include <string.h>

#include <iostream>

using namespace std;

namespace elp {

LabLibLoader::LabLibLoader(const char* libname, const char* depname) {

	dephandle = 0;
	libhandle = 0;

#ifdef _WIN32
	dephandle = LoadLibrary(depname);
	libhandle = LoadLibrary(libname);
#else
	// Open plugin dependency shared object
	dephandle = dlopen(depname, RTLD_NOW | RTLD_GLOBAL); //"./Debug/Plugins/lib/libelp.so"
	// Open plugin shared object
	libhandle = dlopen(libname, RTLD_NOW | RTLD_GLOBAL); // RTLD_LAZY | RTLD_GLOBAL); //RTLD_NOW
#endif

	// Handle so loading errors
	string s_e = "Library could not be loaded : ";
	if (!dephandle)
	{
		s_e += (string)depname;
		std::cerr << s_e << endl; throw(s_e);
	}
	if (!libhandle)
	{
		s_e += (string)libname;
		std::cerr << s_e << endl; throw(s_e);
	}
}
;

LabLibLoader::~LabLibLoader() {

	if (libhandle) {
#ifdef _WIN32
		FreeLibrary((HINSTANCE)libhandle);
#else
		dlclose (libhandle);
#endif
	}

	if (dephandle) {
#ifdef _WIN32
		FreeLibrary((HINSTANCE)dephandle);
#else
		dlclose (dephandle);
#endif
	}

}
;

void *LabLibLoader::GetFunc(const char* funcName)
{
	void* func = 0;
	if (libhandle) {
#ifdef _WIN32
		func = GetProcAddress((HINSTANCE)libhandle, funcName);
#else
		func = dlsym(libhandle, funcName);
#endif
	}

	if (!func)
		throw("Function could not be loaded.");

	return func;
};

}
