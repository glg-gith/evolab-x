/*
 * ALabCallBack.cpp
 *
 *  Created on: 9 oct. 2012
 *      Author: golgauth
 */

#include "ALabCallBack.h"

namespace elps {

ALabCallBack::ALabCallBack()
{
	is_cy_call = true;
};

ALabCallBack::ALabCallBack(PyObject *obj)
{
	//method         = _method;
	this->m_obj = obj;
	// Provided by "cyelp_api.h"
	if (import_cyelp()) {
	} else {
		Py_XINCREF(this->m_obj);
	}
};

} /* namespace elps */
