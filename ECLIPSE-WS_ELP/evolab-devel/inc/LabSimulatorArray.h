/*
 * LabSimulatorArray.h
 *
 *  Created on: Apr 6, 2013
 *      Author: golgauth
 */

#ifndef LABSIMULATORARRAY_H_
#define LABSIMULATORARRAY_H_


#include <LabSimulatorBase.h>
#include <LabArrayBinding.h>

namespace elps {

class LIB_CLASS LabSimulatorArray: public LabSimulatorBase, public LabArrayBinding {
public:
	LabSimulatorArray();
	virtual ~LabSimulatorArray();
};

} /* namespace elps */
#endif /* LABSIMULATORARRAY_H_ */
