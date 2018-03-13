/*
 * ALabNetBinding.h
 *
 *  Created on: Feb 16, 2013
 *      Author: golgauth
 */

#ifndef ALABNETBINDING_H_
#define ALABNETBINDING_H_

// Created by Cython when providing 'public api' keywords
#include "../cyelp_api.h"


#include "../../inc/LabNetwork.h"


namespace elps {

class ALabNode: public LabNode {
public:
////	PyObject *m_obj;
	ALabNode();
	ALabNode(PyObject *obj);
	PyObject *GetPyObj();
	void SetPyObj(PyObject *obj);
};

class ALabNetwork: public LabNetwork {
public:
	PyObject *m_obj;
	ALabNetwork(PyObject *obj);
	PyObject *GetPyObj();
};


} /* namespace elps */
#endif /* ALABNETBINDING_H_ */
