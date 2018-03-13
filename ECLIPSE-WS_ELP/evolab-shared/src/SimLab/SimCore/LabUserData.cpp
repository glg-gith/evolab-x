/*
 * LabUserData.cpp
 *
 *  Created on: 10 oct. 2012
 *      Author: golgauth
 */

#include "LabUserData.h"

namespace elps {

LabUserData::LabUserData() {

#ifndef NOPYTHON
	this->m_obj = NULL;
	this->m_data = NULL;
#endif

}


#ifndef NOPYTHON
LabUserData::LabUserData(PyObject *obj): LabUserData() {
	cout << "Create LabUserData" << obj << endl;
	this->CySetPyObj(obj);
}

LabUserData::LabUserData(PyObject *obj, PyObject *data): LabUserData() {
	cout << "Create LabUserData" << obj << endl;
	this->CySetPyObj(obj);
	this->CySetPyData(data);
}
#endif



LabUserData::~LabUserData() {
	cout << "Destroy LabUserData" << endl;
#ifndef NOPYTHON
	//if (this->m_obj) Py_XDECREF(this->m_obj);
	if (this->m_data) Py_XDECREF(this->m_data);
#endif

}

int LabUserData::GetId() {
	return this->id;
}
void LabUserData::SetId(unsigned int id) {
	this->id = id;
}


//// Cython !!!
#ifndef NOPYTHON

PyObject * LabUserData::CyGetPyObj() {
	return this->m_obj;
}

void LabUserData::CySetPyObj(PyObject *obj) {

	if (obj == NULL) { this->m_obj = NULL; return; }

	/**
	 * WARNING : An object should never INCREF itself (because, then Python's
	 * garbage collector can never free the object and call the corresponding C destructor).
	 * What we do here is VERY BAD.
	 * In the future, handle reference counting using lists...
	 * at the Python/Cython level, not here !
	 * Another approach is to DECREF in the destructor (See ~LabUserData()).
	 */
	//if (this->m_obj) Py_XDECREF(this->m_obj);
	this->m_obj = obj;
	//Py_XINCREF(this->m_obj);
}

PyObject * LabUserData::CyGetPyData() {
	return this->m_data;
}

void LabUserData::CySetPyData(PyObject *obj) {

	if (obj == NULL) { this->m_data = NULL; return; }

	if (this->m_data) Py_XDECREF(this->m_data);
	this->m_data = obj;
	Py_XINCREF(this->m_data);
}

#endif

} /* namespace elps */
