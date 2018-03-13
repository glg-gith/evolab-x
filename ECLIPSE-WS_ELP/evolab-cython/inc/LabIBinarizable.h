/*
 * LabIBinarizable.h
 *
 *  Created on: 20 sept. 2012
 *      Author: golgauth
 */

#ifndef LABIBINARIZABLE_H_
#define LABIBINARIZABLE_H_


#include <iostream>
#include <fstream>


#include <LabConsts.h>

using namespace std;

namespace elps {


/**
 * Interface class for all the objects for which we potentially need to backup/restore the state
 * physically to/from a file.
 */
class LIB_CLASS LabIBinarizable {
public:
	LabIBinarizable() { };
	virtual ~LabIBinarizable() { };

	/**
	 * Save the current state of the binarizable object to a given output stream (binary format).<br/>
	 * Must be overridden !
	 * @param ostream Output stream to backup to.
	 */
	virtual void Binarize(ofstream *ostream) {};// = 0; // Should be 'pure' but we need it callable in Cython
	/**
	 * Restore the binarizable object from a given input stream (binary format).<br/>
	 * Must be overridden !
	 * @param istream Input stream from which to get the info required to restore the object
	 */
	virtual void UnBinarize(ifstream *istream) {};// = 0; // Should be 'pure' but we need it callable in Cython

};

} /* namespace elps */
#endif /* LABIBINARIZABLE_H_ */
