/*
 * ALabSite.h
 *
 *  Created on: Feb 15, 2013
 *      Author: golgauth
 */

#ifndef ALABSITE_H_
#define ALABSITE_H_

// Created by Cython when providing 'public api' keywords
#include "../cyelp_api.h"

#include "../../inc/LabUserData.h"
#include "../../inc/LabSiteBase.h"
#include "../../inc/LabSiteEvent.h"

#include "../../inc/LabSimulatorBase.h"

namespace elps {

class ALabUserData: public LabUserData {
public:
	////PyObject *m_obj;
	ALabUserData();
	ALabUserData(PyObject *obj);
	PyObject *GetPyObj();
	void SetPyObj(PyObject *obj);

	////PyObject *m_data;
	ALabUserData(PyObject *obj, PyObject *data);
	PyObject *GetData();
	void SetData(PyObject *data);
};

class ALabSiteBase: /*public ALabUserData,*/ public LabSiteBase {
public:
	////PyObject *m_obj;
	//ALabSiteBase(PyObject *obj);
	ALabSiteBase(PyObject *obj, LabSimulatorBase *sim);
	ALabSiteBase(PyObject *obj, LabSimulatorBase *sim, LabNode *parent_node);
	PyObject *GetPyObj();
	void SetPyObj(PyObject *obj);
};

class ALabSiteEvent: /*public ALabUserData,*/ public LabSiteEvent {
public:
	////PyObject *m_obj;
	//ALabSiteEvent(PyObject *obj);
	ALabSiteEvent(PyObject *obj, LabSimulatorBase * sim);
	ALabSiteEvent(PyObject *obj, LabSimulatorBase * sim, LabNode *parent_node);
	PyObject *GetPyObj();
	void SetPyObj(PyObject *obj);
};

} /* namespace elps */
#endif /* ALABSITE_H_ */
