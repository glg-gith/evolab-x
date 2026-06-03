/*
 * LabXMLUtils.h
 *
 *  Created on: 25 sept. 2012
 *      Author: golgauth
 */

#ifndef LABXMLUTILS_H_
#define LABXMLUTILS_H_

#include <libxml/parser.h>

#include <iostream>
#include <vector>

#include <algorithm>
#include <string>


#include <LabConsts.h>

using namespace std;

namespace elps {

/**
 * Facility class for dealing with XML (depends on "libxml2" library).
 */
class LIB_CLASS LabXMLUtils {
public:
	LabXMLUtils();
	virtual ~LabXMLUtils();

	/**
	 * Find a child by its name among the first level children of a given XML node.
	 * @param node The node to search
	 * @param name The name of the child
	 * @return The first node having that name
	 */
	static xmlNodePtr GetChildByName(xmlNodePtr node, string name);
	/**
	 * Find all the children having a given name among the first level children of a given XML node.
	 * @param node The node to search
	 * @param name The name of the child(ren)
	 * @return The list of the children
	 */
	static vector<xmlNodePtr> GetChildrenByName(xmlNodePtr node, string name);

	/**
	 * Converts a string to lower case.
	 * @param str The string to convert
	 */
	static void StrToLower(string& str);
};

} /* namespace elps */
#endif /* LABXMLUTILS_H_ */
