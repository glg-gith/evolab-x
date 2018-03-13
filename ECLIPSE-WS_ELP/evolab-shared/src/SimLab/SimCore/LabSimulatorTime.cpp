/*
 * LabSimulatorTime.cpp
 *
 *  Created on: 14 sept. 2012
 *      Author: golgauth
 */

#include "LabSimulatorTime.h"

#include <iostream>

using namespace std;


namespace elps {


LabSimulatorTime::LabSimulatorTime(): LabSimulatorBase() {
	Init();
}

#ifndef NOPYTHON
LabSimulatorTime::LabSimulatorTime(PyObject *obj): LabSimulatorBase(obj) {
	Init();
}
#endif

void LabSimulatorTime::Init() {
	r = new LabRng();
	t = 0.0;
}

LabSimulatorTime::~LabSimulatorTime() {
	// TODO Auto-generated destructor stub
	delete(r);
}


void LabSimulatorTime::ResetTime()
{
	t = 0.0;
	nb_fired_events = 0;

	double t_step = 0.0;
	if (!this->IsCython()) {
		t_step = TimeStep();
	}
	else
	{
#ifndef NOPYTHON
		t_step = CyTimeStep();
#endif
	}

	//t_nextevent = t + TimeStep();
	t_nextevent = t + t_step;
	t_nextout = t + t_outstep;
}

double LabSimulatorTime::TimeStep()
{
	double p1 = r->GetNextDouble();
	if (p1 == 0.0) p1 = 0.0000000001;
	if (tot_evt_rate > 0.0)
	{
//		if (nb_fired_events < 400000) {
//			cout << "dt = " << -log(p1) / tot_evt_rate << endl;
//			cout << "fsum = " << tot_evt_rate << endl;
//		}
		return -log(p1) / tot_evt_rate;
	}
	else
	{
		return t_end;	// Should not happen !!
	}

}

void LabSimulatorTime::UpdateEventsRate(double rate)
{
	tot_evt_rate = rate;
}

// Timed simulation loop behaviour
void LabSimulatorTime::RunSimulation()
{
	if (!this->IsCython()) {
		InitSimulation();
		ResetTime();

		InitUnBinarize();

		BeforeRun();
		do
		{
			BeforeStep();			// 'UpdateEventsRate()' must be called in there !!
			StepSimulation();
			AfterStep();
			StepBinarize();
		}
		while (!EndSimulation());
		OutputSimulation();			// One more time for final state
		AfterRun();
	}
	else
	{
#ifndef NOPYTHON
		CyInitSimulation();
		CyResetTime();

		CyInitUnBinarize();

		CyBeforeRun();
		do
		{
			CyBeforeStep();			// 'UpdateEventsRate()' must be called in there !!
			CyStepSimulation();
			CyAfterStep();
			CyStepBinarize();
		}
		while (!CyEndSimulation());
		CyOutputSimulation();			// One more time for final state
		CyAfterRun();
#endif
	}

	cur_rep++;
}

void LabSimulatorTime::StepSimulation()
{

	// If time for output
	if(t_nextout <= t_nextevent && t_nextout <= t_end)
	{
		t = t_nextout;

		if (!this->IsCython()) {
			OutputSimulation();
		}
		else
		{
#ifndef NOPYTHON
			CyOutputSimulation();
#endif
		}

		t_nextout = t + t_outstep;
	}

	// If time for firing event
	if(t_nextevent <= t_nextout && t_nextout <= t_end)
	{
		t = t_nextevent;

		double t_step = 0.0;
		if (!this->IsCython()) {
			FireEvent();
			t_step = TimeStep();
		}
		else
		{
#ifndef NOPYTHON
			CyFireEvent();
			t_step = CyTimeStep();
#endif
		}

		//t_nextevent = t + TimeStep();
		t_nextevent = t + t_step;
		nb_fired_events++;
	}

	// End reached = force t
	if(t_nextout > t_end) t = t_end;

}

bool LabSimulatorTime::EndSimulation()
{
	return (t >= t_end);
}



void LabSimulatorTime::SetEndTime(double end_time)
{
	t_end = end_time;
}

void LabSimulatorTime::SetOutputTimeStep(double out_time_step)
{
	t_outstep = out_time_step;
}

double LabSimulatorTime::GetTime()
{
	return t;
}

void LabSimulatorTime::SetTime(double time)
{
	t = time;
	t_nextevent = t + TimeStep();
	t_nextout = t + t_outstep;
}

int LabSimulatorTime::GetNbFiredEvents()
{
	return nb_fired_events;
}




//// Cython !!!
#ifndef NOPYTHON

// Additional features...

void LabSimulatorTime::CyResetTime() {
	PyObject *pyobj = this->CyGetPyObj();
	if (pyobj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(pyobj, "ResetTime", error);
		if (error) LabSimulatorTime::ResetTime();
	}
}

double LabSimulatorTime::CyTimeStep() {
	PyObject *pyobj = this->CyGetPyObj();
	if (pyobj) {
		bool error;
		double result = LabCythonUtils::CyCallFuncDouble(pyobj, "TimeStep", error);
		if (error) result = LabSimulatorTime::TimeStep();
		return result;
	}
	return 0.0;
}

void LabSimulatorTime::CyFireEvent() {
	PyObject *pyobj = this->CyGetPyObj();
	if (pyobj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(pyobj, "FireEvent", error);
		if (error) LabSimulatorTime::FireEvent();
	}
}

// Features from Base...

void LabSimulatorTime::CyInitSimulation()
{
//	if (this->m_obj) {
//		int error;
//		cy_call_func_void(this->m_obj, (char*)"InitSimulation", &error);
//		if (error) LabSimulatorBase::InitSimulation();
//	}
	PyObject *pyobj = this->CyGetPyObj();
	if (pyobj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(pyobj, "InitSimulation", error);
		if (error) LabSimulatorTime::InitSimulation();
	}
}

void LabSimulatorTime::CyStepSimulation()
{
	PyObject *pyobj = this->CyGetPyObj();
	if (pyobj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(pyobj, "StepSimulation", error);
		if (error) LabSimulatorTime::StepSimulation();
	}
}

void LabSimulatorTime::CyBeforeStep()
{
	PyObject *pyobj = this->CyGetPyObj();
	if (pyobj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(pyobj, "BeforeStep", error);
		if (error) LabSimulatorTime::BeforeStep();
	}
}

void LabSimulatorTime::CyAfterStep()
{
	PyObject *pyobj = this->CyGetPyObj();
	if (pyobj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(pyobj, "AfterStep", error);
		if (error) LabSimulatorTime::AfterStep();
	}
}

void LabSimulatorTime::CyOutputSimulation()
{
	PyObject *pyobj = this->CyGetPyObj();
	if (pyobj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(pyobj, "OutputSimulation", error);
		if (error) LabSimulatorTime::OutputSimulation();
	}
}

bool LabSimulatorTime::CyEndSimulation()
{
	PyObject *pyobj = this->CyGetPyObj();
	if (pyobj) {
		bool error;
		bool result = LabCythonUtils::CyCallFuncBool(pyobj, "EndSimulation", error);
		if (error) result = LabSimulatorTime::EndSimulation();
		return result;
	}
	return true;
}

void LabSimulatorTime::CyInitUnBinarize()
{
	PyObject *pyobj = this->CyGetPyObj();
	if (pyobj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(pyobj, "InitUnBinarize", error);
		if (error) LabSimulatorTime::InitUnBinarize();
	}
}

void LabSimulatorTime::CyStepBinarize()
{
	PyObject *pyobj = this->CyGetPyObj();
	if (pyobj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(pyobj, "StepBinarize", error);
		if (error) LabSimulatorTime::StepBinarize();
	}
}

void LabSimulatorTime::CyRunSimulation()
{
	PyObject *pyobj = this->CyGetPyObj();
	if (pyobj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(pyobj, "RunSimulation", error);
		if (error) LabSimulatorTime::RunSimulation();
	}
}

void LabSimulatorTime::CyBeforeRun()
{
	PyObject *pyobj = this->CyGetPyObj();
	if (pyobj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(pyobj, "BeforeRun", error);
		if (error) LabSimulatorTime::BeforeRun();
	}
}

void LabSimulatorTime::CyAfterRun()
{
	PyObject *pyobj = this->CyGetPyObj();
	if (pyobj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(pyobj, "AfterRun", error);
		if (error) LabSimulatorTime::AfterRun();
	}
}
//
//void LabSimulatorBase::Run()
//{
//	if (this->m_obj) {
//		int error;
//		cout << "Runnin'..." << endl;
//		cy_call_func_void(this->m_obj, (char*)"Run", &error);
//		if (error) LabSimulatorBase::Run();
//	}
//}

void LabSimulatorTime::CyBinarizeAll()
{
	PyObject *pyobj = this->CyGetPyObj();
	if (pyobj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(pyobj, "BinarizeAll", error);
		if (error) LabSimulatorTime::BinarizeAll();
	}
}

void LabSimulatorTime::CyUnBinarizeAll()
{
	PyObject *pyobj = this->CyGetPyObj();
	if (pyobj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(pyobj, "UnBinarizeAll", error);
		if (error) LabSimulatorTime::UnBinarizeAll();
	}
}

#endif


}  // namespace elps


