/*
 * ALab.cpp
 *
 *  Created on: Feb 13, 2013
 *      Author: golgauth
 */

#include "ALabSiteSetsManager.h"

#include "Python.h"


namespace elps {



ALabSet::ALabSet(): LabSet() {

}
ALabSet::ALabSet(const LabSet& src): LabSet(src) {

}


ALabSet::ALabSet(PyObject *obj): LabSet/*<ALabUserData *>*/() {

	this->m_obj = obj;
//	// Provided by "cyelp_api.h"
//	if (import_cyelp()) {
//	} else {
////		Py_XINCREF(this->m_obj);
//	}

}

ALabSet::ALabSet(PyObject *obj, unsigned int sizeinbits): LabSet(sizeinbits) {

	this->m_obj = obj;
//	// Provided by "cyelp_api.h"
//	if (import_cyelp()) {
//	} else {
////		Py_XINCREF(this->m_obj);
//	}

}

ALabSet::ALabSet(PyObject *obj, const ALabSet& src): LabSet/*<ALabUserData *>*/(src) {

	this->m_obj = obj;
//	// Provided by "cyelp_api.h"
//	if (import_cyelp()) {
//	} else {
////		Py_XINCREF(this->m_obj);
//	}


}

//ALabSet::~ALabSet() {
////	Py_XDECREF(this->m_obj);
//	cout << "delete from ADAPTER !!" << endl;
//}


PyObject *ALabSet::GetPyObj() {
	return this->m_obj;
}


//
//
//ALabSiteSetsManager::ALabSiteSetsManager(PyObject *obj) {
//
//	this->m_obj = obj;
//	// Provided by "cyelp_api.h"
//	if (import_cyelp()) {
//	} else {
//		Py_XINCREF(this->m_obj);
//	}
//
//	setsManager = LabSiteSetsManager::GetInstance();
//}
//
//ALabSiteSetsManager::~ALabSiteSetsManager() {
//	Py_XINCREF(this->m_obj);
//}
//
//
//PyObject *ALabSiteSetsManager::GetPyObj() {
//	return this->m_obj;
//}


} /* namespace elps */
