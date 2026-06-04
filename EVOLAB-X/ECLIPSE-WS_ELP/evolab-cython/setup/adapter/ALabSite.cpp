/*
 * ALabSite.cpp
 *
 *  Created on: Feb 15, 2013
 *      Author: golgauth
 */

#include "ALabSite.h"

namespace elps {

ALabUserData::ALabUserData(): LabUserData() {
//	this->m_obj = NULL;
//	this->m_data = NULL;
	//cout << "PY INIT [ALabUserData (default constructor)] = " << this->m_obj << endl;
}
ALabUserData::ALabUserData(PyObject *obj): LabUserData() {
//	this->m_obj = obj;
//	// Provided by "cyelp_api.h"
//	if (import_cyelp()) {
//	} else {
//		Py_XINCREF(this->m_obj);
//	}
//	this->m_data = NULL;
//	//cout << "PY INIT [ALabUserData] = " << this->m_obj << endl;

	this->SetPyObj(obj);
}
PyObject *ALabUserData::GetPyObj() {
	//cout << "PY OBJECT [ALabUserData] = " << this->m_obj << endl;
//	return this->m_obj;
	return (PyObject *)this->GetCyPyObj();
}
void ALabUserData::SetPyObj(PyObject *obj) {
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

ALabUserData::ALabUserData(PyObject *obj, PyObject *data) {
//	this->m_obj = obj;
//	// Provided by "cyelp_api.h"
//	if (import_cyelp()) {
//	} else {
//		Py_XINCREF(this->m_obj);
//	}

	this->SetPyObj(obj);

//	this->m_data = data;
//	// Provided by "cyelp_api.h"
//	if (import_cyelp()) {
//	} else {
//		Py_XINCREF(this->m_data);
//	}

	this->SetData(obj);
}
PyObject *ALabUserData::GetData() {
//	return this->m_data;
	return (PyObject *)this->GetCyData();
}
void ALabUserData::SetData(PyObject *data) {
//	this->m_data = data;
//	// Provided by "cyelp_api.h"
//	if (import_cyelp()) {
//	} else {
//		Py_XINCREF(this->m_data);
//	}

	this->SetCyData(data);
	// Provided by "cyelp_api.h"
	if (import_cyelp()) {
	} else {
		Py_XINCREF(data);
	}
}


ALabSiteBase::ALabSiteBase(PyObject *obj, LabSimulatorBase *sim): LabSiteBase(sim) {
//	this->m_obj = obj;
//	// Provided by "cyelp_api.h"
//	if (import_cyelp()) {
//	} else {
//		Py_XINCREF(this->m_obj);
//	}
//	//cout << "PY INIT [ALabSiteBase] = " << this->m_obj << endl;
//
//	this->SetCyPyObj(this->m_obj);

	this->SetPyObj(obj);
}

ALabSiteBase::ALabSiteBase(PyObject *obj, LabSimulatorBase *sim, LabNode *parent_node): LabSiteBase(sim, parent_node) {
//	this->m_obj = obj;
//	// Provided by "cyelp_api.h"
//	if (import_cyelp()) {
//	} else {
//		Py_XINCREF(this->m_obj);
//	}
//	//cout << "PY INIT [ALabSiteBase] = " << this->m_obj << endl;
//
//	this->SetCyPyObj(this->m_obj);

	this->SetPyObj(obj);
}
PyObject *ALabSiteBase::GetPyObj() {
	//cout << "PY OBJECT [ALabUserData] = " << this->m_obj << endl;
//	return this->m_obj;
	return (PyObject *)this->GetCyPyObj();
}
void ALabSiteBase::SetPyObj(PyObject *obj) {
//	this->m_obj = obj;
//	// Provided by "cyelp_api.h"
//	if (import_cyelp()) {
//	} else {
//		Py_XINCREF(this->m_obj);
//	}
//
//	this->SetCyPyObj(this->m_obj);

	this->SetCyPyObj(obj);
	// Provided by "cyelp_api.h"
	if (import_cyelp()) {
	} else {
		Py_XINCREF(obj);
	}
}


ALabSiteEvent::ALabSiteEvent(PyObject *obj, LabSimulatorBase *sim): LabSiteEvent(sim) {
//	this->m_obj = obj;
//	// Provided by "cyelp_api.h"
//	if (import_cyelp()) {
//	} else {
//		Py_XINCREF(this->m_obj);
//	}
//	//cout << "PY INIT [ALabSiteEvent] = " << this->m_obj << endl;
//
//	this->SetCyPyObj(this->m_obj);

	this->SetPyObj(obj);
}

ALabSiteEvent::ALabSiteEvent(PyObject *obj, LabSimulatorBase *sim, LabNode *parent_node): LabSiteEvent(sim, parent_node) {
//	this->m_obj = obj;
//	// Provided by "cyelp_api.h"
//	if (import_cyelp()) {
//	} else {
//		Py_XINCREF(this->m_obj);
//	}
//	//cout << "PY INIT [ALabSiteBase] = " << this->m_obj << endl;
//
//	this->SetCyPyObj(this->m_obj);

	this->SetPyObj(obj);
}
PyObject *ALabSiteEvent::GetPyObj() {
	//cout << "PY OBJECT [ALabUserData] = " << this->m_obj << endl;
//	return this->m_obj;
	return (PyObject *)this->GetCyPyObj();
}
void ALabSiteEvent::SetPyObj(PyObject *obj) {
	//	this->m_obj = obj;
	//	// Provided by "cyelp_api.h"
	//	if (import_cyelp()) {
	//	} else {
	//		Py_XINCREF(this->m_obj);
	//	}
	//
	//	this->SetCyPyObj(this->m_obj);

	this->SetCyPyObj(obj);
	// Provided by "cyelp_api.h"
	if (import_cyelp()) {
	} else {
		Py_XINCREF(obj);
	}
}

} /* namespace elps */
