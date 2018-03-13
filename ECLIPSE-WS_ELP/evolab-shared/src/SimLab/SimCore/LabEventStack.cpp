/*
 * LabEvent.cpp
 *
 *  Created on: 13 sept. 2012
 *      Author: golgauth
 */

#include "LabEventStack.h"

namespace elps {


LabEventStack::LabEventStack() {
	// TODO Auto-generated constructor stub
	nb_fired_evts = 0;
	double delta_t = 0.0;
	double total_rate = 0.0;
}

LabEventStack::~LabEventStack() {
	// TODO Auto-generated destructor stub
}

void LabEventStack::PushEvent(string name, double initial_rate,
		EventCallBackParam param,
		RateCallBack *rate_callback,
		RealizeCallBack *realize_callback)
{
	t_event evt;
	evt.name = name;
	evt.data.rate = initial_rate;
	evt.param = param;
	evt.rate_callback = rate_callback;
	evt.realize_callback = realize_callback;
	events.push_back(evt);
}

void LabEventStack::PopEvent()
{
	events.pop_back();
}

void LabEventStack::Clear()
{
	// TODO : free allocated callbacks...
	events.clear();
}

int LabEventStack::GetSize() {
	return events.size();
}

string LabEventStack::GetStats() {
	stringstream ss;

	double ter = 0.0;

	for (vector<t_event>::iterator it=events.begin(); it!=events.end(); it++)
	{
		ter += (*it).data.rate;
	}

	ss << "evts : " << nb_fired_evts << " | dt : " << delta_t << " | ter : " << stackRate;
	return ss.str();
}

double LabEventStack::GetEventRate(int id)
{
	return events[id].data.rate;
}

void LabEventStack::SetEventRate(int id, double rate)
{
	events[id].data.rate = rate;
}


string LabEventStack::GetEventName(int id)
{
	return events[id].name;
}

vector<LabEventStack::t_event>& LabEventStack::GetEvents()
{
	return events;
}


double LabEventStack::GetStackRate()
{
	return stackRate;
}

//void LabEventStack::UpdateStackRate()
//{
//	stackRate = 0.0;
//	for (vector<t_event>::iterator it=events.begin(); it!=events.end(); it++)
//	{
//		stackRate += (*it).rate;
//	}
//}

/*
 * Update the rate for each event, as well as the global 'stackRate'
 */
void LabEventStack::UpdateStackRates()
{
	//cout << "Update stack rates = " << endl;
	stackRate = 0.0;
	for (vector<t_event>::iterator it=events.begin(); it!=events.end(); it++)
	{
		//cout << "Update stack rates = " << (*it).param << endl;
		//cout << "Try exec evt = " << (*it).name << endl;
		if ((*it).rate_callback->IsCythonCall()) {
			//cout << "Try exec Cython ! >> " << (*it).name << " | " << (*it).rate_callback << endl;
			(*it).data.rate = (*it).rate_callback->cy_execute((*it).param);
		} else {
			//cout << "Try exec C++ !" << endl;
			(*it).data.rate = (*it).rate_callback->execute((*it).param);
		}
		//cout << "Try exec OK !" << endl;
		stackRate += (*it).data.rate;
	}
	//cout << "Update stack rates Done ! " << endl;
}


void LabEventStack::RealizeEvent(int id, double time)
{
	delta_t = time - GetTime();
	SetTime(time);
	if (events[id].realize_callback->IsCythonCall())
	{
		cout << "Try exec cy_execute !!!" << endl;
		events[id].realize_callback->cy_execute(events[id].param);
	}
	else
	{
		cout << "Try exec normal execute !!!" << endl;
		events[id].realize_callback->execute(events[id].param);
	}

	nb_fired_evts++;
}

void LabEventStack::RealizeEvent(double p1, double time)
{
	delta_t = time - GetTime();
	SetTime(time);

	double add = 0.0;
	p1 *= stackRate;

	//int n = 0;

	for (vector<t_event>::iterator it=events.begin(); it!=events.end(); it++)
	{
		add += (*it).data.rate;
		if(p1 < add)
		{
			//SetTime(time);
			if ((*it).realize_callback->IsCythonCall()) {
				(*it).realize_callback->cy_execute((*it).param);}
			else {
				(*it).realize_callback->execute((*it).param);}
			//cout << "Chosen event in stack = " << n << endl;
			break;
		}
		//n++;
	}

	nb_fired_evts++;
}

void LabEventStack::RealizeEvent(gsl_rng *r, double time)
{
	RealizeEvent(gsl_rng_uniform(r), time);
}


void LabEventStack::Copy(LabEventStack *src_stack)
{
	//cout << "Plantage here 1.1.1" << endl;
	//cout << "NULL src_stack !!" << endl;
	stackRate = src_stack->GetStackRate();
	//cout << "Plantage here 1.1.2" << endl;
	t = src_stack->GetTime();
	//cout << "Plantage here 1.1.3" << endl;
	vector<t_event> evts = src_stack->GetEvents();
	events = evts; //src_stack->GetEvents();
	//cout << "Plantage here 1.1.4" << endl;
}


void LabEventStack::ResetStackRate()
{
	stackRate = 0.0;
}

void LabEventStack::SetTime(double time)
{
	t = time;
}

double LabEventStack::GetTime()
{
	return t;
}


// Implement virtual stuffs from 'LabIBinarizable'
void LabEventStack::Binarize(ofstream *ostream)
{
	LabBinaryUtils::WriteValueToStream(stackRate, ostream);
	LabBinaryUtils::WriteValueToStream(t, ostream);

	// Save the 'data' part of the events
	vector<t_event_data> data;
	vector<t_event>::iterator it;
	for (it=events.begin(); it!=events.end(); it++)
		data.push_back((*it).data);
	LabBinaryUtils::WriteVectorToStream(data, ostream);
}

void LabEventStack::UnBinarize(ifstream *istream)
{
	LabBinaryUtils::ReadValueFromStream(stackRate, istream);
	LabBinaryUtils::ReadValueFromStream(t, istream);

	// Restore the 'data' part of the events
	vector<t_event_data> data;
	LabBinaryUtils::ReadVectorFromStream(data, istream);
	vector<t_event>::iterator it = events.begin();
	vector<t_event_data>::iterator it2 = data.begin();
	while (it != events.end())
	{
		(*it).data = (*it2);
		it++; it2++;
	}
}


}  // namespace elps



