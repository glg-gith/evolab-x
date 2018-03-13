/*
 * TestClassWrapper.h
 *
 *  Created on: 2 oct. 2012
 *      Author: golgauth
 */

#ifndef ATESTCLASS_H_
#define ATESTCLASS_H_

// Created by Cython when providing 'public api' keywords
#include "../cyelp_api.h"

#include "../../inc/TestClass.h"

namespace elps {

class ATestClass : public TestClass {
public:
	PyObject *m_obj;

	ATestClass(PyObject *obj);
	virtual ~ATestClass();
	virtual int override_me();
};

} /* namespace elps */
#endif /* ATESTCLASS_H_ */
