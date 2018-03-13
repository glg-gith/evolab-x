/*
 * LabSiteBase.cpp
 *
 *  Created on: 18 sept. 2012
 *      Author: golgauth
 */

#include "LabSiteBase.h"

namespace elps {


LabSiteBase::LabSiteBase(LabSimulatorBase *sim) {
	Init(sim, NULL, false);
}

LabSiteBase::LabSiteBase(LabSimulatorBase *sim, LabNode *parent_node) {
	Init(sim, parent_node, false);
}

//// Cython !!!
#ifndef NOPYTHON
LabSiteBase::LabSiteBase(PyObject *obj, LabSimulatorBase *sim): LabSiteBase(sim) {
	cout << "Create LabSiteBase" << obj << endl;
	this->CySetPyObj(obj);
}

LabSiteBase::LabSiteBase(PyObject *obj, LabSimulatorBase *sim, LabNode *parent_node): LabSiteBase(sim, parent_node) {
	cout << "Create LabSiteBase" << obj << endl;
	this->CySetPyObj(obj);
}
#endif


// PROTECTED !!
LabSiteBase::LabSiteBase(LabSimulatorBase *sim, LabNode *parent_node, bool set_trackable) {
	Init(sim, parent_node, set_trackable);
}

void LabSiteBase::Init(LabSimulatorBase *sim, LabNode *parent_node, bool set_trackable) {
	simulator = sim;
	parent = parent_node;



	indAttributesStack = new LabAttributesStack();
	siteAttributesStack = new LabAttributesStack();

	CreateStateStack();

	setsManager = SiteSetsMgr::GetInstance();

	//	// Done by the Array/Net-Binding now !!!
	//	this->id = SetsManager::GetInstance()->GetIndividualsCount();
	//	SetsManager::GetInstance()->AddSite(this);
	setTrackable = set_trackable;

#ifndef NOPYTHON
	this->CySetPyObj(NULL);
#endif
}


LabSiteBase::~LabSiteBase() {
	// TODO : Implement the possibility of unsubscrtibing a site from LabSiteSetsManager
	//		  instead of this shity stuff which terminates the program !!
	//	cerr << "Exception : Deleting a site is currently forbidden." << endl;
	//	throw "Deleting a site is currently forbidden.";
}

//LabAttributesStack *LabSiteBase::GetAttributesStack()
//{
//	return attributesStack;
//}

LabAttributesStack *LabSiteBase::GetIndAttributesStack()
{
	return indAttributesStack;
}

LabAttributesStack *LabSiteBase::GetSiteAttributesStack()
{
	return siteAttributesStack;
}

void LabSiteBase::PushAttribute(
		string name,
		t_deptype dep_type,
		///int subset_id,
		double d_value,
		//int i_value//,
		//void *o_value = NULL
		bool indexed,
		int nb_beans
)
{

	int index = round(d_value);
	bool valid_index = false;

	if (dep_type == IND_DEP) {
		indAttributesStack->PushAttribute(name, d_value, indexed, nb_beans);//, i_value);
		valid_index = indAttributesStack->IsValidIndexedAttrValue(indAttributesStack->GetSize() - 1, index);
	}
	else {
		siteAttributesStack->PushAttribute(name, d_value, indexed, nb_beans);//, i_value);
		valid_index = siteAttributesStack->IsValidIndexedAttrValue(siteAttributesStack->GetSize() - 1, index);
	}

	// Sets
	if (indexed && valid_index)
	{
		// Create additional sets
		// First of the serie already existing : do nothing at all
		string first_set = name + "0";
		if (!(setsManager->SetExists(first_set)))
		{
			stringstream ss;
			for (int i=0; i<nb_beans; i++)
			{
				ss.str("");
				ss.clear(); // Clear state flags.
				ss << name << i;
				setsManager->CreateSet(simulator->GetMaxPopulationSize(), ss.str());
			}
		}

		// Subscribe to the right one (SetAttrDouble() can handle this...)
		if (setTrackable) this->SetAttrDouble(dep_type, indAttributesStack->GetSize() - 1, index);
	}
}

void LabSiteBase::PushAttribute(
		string name,
		int dep_type,
		double d_value,
		//int i_value
		bool indexed,
		int nb_beans
)
{
	this->PushAttribute(name, (t_deptype)dep_type, d_value, indexed, nb_beans);//, i_value);
}

void LabSiteBase::PopAttribute(t_deptype dep_type)
{
	LabAttributesStack::t_attr *attr;
	int index;
	bool valid_index = false;
	if (dep_type == IND_DEP) {
		attr = indAttributesStack->GetAttribute(indAttributesStack->GetSize() - 1);
		index = round(attr->data.d_value);
		valid_index = indAttributesStack->IsValidIndexedAttrValue(indAttributesStack->GetSize() - 1, index);
		indAttributesStack->PopAttribute(/*dep_type*/);
	}
	else {
		attr = siteAttributesStack->GetAttribute(siteAttributesStack->GetSize() - 1);
		index = round(attr->data.d_value);
		valid_index = siteAttributesStack->IsValidIndexedAttrValue(siteAttributesStack->GetSize() - 1, index);
		siteAttributesStack->PopAttribute(/*dep_type*/);
	}

	// Sets
	stringstream ss;
	ss << attr->name << index;
	if (attr->data.indexed && valid_index) { this->UnSubscribeSet(ss.str()); }
}
void LabSiteBase::PopAttribute(int dep_type)
{
	this->PopAttribute((t_deptype)dep_type);
}

//vector<LabAttributesStack::t_attr> LabSiteBase::GetAttributes()
//{
//	return this->GetAttributesStack()->GetAttributes();
//}

vector<LabAttributesStack::t_attr>& LabSiteBase::GetIndAttributes()
{
	return indAttributesStack->GetAttributes();
}

vector<LabAttributesStack::t_attr>& LabSiteBase::GetSiteAttributes()
{
	return siteAttributesStack->GetAttributes();
}

//vector<LabAttributesStack::t_attr*> LabSiteBase::GetEnvAttributes()
//{
//	return this->GetAttributesStack()->GetEnvAttributes();
//}

int LabSiteBase::GetNbAttributes(int dep_filter) {
	switch (dep_filter) {
	case IND_DEP:
		return indAttributesStack->GetSize();
		break;
	case SITE_DEP:
		return siteAttributesStack->GetSize();
		break;
	case (IND_DEP | SITE_DEP):
						return indAttributesStack->GetSize() + siteAttributesStack->GetSize();
	break;
	}
	//return attributesStack->GetSize(dep_type);
}

//int LabSiteBase::GetNbAttributes(int dep_type) {
//	return attributesStack->GetSize(dep_type);
//}


int LabSiteBase::GetAttrInt(t_deptype dep_filter, int id)
{
	return RND(GetAttrDouble(dep_filter, id));
}
int LabSiteBase::GetAttrInt(int dep_filter, int id)
{
	return this->GetAttrInt((t_deptype)dep_filter, id);
}

double LabSiteBase::GetAttrDouble(t_deptype dep_type, int id)
{
	if (dep_type == IND_DEP)
		return indAttributesStack->GetAttribute(/*dep_filter,*/ id)->data.d_value;
	else
		return siteAttributesStack->GetAttribute(/*dep_filter,*/ id)->data.d_value;
}
double LabSiteBase::GetAttrDouble(int dep_type, int id)
{
	return this->GetAttrDouble((t_deptype)dep_type, id);
}
//
//void *LabSiteBase::GetAttrObject(LabAttributesStack::t_deptype dep_filter, int id)
//{
//	return attributesStack->GetAttribute(dep_filter, id)->data.o_value;
//}

/*void LabSiteBase::SetAttrInt(LabAttributesStack::t_deptype dep_filter, int id, int value)
{
	attributesStack->GetAttribute(dep_filter, id)->data.i_value = value;
}*/
/*void LabSiteBase::SetAttrInt(int dep_filter, int id, int value)
{
	this->SetAttrInt((LabAttributesStack::t_deptype)dep_filter, id, value);
}*/

void LabSiteBase::SetAttrDouble(t_deptype dep_type, int id, double value)
{

	LabAttributesStack::t_attr *attr;

	int index = RND(value);
	bool valid_index;

	//double prev_val;
	int prev_index;
	bool valid_prev_index;

	if (dep_type == IND_DEP)
	{
		attr = indAttributesStack->GetAttribute(/*dep_filter,*/ id);
		prev_index = RND(attr->data.d_value);
		valid_prev_index = indAttributesStack->IsValidIndexedAttrValue(id, prev_index);
		attr->data.d_value = value;
		valid_index = indAttributesStack->IsValidIndexedAttrValue(id, index);
	}
	else
	{
		attr = siteAttributesStack->GetAttribute(/*dep_filter,*/ id);
		prev_index = RND(attr->data.d_value);
		valid_prev_index = siteAttributesStack->IsValidIndexedAttrValue(id, prev_index);
		attr->data.d_value = value;
		valid_index = siteAttributesStack->IsValidIndexedAttrValue(id, index);
	}


	// Sets
	if (setTrackable && attr->data.indexed && prev_index != index)
	{
		//cout << "#2 INSERT : " << this << endl;

		stringstream ss1, ss2;
		if (valid_prev_index) {
			ss1 << attr->name << prev_index;
			this->UnSubscribeSet(ss1.str());
		}
		if (valid_index) {
			ss2 << attr->name << index;
			this->SubscribeSet(ss2.str());
		}
	}
}
void LabSiteBase::SetAttrDouble(int dep_type, int id, double value)
{
	this->SetAttrDouble((t_deptype)dep_type, id, value);
}
//
//void LabSiteBase::SetAttrObject(LabAttributesStack::t_deptype dep_filter, int id, void *value)
//{
//	attributesStack->GetAttribute(dep_filter, id)->data.o_value = value;
//}


LabStateStack *LabSiteBase::GetStateStack()
{
	return stateStack;
}

void LabSiteBase::CreateStateStack()
{
	stateStack = new LabStateStack();
}

int LabSiteBase::GetState()
{
	return stateStack->GetCurrentState();
}

void LabSiteBase::SetState(int state, double time)
{
	int prev_id = stateStack->GetCurrentState();
	int prev_valid = (stateStack->IsValidStateId(prev_id));


	LabStateStack::t_state old_st;
	if (prev_valid) old_st = stateStack->GetStates()[stateStack->GetCurrentState()];

	if (stateStack->GetCurrentState() != state)
	{

		stateStack->SetCurrentState(state, time);

		LabStateStack::t_state new_st = stateStack->GetStates()[stateStack->GetCurrentState()];

		// Sets
		if (setTrackable)
		{
			//if (prev_valid && old_st.data.indexed) {
				//cout << "#3 INSERT : " << this << endl;
				this->UnSubscribeSet(old_st.name);
			//}
			//if (stateStack->IsValidStateId(state) && new_st.data.indexed) {
				//cout << "#4 INSERT : " << this << endl;
				this->SubscribeSet(new_st.name);
			//}
		}
	}
}

void LabSiteBase::PushState(string name, /*bool indexed,*/ int next_id, int timeout_id, double timeout)
{
	stateStack->PushState(name, /*indexed,*/ next_id, timeout_id, timeout);
	//	// Sets
	//	if (stateStack->GetStates()[stateStack->GetSize()-1].data.indexed) {
	//		//cout << "#1 INSERT : " << this << endl;
	//		this->SubscribeSet(name);
	//	}
	setsManager->CreateSet(simulator->GetMaxPopulationSize(), name);
}

void LabSiteBase::PopState()
{
	LabStateStack::t_state st = stateStack->PopState();

	// Sets
	this->UnSubscribeSet(st.name);
}

void LabSiteBase::GotoNextState(double time)
{
	stateStack->GotoNextState(time);
}

void LabSiteBase::TryDoTimeoutState(double time)
{
	stateStack->TryDoTimeout(time);
}


int LabSiteBase::GetNbStates() {
	return stateStack->GetSize();
}

double LabSiteBase::GetCurrentStateElapsedTime(double time) {
	return stateStack->GetCurrentStateElapsedTime(time);
}

double LabSiteBase::GetCurrentStateRemainingTime(double time) {
	return stateStack->GetCurrentStateRemainingTime(time);
}



LabUserData *LabSiteBase::New(LabNode *parent_node) {
//	cout << "From new 1 : " << parent_node->GetNeighbors().size() << endl;
//	parent = parent_node;
//	cout << "From new 2 : " << parent->GetNeighbors().size() << endl;
	return new LabSiteBase(simulator, parent_node, true);
}

void LabSiteBase::Copy(LabUserData *src_site, int dep_filter)
{
	if (this->setTrackable) this->UnSubscribeAllSets();

	switch (dep_filter) {
	case IND_DEP:
		indAttributesStack->Copy(((LabSiteBase *)src_site)->indAttributesStack);
		break;
	case SITE_DEP:
		siteAttributesStack->Copy(((LabSiteBase *)src_site)->siteAttributesStack);
		break;
	case (IND_DEP | SITE_DEP):
		indAttributesStack->Copy(((LabSiteBase *)src_site)->indAttributesStack);
		siteAttributesStack->Copy(((LabSiteBase *)src_site)->siteAttributesStack);
		break;
	}

	stateStack->Copy(((LabSiteBase *)src_site)->GetStateStack());

	if (this->setTrackable) this->SubscribeAllSets();

	this->site_model_type = ((LabSiteBase *)src_site)->site_model_type;

//	bool was_unset = (this->GetSubscribedSets().find(UNSET) != this->GetSubscribedSets().end());
//	this->SetSubscribedSets(((LabSiteBase *)src_site)->GetSubscribedSets()); //this->subscribed_sets = ((LabSiteBase *)src_site)->subscribed_sets;
//	// Stays UNSET if it was...
//	if (was_unset) this->GetSubscribedSets().insert(UNSET);
}


void LabSiteBase::Binarize(ofstream *ostream)
{
	indAttributesStack->Binarize(ostream);
	siteAttributesStack->Binarize(ostream);

	stateStack->Binarize(ostream);
}

void LabSiteBase::UnBinarize(ifstream *istream)
{
	indAttributesStack->UnBinarize(istream);
	siteAttributesStack->UnBinarize(istream);

	stateStack->UnBinarize(istream);
}

//string  LabSiteBase::GetPop() {
//	return this->pop_name;
//}
//void LabSiteBase::SetPop(string pop_name) {
//	if (this->pop_name != pop_name) {
//		this->UnSubscribeSet(this->pop_name);
//		this->pop_name = pop_name;
//		this->SubscribeSet(this->pop_name);
//	}
//}

//
//void LabSiteBase::SubscribeSet(string set_name) {
//	set<string>::iterator it = subscribed_sets.find(set_name);
//	if (it == subscribed_sets.end())
//	{
//		subscribed_sets.insert(set_name);
//		LabSiteSetsManager<LabUserData *>::GetInstance()->Subscribe(set_name, this, this->GetId());
//	}
//}
//
//void LabSiteBase::UnSubscribeSet(string set_name) {
//	set<string>::iterator it = subscribed_sets.find(set_name);
//	if (it != subscribed_sets.end())
//	{
//		subscribed_sets.erase(set_name);
//		LabSiteSetsManager<LabUserData *>::GetInstance()->UnSubscribe(set_name, this, this->GetId());
//	}
//}

//
//set<string>& LabSiteBase::GetSubscribeSets() {
//	return subscribed_sets;
//}


void LabSiteBase::SubscribeSet(string set_name) {

	//	if (!setTrackable) throw LabConsts::E(LabConsts::NOT_SET_TRACKABLE);

//	t_string_set::iterator it = subscribed_sets.find(set_name);
	t_string_set::iterator it = std::find(subscribed_sets.begin(), subscribed_sets.end(), set_name);
	if (it == subscribed_sets.end())
	{
//		subscribed_sets.insert(set_name);
//		cout << "try subscribe to : #" << set_name << "#" << endl;
//		PrintSets();
		subscribed_sets.insert(set_name);
		if (setTrackable) {
			setsManager->Subscribe(set_name, this, this->GetId());
		}
//		PrintSets();
	}
}

void LabSiteBase::UnSubscribeSet(string set_name) {

	//	if (!setTrackable) throw LabConsts::E(LabConsts::NOT_SET_TRACKABLE);

//	t_string_set::iterator it = subscribed_sets.find(set_name);
	t_string_set::iterator it = std::find(subscribed_sets.begin(), subscribed_sets.end(), set_name);
	if (it != subscribed_sets.end())
	{
		subscribed_sets.erase(it);
		if (setTrackable) {
			setsManager->UnSubscribe(set_name, this, this->GetId());
		}
	}
}


void LabSiteBase::UnSubscribeAllSets() {

	//	if (!setTrackable) throw LabConsts::E(LabConsts::NOT_SET_TRACKABLE);

	t_string_set::iterator it = subscribed_sets.begin();
	while (it != subscribed_sets.end())
	{
		if (*it != U_SET && *it != UNSET)			// Never un-subscribe from Universe set... or from UNSET...
			if (setTrackable) setsManager->UnSubscribe(*it, this, this->GetId());
		std::advance(it, 1);
	}

	subscribed_sets.clear();
}

void LabSiteBase::SubscribeAllSets() {

	//	if (!setTrackable) throw LabConsts::E(LabConsts::NOT_SET_TRACKABLE);


//	bool was_unset = (subscribed_sets.find(UNSET) != subscribed_sets.end());
//	subscribed_sets.clear();
//	// Stays UNSET if it was...
//	if (was_unset) subscribed_sets.insert(UNSET);

	int index;
	bool valid_index = false;

	// All attributes
	vector<LabAttributesStack::t_attr>::iterator it1 = indAttributesStack->GetAttributes().begin();
	int i = 0, j;
	while (it1 != indAttributesStack->GetAttributes().end())
	{
		LabAttributesStack::t_attr attr = (*it1);
		index = round(attr.data.d_value);
		valid_index = indAttributesStack->IsValidIndexedAttrValue(i, index);
		if (attr.data.indexed && valid_index) {
			for (j=0; j<attr.data.nb_beans; j++) {
				if (index == j) {
					stringstream ss; ss << attr.name << j;
//					subscribed_sets.insert(ss.str());
					subscribed_sets.insert(ss.str());
					//this->PrintSets();
					if (setTrackable) setsManager->Subscribe(ss.str(), this, this->GetId());
					break;
				}
			}
		}
		it1++;
		i++;
	}

	i = 0;
	vector<LabAttributesStack::t_attr>::iterator it2 = siteAttributesStack->GetAttributes().begin();
	while (it2 != siteAttributesStack->GetAttributes().end())
	{
		LabAttributesStack::t_attr attr = (*it2);
		index = round(attr.data.d_value);
		valid_index = siteAttributesStack->IsValidIndexedAttrValue(i, index);
		if (attr.data.indexed && valid_index) {
			for (j=0; j<attr.data.nb_beans; j++) {
				if (index == j) {
					stringstream ss; ss << attr.name << j;
//					subscribed_sets.insert(ss.str());
					subscribed_sets.insert(ss.str());
					//this->PrintSets();
					if (setTrackable) setsManager->Subscribe(ss.str(), this, this->GetId());
					break;
				}
			}
		}
		it1++;
		i++;
	}

	// All states
	i = 0;
	vector<LabStateStack::t_state>::iterator it3 = stateStack->GetStates().begin();
	while (it3 != stateStack->GetStates().end())
	{
		LabStateStack::t_state state = (*it3);
		if (this->GetState() == i) {
//			subscribed_sets.insert(state.name);
			subscribed_sets.insert(state.name);
			if (setTrackable) setsManager->Subscribe(state.name, this, this->GetId());
			break;
		}
		it3++;
		i++;
	}

}

void LabSiteBase::PrintSubscribedSets() {
	cout << "{";
	t_string_set::iterator it;
	for (it = subscribed_sets.begin(); it != subscribed_sets.end(); ) {
		cout << " " << (*it);
		std::advance(it, 1);
	}
	cout << " }" << endl;
}


} /* namespace elps */


