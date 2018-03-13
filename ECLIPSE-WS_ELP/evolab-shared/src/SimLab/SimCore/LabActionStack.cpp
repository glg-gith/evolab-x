/*
 * LabActionStack.cpp
 *
 *  Created on: 17 sept. 2012
 *      Author: golgauth
 */

#include <LabActionStack.h>


namespace elps {


LabActionStack::LabActionStack() {
	// TODO Auto-generated constructor stub
	t = 0.0;
}

LabActionStack::~LabActionStack() {
	// TODO Auto-generated destructor stub
}

void LabActionStack::PushTimeoutAction(string name,
		ActionCallBackParam param,
		ActionCallBack *action_callback,
		double start_time)
{
	t_action act;
	act.name = name;
	act.param = param;
	act.action_callback = action_callback;
	act.data.status = WAITING;
	act.data.t_start = start_time;
	act.data.interval = -1;
	actions.push_back(act);
}

void LabActionStack::PushIntervalAction(string name,
		ActionCallBackParam param,
		ActionCallBack *action_callback,
		double start_time,
		double timeout,
		double interval)
{
	t_action act;
	act.name = name;
	act.param = param;
	act.action_callback = action_callback;
	act.data.status = WAITING;
	act.data.t_start = start_time;
	act.data.timeout = timeout;
	act.data.interval = interval;
	act.data.t_last_fired = -1;
	actions.push_back(act);
}

void LabActionStack::PopAction()
{
	actions.pop_back();
}

int LabActionStack::GetSize() {
	return actions.size();
}


string LabActionStack::GetActionName(int id)
{
	return actions[id].name;
}

double LabActionStack::GetTime()
{
	return t;
}

void LabActionStack::SetTime(double time)
{
	t = time;
}

vector<LabActionStack::t_action>& LabActionStack::GetActions()
{
	return actions;
}


void LabActionStack::FireAction(double current_time, t_action *ptr_act, bool& success)
{
	success = false;
	if (ptr_act->data.status != TERMINATED)
	{
		if (current_time >= ptr_act->data.t_start)
		{
			if (ptr_act->data.interval <= 0)								// "Timeout - play once" case
			{
				if (ptr_act->action_callback->IsCythonCall())
					ptr_act->action_callback->cy_execute(ptr_act->param);
				else
					ptr_act->action_callback->execute(ptr_act->param);
				success = true;
				ptr_act->data.status = TERMINATED;
			}
			else													// "Repeat every 'interval'" case
			{
				if (ptr_act->data.status == WAITING)
					ptr_act->data.status = RUNNING;

				if (ptr_act->data.timeout >= 0 && current_time >= ptr_act->data.timeout)
				{
					ptr_act->data.status = TERMINATED;
				}
				else if (ptr_act->data.t_last_fired < 0 ||
						(current_time >= ptr_act->data.t_last_fired + ptr_act->data.interval))
				{
					if (ptr_act->action_callback->IsCythonCall())
						ptr_act->action_callback->cy_execute(ptr_act->param);
					else
						ptr_act->action_callback->execute(ptr_act->param);
					success = true;
					ptr_act->data.t_last_fired = current_time;
				}
			}
		}
	}
}

bool LabActionStack::TryFire(double current_time)
{
	bool at_least_one_success = false, success;

	// Update the current time
	this->SetTime(current_time);

	for (vector<t_action>::iterator it=actions.begin(); it!=actions.end(); it++)
	{
		FireAction(current_time, &(*it), success);
		if (success) at_least_one_success = true;
	}
	return at_least_one_success;
}

bool LabActionStack::TryFireAction(double current_time, int id)
{
	bool success;

	// Update the current time
	this->SetTime(current_time);

	FireAction(current_time, &(actions[id]), success);
	return success;
}

void LabActionStack::FireNow()
{
	for (vector<t_action>::iterator it=actions.begin(); it!=actions.end(); it++)
	{
		if ((*it).action_callback->IsCythonCall())
			(*it).action_callback->cy_execute((*it).param);
		else
			(*it).action_callback->execute((*it).param);
	}
}

void LabActionStack::FireActionNow(int id)
{
	if (actions[id].action_callback->IsCythonCall())
		actions[id].action_callback->cy_execute(actions[id].param);
	else
		actions[id].action_callback->execute(actions[id].param);
}


void LabActionStack::Copy(LabActionStack *src_stack)
{
	t = src_stack->GetTime();
	actions = src_stack->GetActions();
}


// Implement virtual stuffs from 'LabIBinarizable'
void LabActionStack::Binarize(ofstream *ostream)
{
	LabBinaryUtils::WriteValueToStream(t, ostream);

	// Save the 'data' part of the actions
	vector<t_action_data> data;
	vector<t_action>::iterator it;
	for (it=actions.begin(); it!=actions.end(); it++)
		data.push_back((*it).data);
	LabBinaryUtils::WriteVectorToStream(data, ostream);
}

void LabActionStack::UnBinarize(ifstream *istream)
{
	LabBinaryUtils::ReadValueFromStream(t, istream);

	// Restore the 'data' part of the actions
	vector<t_action_data> data;
	LabBinaryUtils::ReadVectorFromStream(data, istream);
	vector<t_action>::iterator it = actions.begin();
	vector<t_action_data>::iterator it2 = data.begin();
	while (it != actions.end())
	{
		(*it).data = (*it2);
		it++; it2++;
	}
	LabBinaryUtils::ReadVectorFromStream(actions, istream);
}


}  // namespace elp



