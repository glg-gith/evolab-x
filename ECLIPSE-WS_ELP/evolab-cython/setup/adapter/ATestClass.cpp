/*
 * TestClassWrapper.cpp
 *
 *  Created on: 2 oct. 2012
 *      Author: golgauth
 */

#include "ATestClass.h"

namespace elps {

ATestClass::ATestClass(PyObject *obj): m_obj(obj) {
	// Provided by "cyelp_api.h"
	if (import_cyelp()) {
	} else {
		Py_XINCREF(this->m_obj);
	}
}

ATestClass::~ATestClass() {
	Py_XDECREF(this->m_obj);
}

int ATestClass::override_me()
{
	if (this->m_obj) {
		int error;
		// Call a virtual overload, if it exists
		int result = cy_call_func_int(this->m_obj, (char*)"override_me", &error);
		if (error)
			// Call parent method
			result = TestClass::override_me();
		return result;
	}
	// Throw error ?
	return 0;
}

} /* namespace elps */
