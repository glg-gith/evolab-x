/*
 * LabSimulatorTimeArray.h
 *
 *  Created on: Apr 6, 2013
 *      Author: golgauth
 */

#ifndef LABSIMULATORTIMEARRAY_H_
#define LABSIMULATORTIMEARRAY_H_


#include <LabSimulatorTime.h>
#include <LabArrayBinding.h>

namespace elps {

/**
 * Class designed to create MASS ACTION and TIME based simulation.</br>
 * It's got all the features of a LabSimulatorTime simulation
 * but additionally aggregates a list of Sites via the
 * LabArrayBinding inheritance.
 *
 * COMMON USAGE :
 *
 * - Create a subclass of LabSimulatorTimeArray
 * - Feel free to override the hooks (virtual) functions fitting your needs
 *
 */
class LIB_CLASS LabSimulatorTimeArray : public LabSimulatorTime, public LabArrayBinding {
public:
	LabSimulatorTimeArray();
	virtual ~LabSimulatorTimeArray();
};

} /* namespace elps */
#endif /* LABSIMULATORTIMEARRAY_H_ */
