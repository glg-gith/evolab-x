/*
 * LabSiteEvent.cpp
 *
 *  Created on: 18 sept. 2012
 *      Author: golgauth
 */

#include "LabSiteEvent.h"

namespace elps {

//LabSiteEvent::LabSiteEvent() : LabSiteBase() {
//	// TODO Auto-generated constructor stub
//	//CreateStateStack();
//	CreateEventStack();
//}

LabSiteEvent::LabSiteEvent(LabSimulatorBase * sim) : LabSiteBase(sim) {
	Init();
}

LabSiteEvent::LabSiteEvent(LabSimulatorBase * sim, LabNode *parent_node) : LabSiteBase(sim, parent_node) {
	Init();
}

//// Cython !!!
#ifndef NOPYTHON
LabSiteEvent::LabSiteEvent(PyObject *obj, LabSimulatorBase *sim): LabSiteEvent(sim) {
	cout << "Create LabSiteEvent" << obj << endl;
	this->CySetPyObj(obj);
}

LabSiteEvent::LabSiteEvent(PyObject *obj, LabSimulatorBase *sim, LabNode *parent_node): LabSiteEvent(sim, parent_node) {
	cout << "Create LabSiteEvent" << obj << endl;
	this->CySetPyObj(obj);
}
#endif


// PROTECTED !!
LabSiteEvent::LabSiteEvent(LabSimulatorBase * sim, LabNode *parent_node, bool set_trackable) : LabSiteBase(sim, parent_node, set_trackable) {
	Init();
}

void LabSiteEvent::Init()
{
	eventStack = new LabEventStack();

#ifndef NOPYTHON
	this->CySetPyObj(NULL);
#endif
}

//LabSiteEvent::~LabSiteEvent() {
//	// TODO Auto-generated destructor stub
//}

LabEventStack *LabSiteEvent::GetEventStack()
{
	return eventStack;
}



void LabSiteEvent::PushEvent(string name, double initial_rate,
		EventCallBackParam param,
		RateCallBack *rate_callback,
		RealizeCallBack *realize_callback)
{
	eventStack->PushEvent(name, initial_rate, param, rate_callback, realize_callback);
}

void LabSiteEvent::PopEvent()
{
	eventStack->PopEvent();
}

int LabSiteEvent::GetNbEvents() {
	return eventStack->GetSize();
}


double LabSiteEvent::GetEventStackRate()
{
	return eventStack->GetStackRate();
}

void LabSiteEvent::ResetEventStackRate()
{
	eventStack->ResetStackRate();
}

void LabSiteEvent::SetTimeEvent(double time)
{
	eventStack->SetTime(time);
}

double LabSiteEvent::GetTimeEvent()
{
	return eventStack->GetTime();
}

void LabSiteEvent::UpdateEventStackRates()
{
	eventStack->UpdateStackRates();
}

void LabSiteEvent::RealizeEvent(int id, double time)
{
	eventStack->RealizeEvent(id, time);
}

void LabSiteEvent::RealizeEvent(double p1, double time)
{
	eventStack->RealizeEvent(p1, time);
}

void LabSiteEvent::ClearEvents()
{
	eventStack->Clear();
}



LabUserData *LabSiteEvent::New(LabNode *parent_node) {
	return new LabSiteEvent(simulator, parent_node, true);
}

void LabSiteEvent::Copy(LabUserData *src_site, int dep_filter)
{
	// Events

	if (!parent) LabConsts::E(LabConsts::SITE_EVENT_PARENT_NULL);

	// Basic copy
	//cout << "Plantage here 1.1" << endl;
	//cout << "NB events 4 = " << ((LabSiteEvent *)src_site)->GetStateStack() << endl;
	eventStack->Copy(((LabSiteEvent *)src_site)->GetEventStack());
	//cout << "Plantage here 1.2" << endl;
	// Deep copy (for CallBack stuffs)
	bool is_cython_call;
	vector<LabEventStack::t_event>::iterator it;
	for (it = eventStack->GetEvents().begin(); it != eventStack->GetEvents().end(); it++) {
		// Renew CallBack stuffs
		// TODO : delete the previously allocated callbacks !!
		is_cython_call = (*it).rate_callback->IsCythonCall();
		if (is_cython_call)
		{
			// (*it).param = parent; => Cannot be done here cause we need a PyLabNode, so done from cython side (See SetCallBackParamAt())
			(*it).rate_callback = new RateCallBack((*it).rate_callback->GetMethod(), (*it).rate_callback->GetUserData());
			(*it).realize_callback = new RealizeCallBack((*it).realize_callback->GetMethod(), (*it).realize_callback->GetUserData());
		}
		else
		{
			(*it).param = parent;
			//cout << "Parent is = " << parent << endl;
			(*it).rate_callback = new RateCallBack((*it).rate_callback->GetMethod2());
			(*it).realize_callback = new RealizeCallBack((*it).realize_callback->GetMethod2());
		}
	}

	// Attributes & States
	//cout << "Plantage here 1.3" << endl;

	LabSiteBase::Copy((LabSiteBase *)src_site, dep_filter);
	//cout << "Plantage here 1.4" << endl;
}


void LabSiteEvent::Binarize(ofstream *ostream)
{
	eventStack->Binarize(ostream);
	LabSiteBase::Binarize(ostream);			// Binarize 'attributesStack'
}

void LabSiteEvent::UnBinarize(ifstream *istream)
{
	eventStack->UnBinarize(istream);
	LabSiteBase::UnBinarize(istream);		// UnBinarize 'attributesStack'
}

void LabSiteEvent::SetCallBackParamAt(int i, void *p) {
	eventStack->GetEvents()[i].param = p;
}



} /* namespace elps */


