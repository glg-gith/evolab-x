/*
 * LabCythonUtils.h
 *
 *  Created on: Jul 25, 2013
 *      Author: golgauth
 */

#ifndef LABCYTHONUTILS_H_
#define LABCYTHONUTILS_H_

//// Cython !!!
#ifndef NOPYTHON

#include <LabConsts.h>
#include <Python.h>

#include <string>
using namespace std;

class LIB_CLASS LabCythonUtils {

public:

	static void CyCallFuncVoid(PyObject *pyobj, string method, bool& error) {

		if (PyObject_HasAttrString(pyobj, method.c_str())) {
			error = false;
			/*PyObject *ret =*/ PyObject_CallMethod(pyobj, (char*)method.c_str(), NULL);
		}
		else {
			error = true;
		}
	}
	static bool CyCallFuncBool(PyObject *pyobj, string method, bool& error) {

		if (PyObject_HasAttrString(pyobj, method.c_str())) {
			error = false;
			PyObject *ret = PyObject_CallMethod(pyobj, (char*)method.c_str(), NULL);
			return (ret == Py_True);
		}
		else {
			error = true;
			return false;
		}
	}
	static int CyCallFuncInt(PyObject *pyobj, string method, bool& error) {

		if (PyObject_HasAttrString(pyobj, method.c_str())) {
			error = false;
			PyObject *ret = PyObject_CallMethod(pyobj, (char*)method.c_str(), NULL);
			return (int)PyInt_AsLong(ret);
		}
		else {
			error = true;
			return 0;
		}
	}
	static double CyCallFuncDouble(PyObject *pyobj, string method, bool& error) {

		if (PyObject_HasAttrString(pyobj, method.c_str())) {
			error = false;
			PyObject *ret = PyObject_CallMethod(pyobj, (char*)method.c_str(), NULL);
			return PyFloat_AsDouble(ret);
		}
		else {
			error = true;
			return 0.0;
		}
	}


};

#endif

#endif /* LABCYTHONUTILS_H_ */
