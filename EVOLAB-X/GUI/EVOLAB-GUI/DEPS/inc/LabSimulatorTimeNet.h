/*
 * LabSimulatorTimeNet.h
 *
 *  Created on: Feb 1, 2013
 *      Author: golgauth
 */

#ifndef LABSIMULATORTIMENET_H_
#define LABSIMULATORTIMENET_H_

#include <LabSimulatorTime.h>
#include <LabNetBinding.h>

namespace elps {

/**
 * Class designed to create SPACE and TIME based simulation.</br>
 * It's got all the features of a LabSimulatorTime simulation
 * but additionally aggregates a network (See LabNetwork) via
 * the LabNetBinding inheritance.
 *
 * COMMON USAGE :
 *
 * - Create a subclass of LabSimulatorTimeNet
 * - Feel free to override the hooks (virtual) functions fitting your needs
 *
 */
class LIB_CLASS LabSimulatorTimeNet: public LabSimulatorTime, public LabNetBinding {
public:
	LabSimulatorTimeNet();
	virtual ~LabSimulatorTimeNet();
};

} /* namespace elps */
#endif /* LABSIMULATORTIMENET_H_ */
