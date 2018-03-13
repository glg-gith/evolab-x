/*
 * DynLibLoader.h
 *
 *  Created on: 17 juil. 2012
 *      Author: golgauth
 */

#ifndef DYNLIBLOADER_H_
#define DYNLIBLOADER_H_

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif


namespace elp {

class LabLibLoader
{
private:
	void* libhandle;
	void* dephandle;

public:
	LabLibLoader(const char* libname, const char* depname); // We allow only one so dependency right now...
	~LabLibLoader();

	void *GetFunc(const char* funcName);
};

#endif /* DYNLIBLOADER_H_ */

}
