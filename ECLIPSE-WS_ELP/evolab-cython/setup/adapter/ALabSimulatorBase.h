/*
 * ILabSimulatorTime.h
 *
 *  Created on: 4 oct. 2012
 *      Author: golgauth
 */

#ifndef ALABSIMULATORTIME_H_
#define ALABSIMULATORTIME_H_

// Created by Cython when providing 'public api' keywords
#include "../cyelp_api.h"

#include "../../inc/LabSimulatorBase.h"

namespace elps {

class ALabSimulatorBase: public LabSimulatorBase {
public:
	PyObject *m_obj;

	ALabSimulatorBase() {};
	ALabSimulatorBase(PyObject *obj);
	virtual ~ALabSimulatorBase();

	// To be absolutely overridden :
	virtual void InitSimulation();
	virtual void StepSimulation();
	virtual void BeforeStep();
	virtual void AfterStep();
	virtual void OutputSimulation();
	virtual bool EndSimulation();

	virtual void InitUnBinarize();
	virtual void StepBinarize();

	// Override it only if the default behaviour doesn't match your needs :
	virtual void RunSimulation();
	virtual void BeforeRun();
	virtual void AfterRun();
	////virtual void Run();

	// Override it only if you need the binary features
	virtual void BinarizeAll();
	virtual void UnBinarizeAll();

};

} /* namespace elps */
#endif /* ALABSIMULATORTIME_H_ */
