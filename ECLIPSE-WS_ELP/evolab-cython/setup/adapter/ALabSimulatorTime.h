/*
 * ILabSimulatorTime.h
 *
 *  Created on: 4 oct. 2012
 *      Author: golgauth
 */

#ifndef ALABSIMULATORBASE_H_
#define ALABSIMULATORBASE_H_

// Created by Cython when providing 'public api' keywords
#include "../cyelp_api.h"

#include "../../inc/LabSimulatorTime.h"

namespace elps {

class ALabSimulatorTime: public LabSimulatorTime {
public:
	PyObject *m_obj;

	ALabSimulatorTime(PyObject *obj);
	virtual ~ALabSimulatorTime();

	// Additional features
	virtual void ResetTime();
    virtual double TimeStep();
    virtual void FireEvent();			// Should be pure virtual,
    									// but ALabSimulatorTime is to be instantiated
    // Reimplemented features
    virtual void StepSimulation();
    virtual bool EndSimulation();
    virtual void RunSimulation();

    // Pasted from Base //
	// To be absolutely overridden :
	virtual void InitSimulation();
	virtual void BeforeStep();
	virtual void AfterStep();
	virtual void OutputSimulation();

	virtual void InitUnBinarize();
	virtual void StepBinarize();

	// Override it only if the default behaviour doesn't match your needs :
	virtual void BeforeRun();
	virtual void AfterRun();
	////virtual void Run();

	// Override it only if you need the binary features
	virtual void BinarizeAll();
	virtual void UnBinarizeAll();

};

} /* namespace elps */
#endif /* ALABSIMULATORBASE_H_ */
