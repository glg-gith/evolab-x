/*
 * DrugRes.h
 *
 *  Created on: 18 sept. 2012
 *      Author: golgauth
 */

#ifndef DRUGRES_H_
#define DRUGRES_H_


#include <LabSiteBase.h>
#include <LabSimulatorTimeArray.h>
#include <LabEventStack.h>
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



class DrugRes : public LabSimulatorTimeArray {
public:

	DrugRes();
	virtual ~DrugRes();

	typedef enum { EMPTY, SUSCEPTIBLE, INFECTED } t_state;
	typedef enum { TREATED/*, a, b, c*/, GENO } t_site_attr;
	typedef enum { alpha = 0, beta = 1, gamma = 2 } t_attr_id;		// virulence, transmission, clearance

	//typedef LabSiteBase t_site;

	void DefaultInputs();
//	string ParseCL(int argc, char **argv);
//	void ParseXML(string filename);
//	void CheckInputs();
	void PrintInputs();
	void SetOutputFilenames();

	void DefaultModels();

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
	////void BinarizeAll();
	////void UnBinarizeAll();

	// Override default simulator behaviour
	////void Run();


private:

	//typedef enum { BIRTH, DEATH, INFECTION, CLEARANCE, SUPERINFECTION, MUTATION } t_event;

	void InitPopulation();

	void Update_F_Averages();

//	// Rates callbacks
	RATE_CALLBACK_FUNC FBirthRate;
	RATE_CALLBACK_FUNC FDeathRate;
	RATE_CALLBACK_FUNC FInfectionRate;
	RATE_CALLBACK_FUNC FClearanceRate;
	RATE_CALLBACK_FUNC FSuperinfectionRate;
	RATE_CALLBACK_FUNC FMutationRate;


//	void ResynchTotalRate();

//	// Events callbacks
	void RealizeEvent();
	REALIZE_CALLBACK_FUNC FRealizeBirth;
	REALIZE_CALLBACK_FUNC FRealizeDeath;
	REALIZE_CALLBACK_FUNC FRealizeInfection;
	REALIZE_CALLBACK_FUNC FRealizeClearance;
	REALIZE_CALLBACK_FUNC FRealizeSuperinfection;
	REALIZE_CALLBACK_FUNC FRealizeMutation;


	// Outputs
	void Census();
	LabOutputerLine *outputer;
//	LabOutputerLine *outputer2;
	string outputFilename;
//	string outputFilename2;

	// <//DRUGRES//>
	// In this demo, firing events is managed at the simulation level
	// (in "BdmLsimX", events occurred at the site level).
	LabEventStack *eventStack;

	LabActionStack *actionStack;

};

#endif /* DRUGRES_H_ */
