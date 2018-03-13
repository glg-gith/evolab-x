/*
 * ILabSimulatorTime.cpp
 *
 *  Created on: 4 oct. 2012
 *      Author: golgauth
 */

#include "ALabSimulatorTime.h"

namespace elps {

ALabSimulatorTime::ALabSimulatorTime(PyObject *obj) {
	this->m_obj = obj;
	// Provided by "cyelp_api.h"
	if (import_cyelp()) {
	} else {
		Py_XINCREF(this->m_obj);
	}
}

ALabSimulatorTime::~ALabSimulatorTime() {
	Py_XDECREF(this->m_obj);
}


void ALabSimulatorTime::ResetTime()
{
	if (this->m_obj) {
		int error;
		// Call a virtual overload, if it exists
		cy_call_func_void(this->m_obj, (char*)"ResetTime", &error);
		if (error)
			// Call parent method
			LabSimulatorTime::ResetTime();
	}
	// else : Throw an error ?
}


double ALabSimulatorTime::TimeStep()
{
	if (this->m_obj) {
		int error;
		double result = cy_call_func_double(this->m_obj, (char*)"TimeStep", &error);
		if (error) result = LabSimulatorTime::TimeStep();
		return result;
	}
	return NAN;
}

void ALabSimulatorTime::FireEvent()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"FireEvent", &error);
		if (error) LabSimulatorTime::FireEvent();
	}
}

void ALabSimulatorTime::StepSimulation()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"StepSimulation", &error);
		if (error) LabSimulatorTime::StepSimulation();
	}
}

bool ALabSimulatorTime::EndSimulation()
{
	if (this->m_obj) {
		int error;
		bool result = cy_call_func_bool(this->m_obj, (char*)"EndSimulation", &error);
		if (error) result = LabSimulatorTime::EndSimulation();
		return result;
	}
	return true;
}

void ALabSimulatorTime::RunSimulation()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"RunSimulation", &error);
		if (error) LabSimulatorTime::RunSimulation();
	}
}


// From base

void ALabSimulatorTime::InitSimulation()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"InitSimulation", &error);
		if (error) LabSimulatorTime::InitSimulation();
	}
}

void ALabSimulatorTime::BeforeStep()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"BeforeStep", &error);
		if (error) LabSimulatorTime::BeforeStep();
	}
}

void ALabSimulatorTime::AfterStep()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"AfterStep", &error);
		if (error) LabSimulatorTime::AfterStep();
	}
}

void ALabSimulatorTime::OutputSimulation()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"OutputSimulation", &error);
		if (error) LabSimulatorTime::OutputSimulation();
	}
}

void ALabSimulatorTime::InitUnBinarize()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"InitUnBinarize", &error);
		if (error) LabSimulatorTime::InitUnBinarize();
	}
}

void ALabSimulatorTime::StepBinarize()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"StepBinarize", &error);
		if (error) LabSimulatorTime::StepBinarize();
	}
}

void ALabSimulatorTime::BeforeRun()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"BeforeRun", &error);
		if (error) LabSimulatorTime::BeforeRun();
	}
}

void ALabSimulatorTime::AfterRun()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"AfterRun", &error);
		if (error) LabSimulatorTime::AfterRun();
	}
}
//
//void ALabSimulatorBase::Run()
//{
//	if (this->m_obj) {
//		int error;
//		cout << "Runnin'..." << endl;
//		cy_call_func_void(this->m_obj, (char*)"Run", &error);
//		if (error) LabSimulatorBase::Run();
//	}
//}

void ALabSimulatorTime::BinarizeAll()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"BinarizeAll", &error);
		if (error) LabSimulatorTime::BinarizeAll();
	}
}

void ALabSimulatorTime::UnBinarizeAll()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"UnBinarizeAll", &error);
		if (error) LabSimulatorTime::UnBinarizeAll();
	}
}



} /* namespace elps */
