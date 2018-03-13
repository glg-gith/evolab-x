/*
 * LabXMLUtils.cpp
 *
 *  Created on: 25 sept. 2012
 *      Author: golgauth
 */

#include "LabXMLUtils.h"

namespace elps {

LabXMLUtils::LabXMLUtils() {
	// TODO Auto-generated constructor stub

}

LabXMLUtils::~LabXMLUtils() {
	// TODO Auto-generated destructor stub
}


xmlNodePtr LabXMLUtils::GetChildByName(xmlNodePtr node, string name)
{
	xmlNodePtr cur = node->xmlChildrenNode;

	while (cur != NULL)
	{
		if ((cur->type == XML_ELEMENT_NODE) &&
							(!xmlStrcmp(cur->name, (const xmlChar *)name.c_str())))
		{
			break;
		}
		cur = cur->next;
	}

	return cur;
}

vector<xmlNodePtr> LabXMLUtils::GetChildrenByName(xmlNodePtr node, string name)
{
	vector<xmlNodePtr> ret_vec;
	xmlNodePtr cur;

    for (cur = node->children; cur != NULL; cur = cur->next)
    {
        if ((cur->type == XML_ELEMENT_NODE) &&
        					(!xmlStrcmp(cur->name, (const xmlChar *)name.c_str())))
        {
        	ret_vec.push_back(cur);
        	LabXMLUtils::GetChildrenByName(cur, name);
        }
    }

	return ret_vec;
}

void LabXMLUtils::StrToLower(string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}


} /* namespace elps */
