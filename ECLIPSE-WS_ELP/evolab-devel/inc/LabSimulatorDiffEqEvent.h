/*
 * LabSimulatorDiffEqEvent.h
 *
 *  Created on: Feb 9, 2013
 *      Author: golgauth
 */

#ifndef LABSIMULATORDIFFEQEVENT_H_
#define LABSIMULATORDIFFEQEVENT_H_

#include <LabSimulatorTime.h>

#include <LabEventStack.h>

namespace elps {

/**
 * Event driven version of LabSimulatorDiffEq, but has nothing to do with LabSimulatorDiffEq !!!
 */
class LIB_CLASS LabSimulatorDiffEqEvent: public LabSimulatorTime {
public:
	LabSimulatorDiffEqEvent();
	virtual ~LabSimulatorDiffEqEvent();

private:
	LabEventStack *eventsStack;
};

} /* namespace elps */
#endif /* LABSIMULATORDIFFEQEVENT_H_ */
