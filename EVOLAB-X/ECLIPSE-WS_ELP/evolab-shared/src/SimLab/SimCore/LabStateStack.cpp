/*
 * LabStateStack.cpp
 *
 *  Created on: 13 sept. 2012
 *      Author: golgauth
 */

#include "LabStateStack.h"


namespace elps {


LabStateStack::LabStateStack() {
	// TODO Auto-generated constructor stub
	curState = -1;
	curStateTime = 0.0;
}

LabStateStack::~LabStateStack() {
	// TODO Auto-generated destructor stub
}

void LabStateStack::PushState(string name, /*bool indexed,*/ int next_id, int timeout_id, double timeout)
{
	t_state st;
	st.name = name;
	//st.data.indexed = indexed;
	st.data.timeout = timeout;
	st.data.next_id = next_id;
	st.data.timeout_id = timeout_id;
	states.push_back(st);
}

LabStateStack::t_state& LabStateStack::PopState()
{
	t_state st = states.at(states.size()-1);
	states.pop_back();
	return st;
}

int LabStateStack::GetSize(){
	return states.size();
}

int LabStateStack::GetCurrentState() {
	return curState;
}

void LabStateStack::SetCurrentState(int id, double time)
{
	if (id >= 0 && id < states.size())
	{
		curStateTime = time;
		curState = id;
	}
}

double LabStateStack::GetCurrentStateTime() {
	return curStateTime;
}

double LabStateStack::GetCurrentStateElapsedTime(double time) {
	return time - curStateTime;
}

double LabStateStack::GetCurrentStateRemainingTime(double time) {
	return curStateTime + states[curState].data.timeout - time;
}


string LabStateStack::GetStateName(int id)
{
	return states[id].name;
}

bool LabStateStack::IsValidStateId(int id) {
	return (id >= 0 && id < (int)states.size());
}

vector<LabStateStack::t_state>& LabStateStack::GetStates()
{
	return states;
}

//double GetStatesSequence(int id);
void LabStateStack::GotoNextState(double time)
{
	curStateTime = time;
	curState = states[curState].data.next_id;
}

void LabStateStack::TryDoTimeout(double time)
{
	if (states[curState].data.timeout >= 0 &&
			time > curStateTime + states[curState].data.timeout)
	{
		curStateTime = time;
		curState = states[curState].data.timeout_id;
	}
}

void LabStateStack::Copy(LabStateStack *src_stack, bool reset_time, double time)
{
	if (reset_time) curStateTime = time;
	else curStateTime = src_stack->GetCurrentStateTime();

	curState = src_stack->GetCurrentState();
	states = src_stack->GetStates();
}

// Implement virtual stuffs from 'LabIBinarizable'
void LabStateStack::Binarize(ofstream *ostream)
{
	LabBinaryUtils::WriteValueToStream(curStateTime, ostream);
	LabBinaryUtils::WriteValueToStream(curState, ostream);

	// Save the 'data' part of the attributes
	vector<t_state_data> data;
	vector<t_state>::iterator it;

	for (it=states.begin(); it!=states.end(); it++)
		data.push_back((*it).data);
	LabBinaryUtils::WriteVectorToStream(data, ostream);
}

void LabStateStack::UnBinarize(ifstream *istream)
{
	LabBinaryUtils::ReadValueFromStream(curStateTime, istream);
	LabBinaryUtils::ReadValueFromStream(curState, istream);

	// Restore the 'data' part of the attributes
	vector<t_state_data> data;
	vector<t_state>::iterator it;
	vector<t_state_data>::iterator it2;

	LabBinaryUtils::ReadVectorFromStream(data, istream);
	it = states.begin(); it2 = data.begin();
	while (it != states.end()) {
		(*it).data = (*it2); it++; it2++;
	}
}


}  // namespace elps




