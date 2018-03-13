/*
 * LabSimulatorDiffEqEventInd.h
 *
 *  Created on: Feb 13, 2013
 *      Author: golgauth
 */

#ifndef LABSIMULATORDIFFEQEVENTIND_H_
#define LABSIMULATORDIFFEQEVENTIND_H_


#include <LabSimulatorDiffEqEvent.h>
#include <LabSiteSetsManager.h>
#include <LabSiteBase.h>


namespace elps {

/**
 * Inherits from LabSimulatorDiffEqEvent for event handling at the simulation's level + dealing with individuals (aka : LabSiteBase objects).
 */
class LIB_CLASS LabSimulatorDiffEqEventInd: public LabSimulatorDiffEqEvent {
public:
	LabSimulatorDiffEqEventInd();
	virtual ~LabSimulatorDiffEqEventInd();

	/**
	 * Instantiate "critical_size" objects of type LabSiteBase. Each of them subscribes to the
	 * LabSiteSetsManager belonging to the "pop_name" set/group.
	 * Each of them is also created with a default "EMPTY" STATE. It belongs to the user to then
	 * initialize the whole population (adding some states and attributes to some of those SITEs).
	 *
	 * @param critical_size The maximum critical size of the population (limit for the possibility of reproduction)
	 * @param pop_name Name of the newly created group of sites (aka : population)
	 * @return
	 */
	// Viré because on peu utiliser un system unique "U" et discriminer ensuite les individus en leur donnant
	// un attribute indexé "pop_name" (impliquant la creation d'autant de sous-pops que indiqué dans le "nb_beans")
	////vector<LabSiteBase *> CreatePopulation(int critical_size, string pop_name);
	/**
	 * Instantiate "critical_size" objects of type LabSiteBase. Each of them subscribes to the
	 * LabSiteSetsManager belonging to the "U" set/group (the UNIVERS of the simulation).
	 * Each of them is also created with a default "EMPTY" STATE. It belongs to the user to then
	 * initialize the whole population (adding some states and attributes to some of those SITEs).
	 * @param critical_size
	 * @return
	 */
	vector<LabSiteBase *> CreateSystem(int critical_size);


	////	inline vector<LabSiteBase *> GetPopulation(int id) { return populations[id]; }
	////
	////	inline vector<LabSiteBase *> GetPopulation(string name) { /* !!!!!!!!!    TODO    !!!!!!!!!*/ return NULL; };

//	/**
//	 * Get the "Set Theory" manager (mainly used to group individuals in logical sets and subsets,
//	 * according to their state and attributes).
//	 * @return An individuals sets manager
//	 */
//	LabSetsBinding *GetSetsBinding();

private:

//	LabSetsBinding/*<LabSiteBase*>*/ *setsBinding;

	////	vector<vector<LabSiteBase *> > populations;

};

} /* namespace elps */
#endif /* LABSIMULATORDIFFEQEVENTIND_H_ */




