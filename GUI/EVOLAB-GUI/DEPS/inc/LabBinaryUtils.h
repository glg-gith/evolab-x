/*
 * LabBinaryUtils.h
 *
 *  Created on: 20 sept. 2012
 *      Author: golgauth
 */

#ifndef LABBINARYUTILS_H_
#define LABBINARYUTILS_H_


#include <iostream>
#include <fstream>

#include <vector>

using namespace std;


#include <LabConsts.h>

namespace elps {

/**
 * Utility class for writing and reading information stored in binary files.
 */
class LIB_CLASS LabBinaryUtils {
public:
	LabBinaryUtils();
	virtual ~LabBinaryUtils();

	// GLG WARN : templates definitions MUST be in '.h', not in '.cpp'

	/**
	 * Write a value (object of any type) to an output binary stream
	 * @param val The object to write
	 * @param ostream The stream to write to
	 */
	template <typename type>
	static void WriteValueToStream(type val, ofstream *ostream)
	{
		ostream->write((const char*)&val, sizeof(val));
	}

	/**
	 * Read a value (object of any type) from an input binary stream
	 * @param val The object to read
	 * @param istream The stream to read from
	 */
	template <typename type>
	static void ReadValueFromStream(type& val, ifstream *istream)
	{
		istream->read((char*)&val, sizeof(val));
	}

	/**
	 * Read an integer from an input binary stream (no-templates for Cython compliance).
	 * @param istream The stream to read from
	 * @return The integer value
	 */
	int ReadIntFromStream(ifstream *istream)
	{
		int val;
		istream->read((char*)&val, sizeof(val));
		return val;
	}

	/**
	 * Read a double from an input binary stream (no-templates for Cython compliance).
	 * @param istream The stream to read from
	 * @return The double value
	 */
	double ReadDoubleFromStream(ifstream *istream)
	{
		double val;
		istream->read((char*)&val, sizeof(val));
		return val;
	}

	/**
	 * Write a vector (containing objects of any type) to an output binary stream
	 * @param vect The vector of objects
	 * @param ostream The stream to write to
	 */
	template <typename vectorType>
	static void WriteVectorToStream(vector<vectorType>& vect, ofstream *ostream)
	{
		int size = vect.size();
		ostream->write((const char*)&size, sizeof(size));
		ostream->write((const char*)&vect[0], size * sizeof(vectorType));
	}

	/**
	 * Read a vector (containing objects of any type) from an input binary stream
	 * @param vect The vector of objects
	 * @param istream The stream to read from
	 */
	template <typename vectorType>
	static void ReadVectorFromStream(vector<vectorType>& vect, ifstream *istream)
	{
	    int size;
	    istream->read((char*)&size, sizeof(size));
	    vect.resize(size);
	    istream->read((char*)&vect[0], size * sizeof(vectorType));
	}

};

} /* namespace elps */
#endif /* LABBINARYUTILS_H_ */
