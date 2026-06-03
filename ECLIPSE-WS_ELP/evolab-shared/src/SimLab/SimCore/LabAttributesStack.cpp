
#include "LabAttributesStack.h"

#include "LabBinaryUtils.h"

namespace elps {

LabAttributesStack::LabAttributesStack(){
	// TODO Auto-generated constructor stub

}

LabAttributesStack::~LabAttributesStack(){
	// TODO Auto-generated destructor stub
}

//		t_attr() {};
//		t_attr(const t_attr& rhs) {
////			cout << "t_attr copy ctor called !!!!!!!" << endl;
//			name = rhs.name;
//			data.d_value = rhs.data.d_value;
//			data.indexed = rhs.data.indexed;
//			data.nb_beans = rhs.data.nb_beans;
//		}
//*
//	 * Add a new attribute on top of the stack.
//	 * @param name Name of the attribute (better use a unique name)
//	 * @param dep_type Dependency level
//	 * @param d_value Value expressed as a double (use i_value if an integer value is required)
//	 * @param i_value Value expressed as an int (use d_value if a float value is required)
//	 
void LabAttributesStack::PushAttribute(string name, double d_value, bool indexed, int nb_beans) {
	t_attr attr;

	attr.name = name;
	attr.data.d_value = d_value;
	//attr.data.i_value = i_value;
	//attr.o_value = o_value;

	attr.data.indexed = indexed;
	attr.data.nb_beans = nb_beans;

	attributes.push_back(attr);

//	switch (dep_type)
//	{
//	case IND_DEP:
//		attr.data.dep_type = IND_DEP;
//		indAttributes.push_back(&attr);
//		break;
//	case SITE_DEP:
//		attr.data.dep_type = SITE_DEP;
//		siteAttributes.push_back(&attr);
//		break;
//	case ENV_DEP:
//		attr.data.dep_type = ENV_DEP;
//		envAttributes.push_back(&attr);
//		break;
//	}

}

//*
//	 * Remove the last pushed attribute from the stack.
//	 * @param dep_type The dependency level has to be specified<br/>
//	 * 			(as the stack deals with 3 separated vectors - one for each level)
//	 
LabAttributesStack::t_attr LabAttributesStack::PopAttribute() {
	t_attr attr = attributes.at(attributes.size()-1);
//	switch (attr.data.dep_type)
//	{
//	case IND_DEP:
//		indAttributes.pop_back();
//		break;
//	case SITE_DEP:
//		siteAttributes.pop_back();
//		break;
//	case ENV_DEP:
//		envAttributes.pop_back();
//		break;
//	}
	attributes.pop_back();
	return attr;
}

////
vector<LabAttributesStack::t_attr> & LabAttributesStack::GetAttributes() {
	return attributes;
}

//vector<LabAttributesStack::t_attr*> LabAttributesStack::GetIndAttributes()
//{
//	return indAttributes;
//}
//vector<LabAttributesStack::t_attr*> LabAttributesStack::GetSiteAttributes()
//{
//	return siteAttributes;
//}
//
//vector<LabAttributesStack::t_attr*> LabAttributesStack::GetEnvAttributes()
//{
//	return envAttributes;
//}

int LabAttributesStack::GetSize() {
	return attributes.size();
}

//int LabAttributesStack::GetSize(t_deptype dep_type) {
//	switch (dep_type)
//	{
//	case IND_DEP:
//		return indAttributes.size();
//		break;
//	case SITE_DEP:
//		return siteAttributes.size();
//		break;
//	case ENV_DEP:
//		return envAttributes.size();
//		break;
//	}
//	return 0;
//}
//
//int LabAttributesStack::GetSize(int dep_type) {
//	return GetSize((t_deptype)dep_type);
//}

bool LabAttributesStack::IsValidIndexedAttrValue(int id, int i_value) {
	return (i_value >= 0 && i_value < attributes[id].data.nb_beans);
}

//*
//	 * Get a single attribute by its dependency level and its id
//	 * @param dep_type Dependency level
//	 * @param id Identifier in the list relatively to the specified dependency level
//	 * @return
//	 
LabAttributesStack::t_attr * LabAttributesStack::GetAttribute(int id) {
	/*switch (dep_type)
	{
	case IND_DEP:
		return &(indAttributes[id]);
		break;
	case SITE_DEP:
		return &(siteAttributes[id]);
		break;
	case ENV_DEP:
		return &(envAttributes[id]);
		break;
	}
	return NULL;*/

	return &(attributes[id]);
}

//*
//	 * Get a single attribute by its dependency level and its name
//	 * @param dep_type Dependency level
//	 * @param name Name in the list relatively to the specified dependency level
//	 * @return
//	 
LabAttributesStack::t_attr * LabAttributesStack::GetAttribute(string name) {
	// TODO ...
	/*switch (dep_type)
	{
	case IND_DEP:
		// ...
		break;
	case SITE_DEP:
		// ...
		break;
	case ENV_DEP:
		// ...
		break;
	}
	return NULL;*/

	return NULL; // TODO
}

//*
//	 * Copy the attributes of a given stack (src_stack) into this one.
//	 * @param src_stack Source stack
//	 * @param dep_filter Filter used to select which attributes will be copied<br/>
//	 * 			(ex1 : dep_filter = IND_DEP | SITE_DEP, means : copies only non-environmental attributes<br/>
//	 * 			 ex2 : dep_filter = IND_DEP | SITE_DEP | ENV_DEP : copies all)
//	 
void LabAttributesStack::Copy(LabAttributesStack * src_stack) {
//	if ((dep_filter & IND_DEP) == IND_DEP) indAttributes = src_stack->GetIndAttributes();
//	if ((dep_filter & SITE_DEP) == SITE_DEP) siteAttributes = src_stack->GetSiteAttributes();
//	if ((dep_filter & ENV_DEP) == ENV_DEP) envAttributes = src_stack->GetEnvAttributes();
	attributes = src_stack->GetAttributes();
}

// Implement virtual stuffs from 'LabIBinarizable'

void LabAttributesStack::Binarize(ofstream * ostream) {
	// Save the 'data' part of the attributes
	vector<t_attr_data> data;
	vector<t_attr>::iterator it;

	/*for (it=indAttributes.begin(); it!=indAttributes.end(); it++)
		data.push_back((*it).data);
	LabBinaryUtils::WriteVectorToStream(data, ostream);

	for (it=siteAttributes.begin(); it!=siteAttributes.end(); it++)
		data.push_back((*it).data);
	LabBinaryUtils::WriteVectorToStream(siteAttributes, ostream);

	for (it=envAttributes.begin(); it!=envAttributes.end(); it++)
		data.push_back((*it).data);
	LabBinaryUtils::WriteVectorToStream(envAttributes, ostream);*/

	for (it=attributes.begin(); it!=attributes.end(); it++)
		data.push_back((*it).data);
	LabBinaryUtils::WriteVectorToStream(data, ostream);
}

//*
//	 * Restore the stack from a given input stream (binary format).
//	 * @param istream Input stream from which to get the info required to restore the stack
//	 
void LabAttributesStack::UnBinarize(ifstream * istream) {
	// Restore the 'data' part of the attributes
	vector<t_attr_data> data;
	vector<t_attr>::iterator it;
	vector<t_attr_data>::iterator it2;

	/*LabBinaryUtils::ReadVectorFromStream(data, istream);
	it = indAttributes.begin(); it2 = data.begin();
	while (it != indAttributes.end()) {
		(*it).data = (*it2); it++; it2++;
	}

	LabBinaryUtils::ReadVectorFromStream(data, istream);
	it = siteAttributes.begin(); it2 = data.begin();
	while (it != siteAttributes.end()) {
		(*it).data = (*it2); it++; it2++;
	}

	LabBinaryUtils::ReadVectorFromStream(data, istream);
	it = envAttributes.begin(); it2 = data.begin();
	while (it != envAttributes.end()) {
		(*it).data = (*it2); it++; it2++;
	}*/

//	attributes.clear();
//	indAttributes.clear(); siteAttributes.clear(); envAttributes.clear();
	LabBinaryUtils::ReadVectorFromStream(data, istream);
	it = attributes.begin(); it2 = data.begin();
	while (it != attributes.end()) {
		(*it).data = (*it2);
//		switch ((*it).data.dep_type)
//		{
//		case IND_DEP:
//			indAttributes.push_back(&(*it));
//			break;
//		case SITE_DEP:
//			siteAttributes.push_back(&(*it));
//			break;
//		case ENV_DEP:
//			envAttributes.push_back(&(*it));
//			break;
//		}
		it++; it2++;
	}

}


} // namespace elps
