/*
 * BdmLsim3.h
 *
 *  Created on: 18 sept. 2012
 *      Author: golgauth
 */

#ifndef BDMLSIM3_H_
#define BDMLSIM3_H_


#include <LabSiteEvent.h>
#include <LabSimulatorTime.h>
#include <LabNetBinding.h>
#include <LabActionStack.h>
#include <LabOutputerLine.h>

#include <LabSiteSetsCalculator.h>



#include <LabRngUtils.h>
#include <LabTimeUtils.h>
#include <LabCallBack2.h>
#include <LabBinaryUtils.h>

#include <vector>
#include <sstream>

#include <gsl/gsl_rng.h>
#include <math.h>
#include <limits>
#include <cmath>

#include <CmdLine.h>

#include <libxml/parser.h>
#include <LabXMLUtils.h>



#include <algorithm>


using namespace TCLAP;
using namespace elps;


class BdmLsim3 : public LabSimulatorTime, public LabNetBinding {
public:

	BdmLsim3();
	virtual ~BdmLsim3();

	typedef enum { EMPTY, RESIDENT, MUTANT } t_state;
	typedef enum { POP, X, M } t_site_attr;
	typedef LabSiteEvent t_site;

	void DefaultInputs();
	string ParseCL(int argc, char **argv);
	void ParseXML(string filename);
	void CheckInputs();
	void PrintInputs();
	void SetOutputFilenames();

	// Base class virtuals override
	void InitSimulation();
	//void StepSimulation();				// Do not override : the implementation from 'LabSimulatorTime' is OK !
	void BeforeStep();
	void AfterStep();
	void BeforeRun();
	void AfterRun();
	void OutputSimulation();				// Use preferably the 'LabOutputer' utility in this hook
	void FireEvent();
	//bool EndSimulation();					// Do not override : the implementation from 'LabSimulatorTime' is OK !
	void BinarizeAll();
	void UnBinarizeAll();

	// Override default simulator behaviour
	void Run();




	static t_site *GetSiteData(LabNode *a_node);

	static int NbOccupiedNghs(LabNode *a_node);
	static void UpdateRatesLocal(LabNode *a_node);
	static void UpdateRates(LabNode *a_node);


private:

	typedef enum { NONE, BIRTH, DEATH, MOVEMENT, MUTATION } t_event;

	void InitPopulation();

	// Rates callbacks
	RATE_CALLBACK_FUNC FNoneRate;
	RATE_CALLBACK_FUNC FBirthRate;
	RATE_CALLBACK_FUNC FDeathRate;
	RATE_CALLBACK_FUNC FMovementRate;
	RATE_CALLBACK_FUNC FMutationRate;


	void ResynchTotalRate();

	// Events callbacks
	void RealizeEvent();
	REALIZE_CALLBACK_FUNC FRealizeNone;
	REALIZE_CALLBACK_FUNC FRealizeBirth;
	REALIZE_CALLBACK_FUNC FRealizeDeath;
	REALIZE_CALLBACK_FUNC FRealizeMovement;
	REALIZE_CALLBACK_FUNC FRealizeMutation;


	// Inputs
	void UpdateInputs();

	// Outputs
	unsigned int Census();
	void DumpLattice();
	LabOutputerLine *outputer;
	LabOutputerLine *outputer2;
	LabOutputerLine *outputer3;
	string outputFilename;
	string outputFilename2;
	string outputFilename3;

	LabActionStack *actionStack;

};

#endif /* BDMLSIM3_H_ */
