/*
 * LabSimulatorNet.h
 *
 *  Created on: 12 sept. 2012
 *      Author: golgauth
 */

#ifndef LABSIMULATORNET_H_
#define LABSIMULATORNET_H_

#include "LabSimulatorBase.h"
#include "LabNetBinding.h"


namespace elps {

/**
 * Class designed to create SPACE based simulation.</br>
 * It's got all the features of a LabSimulatorBase simulation
 * but additionally aggregates a network (See LabNetwork) via
 * the LabNetBinding inheritance.
 *
 * COMMON USAGE :
 *
 * - Create a subclass of LabSimulatorNet
 * - Feel free to override the hooks (virtual) functions fitting your needs
 *
 */
class LIB_CLASS LabSimulatorNet: public LabSimulatorBase, public LabNetBinding {
public:
	LabSimulatorNet();
	virtual ~LabSimulatorNet();
};


}  // namespace elps

#endif /* LABSIMULATORNET_H_ */
