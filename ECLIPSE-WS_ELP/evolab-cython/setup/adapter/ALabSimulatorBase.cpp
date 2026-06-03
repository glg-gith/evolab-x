/*
 * ILabSimulatorTime.cpp
 *
 *  Created on: 4 oct. 2012
 *      Author: golgauth
 */

#include "ALabSimulatorBase.h"

namespace elps {

ALabSimulatorBase::ALabSimulatorBase(PyObject *obj) {
	this->m_obj = obj;
	// Provided by "cyelp_api.h"
	if (import_cyelp()) {
	} else {
		Py_XINCREF(this->m_obj);
	}
}

ALabSimulatorBase::~ALabSimulatorBase() {
	Py_XDECREF(this->m_obj);
}


void ALabSimulatorBase::InitSimulation()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"InitSimulation", &error);
		if (error) LabSimulatorBase::InitSimulation();
	}
}

void ALabSimulatorBase::StepSimulation()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"StepSimulation", &error);
		if (error) LabSimulatorBase::StepSimulation();
	}
}

void ALabSimulatorBase::BeforeStep()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"BeforeStep", &error);
		if (error) LabSimulatorBase::BeforeStep();
	}
}

void ALabSimulatorBase::AfterStep()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"AfterStep", &error);
		if (error) LabSimulatorBase::AfterStep();
	}
}

void ALabSimulatorBase::OutputSimulation()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"OutputSimulation", &error);
		if (error) LabSimulatorBase::OutputSimulation();
	}
}

bool ALabSimulatorBase::EndSimulation()
{
	if (this->m_obj) {
		int error;
		bool result = cy_call_func_bool(this->m_obj, (char*)"EndSimulation", &error);
		if (error) result = LabSimulatorBase::EndSimulation();
		return result;
	}
	return true;
}

void ALabSimulatorBase::InitUnBinarize()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"InitUnBinarize", &error);
		if (error) LabSimulatorBase::InitUnBinarize();
	}
}

void ALabSimulatorBase::StepBinarize()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"StepBinarize", &error);
		if (error) LabSimulatorBase::StepBinarize();
	}
}

void ALabSimulatorBase::RunSimulation()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"RunSimulation", &error);
		if (error) LabSimulatorBase::RunSimulation();
	}
}

void ALabSimulatorBase::BeforeRun()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"BeforeRun", &error);
		if (error) LabSimulatorBase::BeforeRun();
	}
}

void ALabSimulatorBase::AfterRun()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"AfterRun", &error);
		if (error) LabSimulatorBase::AfterRun();
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

void ALabSimulatorBase::BinarizeAll()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"BinarizeAll", &error);
		if (error) LabSimulatorBase::BinarizeAll();
	}
}

void ALabSimulatorBase::UnBinarizeAll()
{
	if (this->m_obj) {
		int error;
		cy_call_func_void(this->m_obj, (char*)"UnBinarizeAll", &error);
		if (error) LabSimulatorBase::UnBinarizeAll();
	}
}

} /* namespace elps */
