/*
 * LabSimulatorArray.h
 *
 *  Created on: 12 sept. 2012
 *      Author: golgauth
 */

#ifndef LABSIMULATORARRAY_H_
#define LABSIMULATORARRAY_H_

#include <LabSimulatorTime.h>
//#include "LabNetBinding.h"


namespace elps {

/**
 * Deterministic : uses GSL equation solvers
 * Override TimeStep() !!! (not logarithmic)
 * attribute and event stacks managed at the simulation level (not at the individual level)
 * => Attribute (ex. genotype) is a new array in the sim
 */
class LIB_CLASS LabSimulatorDiffEq: public LabSimulatorTime/*, public LabNetBinding*/ {
public:
	LabSimulatorDiffEq();
	virtual ~LabSimulatorDiffEq();

};


}  // namespace elps

#endif /* LABSIMULATORARRAY_H_ */
