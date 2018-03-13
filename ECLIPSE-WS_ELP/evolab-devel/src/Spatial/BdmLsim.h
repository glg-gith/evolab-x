/*
 * BdmLsim.h
 *
 *  Created on: 13 sept. 2012
 *      Author: golgauth
 */

#ifndef BDMLSIM_H_
#define BDMLSIM_H_

#include <LabSimulatorTime.h>
#include <LabNetBinding.h>
#include <LabStateStack.h>
#include <LabEventStack.h>
#include <LabActionStack.h>
#include <LabOutputerLine.h>

#include <LabRngUtils.h>
#include <LabTimeUtils.h>
#include <LabCallBack2.h>
#include <LabBinaryUtils.h>


#include <algorithm>
#include <CmdLine.h>


#include <vector>
#include <sstream>

#include <gsl/gsl_rng.h>

using namespace TCLAP;

using namespace elps;


class BdmLsim : public LabSimulatorTime, public LabNetBinding {
public:
	BdmLsim();
	virtual ~BdmLsim();

	/* Each site is characterized by its state (resident, mutant, for instance), trait values x and m, rates for the different events that can happen, and an array that contains the indices of the neighbouring sites (in a previous version I did that with pointers, which was cleaner I guess...) */
	typedef struct
	{
		// STATES : EMPTY, RESIDENT, MUTANT
		LabStateStack *stateStack;

		// EVENTS : NONE, BIRTH, DEATH, MOVEMENT, MUTATION
		// TODO : Should we implement event driven simulation in a class "LabSimulatorNetEvent:LabSimulatorNet" ???
		// TODO : If yes, we'll also need a class "LabSimulatorArrayEvent:LabSimulatorArray" ???
		// TODO : Note : Events are managed in a very different way in networked and arrayed simulations !!
		LabEventStack *eventStack;

		// ADDITIONAL TRAITS
		double x;	// affects birth & death rates
		double m;	// affects movement
	//	LabAttributesStack *attrStack;
	} t_site;


	// Base class virtuals
	void InitSimulation();
	//void StepSimulation();				// Do not implement : the implementation from 'LabSimulatorTime' is OK !
	void BeforeStep();
	void AfterStep();
	void BeforeRun();				// TODO : put TOTAL SIMULATION TIME in there !
	void AfterRun();				// TODO : put TOTAL SIMULATION TIME in there !
	void OutputSimulation();				// Use preferably the 'LabOutputer' utility in this hook
	void FireEvent();
	//bool EndSimulation();					// Do not implement : the implementation from 'LabSimulatorTime' is OK !


	typedef enum { EMPTY, RESIDENT, MUTANT } t_state;

	static t_site *GetSiteData(Node *a_node);
	static int NbOccupiedNghs(Node *a_node);
	static void UpdateRatesLocal(Node *a_node);
	static void UpdateRates(Node *a_node);


private:

	typedef enum { NONE, BIRTH, DEATH, MOVEMENT, MUTATION } t_event;

	void InitInputs();
	void InitPopulation();

	// Rates callbacks
	//double BirthRate(void *node);
	//double DeathRate(Node *a_node);
	//double MovementRate(Node *a_node);
	//double MutationRate(Node *a_node);
	RATE_CALLBACK_FUNC FNoneRate;
	RATE_CALLBACK_FUNC FBirthRate;
	RATE_CALLBACK_FUNC FDeathRate;
	RATE_CALLBACK_FUNC FMovementRate;
	RATE_CALLBACK_FUNC FMutationRate;


	void ResynchTotalRate();

	// Events callbacks
	void RealizeEvent();
	//void RealizeBirth(void *node);
	//void RealizeDeath(Node *node);
	//void RealizeMovement(Node *node);
	//void RealizeMutation(Node *node);
	REALIZE_CALLBACK_FUNC FRealizeNone;
	REALIZE_CALLBACK_FUNC FRealizeBirth;
	REALIZE_CALLBACK_FUNC FRealizeDeath;
	REALIZE_CALLBACK_FUNC FRealizeMovement;
	REALIZE_CALLBACK_FUNC FRealizeMutation;


	// Outputs
	unsigned int Census();
	void DumpLattice();


	LabOutputerLine *outputer;
	LabOutputerLine *outputer2;
	LabOutputerLine *outputer3;
	string outputFilename;
	string outputFilename2;
	string outputFilename3;

	//    vector<Node*> resident;
	//    vector<Node*> mutant;

	LabActionStack *actionStack;

};

#endif /* BDMLSIM_H_ */
