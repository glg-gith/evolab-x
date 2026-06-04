/*
 * ALabNetBinding.cpp
 *
 *  Created on: Feb 16, 2013
 *      Author: golgauth
 */

#include "ALabNetBinding.h"

namespace elps {

ALabNode::ALabNode(): LabNode() {
}

ALabNode::ALabNode(PyObject *obj): LabNode() {
//	this->m_obj = obj;
//	// Provided by "cyelp_api.h"
//	if (import_cyelp()) {
//	} else {
//		Py_XINCREF(this->m_obj);
//	}
	this->SetPyObj(obj);
}
PyObject *ALabNode::GetPyObj() {
	////return this->m_obj;
	return this->CyGetPyObj();
}
void ALabNode::SetPyObj(PyObject *obj) {

//	this->m_obj = obj;
//	// Provided by "cyelp_api.h"
//	if (import_cyelp()) {
//	} else {
//		Py_XINCREF(this->m_obj);
//	}

	this->SetCyPyObj(obj);
	// Provided by "cyelp_api.h"
	if (import_cyelp()) {
	} else {
		Py_XINCREF(obj);
	}
}


ALabNetwork::ALabNetwork(PyObject *obj): LabNetwork() {
	this->m_obj = obj;
	// Provided by "cyelp_api.h"
	if (import_cyelp()) {
	} else {
		Py_XINCREF(this->m_obj);
	}
}
PyObject *ALabNetwork::GetPyObj() {
	return this->m_obj;
}


} /* namespace elps */
