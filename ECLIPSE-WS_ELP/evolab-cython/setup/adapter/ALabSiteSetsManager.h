/*
 * ALabSiteSetsManager.h
 *
 *  Created on: Feb 13, 2013
 *      Author: golgauth
 */

#ifndef ALABSITESETSMANAGER_H_
#define ALABSITESETSMANAGER_H_

// Created by Cython when providing 'public api' keywords
#include "../cyelp_api.h"

#include "../../inc/LabSet.h"
//#include "ALabSite.h"

#include "../../inc/LabSiteSetsManager.h"

//#define CYTHON_REFNANNY 1

#include <iostream>

using namespace std;

namespace elps {

//class ALabSetSite: public LabSetSite {
class ALabSet: public LabSet/*<ALabUserData *>*/ {
public:
	PyObject *m_obj;

	ALabSet();
    ALabSet(const LabSet& src);

	ALabSet(PyObject *obj);
	ALabSet(PyObject *obj, unsigned int sizeinbits);
	ALabSet(PyObject *obj, const ALabSet& src);

//	virtual ~ALabSet();

	PyObject *GetPyObj();

};

//
//class ALabSiteSetsManager {
//public:
//	PyObject *m_obj;
//
//	ALabSiteSetsManager(PyObject *obj);
//	virtual ~ALabSiteSetsManager();
//
//	PyObject *GetPyObj();
//
//	// "inline" LabSiteSetsManager features
//	inline void SomethingTheManagerDoes() { cout << "Sets manager call - getInstance() !" << endl; }
//
////	inline LabSet<t_elt> GS  (string set_name);
////	inline int           CNT (LabSet<t_elt> const& a_set) { return this->CNT(a_set); }
////	inline double        SUM (LabSet<t_elt> const& a_set, string attr_name) { return this->SUM(a_set, attr_name); }
////	inline double        AVG (LabSet<t_elt> const& a_set, string attr_name) { return this->AVG(a_set, attr_name); }
//
//
//private:
//	LabSiteSetsManager *setsManager;
//};

} /* namespace elps */
#endif /* ALABSITESETSMANAGER_H_ */
