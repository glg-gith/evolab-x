/*
 * LabSimulatorTimeNetEvent.h
 *
 *  Created on: Feb 13, 2013
 *      Author: golgauth
 */

#ifndef LABSIMULATORTIMENETEVENT_H_
#define LABSIMULATORTIMENETEVENT_H_


#include <LabConsts.h>

namespace elps {

/**
 * !!!!!!!!!    Class qui mache le travail au max (updateRates, ...) par rapport a LabSimulatorTimeNet    !!!!!!!!!!!!!
 * Qui serait donc specialisee dans les LabSiteEvent
 */
class LIB_CLASS LabSimulatorTimeNetEvent {
public:
	LabSimulatorTimeNetEvent();
	virtual ~LabSimulatorTimeNetEvent();
};

} /* namespace elps */
#endif /* LABSIMULATORTIMENETEVENT_H_ */
