/*
 * LabAttributesStack.cpp
 *
 *  Created on: 18 sept. 2012
 *      Author: golgauth
 */

#include "LabAttributesStack.h"

namespace elps {

LabAttributesStack::LabAttributesStack() {
	// TODO Auto-generated constructor stub

}

LabAttributesStack::~LabAttributesStack() {
	// TODO Auto-generated destructor stub
}

void LabAttributesStack::PushAttribute(
		string name,
		////t_deptype dep_type,
		//int subset_id,
		double d_value,
		//int i_value//,
		//void *o_value
		bool indexed,
		int nb_beans
)
{
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

LabAttributesStack::t_attr LabAttributesStack::PopAttribute(/*t_deptype dep_type*/)
{
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

vector<LabAttributesStack::t_attr>& LabAttributesStack::GetAttributes() {
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


bool LabAttributesStack::IsValidIndexedAttrValue(int id, int i_value)
{
	return (i_value >= 0 && i_value < attributes[id].data.nb_beans);
}


LabAttributesStack::t_attr *LabAttributesStack::GetAttribute(/*t_deptype dep_type,*/ int id)
{
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

LabAttributesStack::t_attr *LabAttributesStack::GetAttribute(/*t_deptype dep_type,*/ string name)
{
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


void LabAttributesStack::Copy(LabAttributesStack *src_stack/*, int dep_filter*/)
{
//	if ((dep_filter & IND_DEP) == IND_DEP) indAttributes = src_stack->GetIndAttributes();
//	if ((dep_filter & SITE_DEP) == SITE_DEP) siteAttributes = src_stack->GetSiteAttributes();
//	if ((dep_filter & ENV_DEP) == ENV_DEP) envAttributes = src_stack->GetEnvAttributes();
	attributes = src_stack->GetAttributes();
}


// Implement virtual stuffs from 'LabIBinarizable'
void LabAttributesStack::Binarize(ofstream *ostream)
{
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

void LabAttributesStack::UnBinarize(ifstream *istream)
{
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


} /* namespace elps */



