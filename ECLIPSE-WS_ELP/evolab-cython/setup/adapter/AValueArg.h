/*
 * AValueArg.h
 *
 *  Created on: 16 oct. 2012
 *      Author: golgauth
 */

#ifndef AVALUEARG_H_
#define AVALUEARG_H_

#include <string>
#include "../../inc/ValueArg.h"

using namespace TCLAP;

namespace elps {

class ADoubleArg: public ValueArg<double> {
public:

	virtual ~ADoubleArg();

	ADoubleArg( const std::string& flag,
              const std::string& name,
              const std::string& desc,
              bool req,
              double value,
              const std::string& typeDesc);

};

class AIntArg: public ValueArg<int> {
public:

	virtual ~AIntArg();

	AIntArg( const std::string& flag,
              const std::string& name,
              const std::string& desc,
              bool req,
              int value,
              const std::string& typeDesc);

};

class AStringArg: public ValueArg<std::string> {
public:

	virtual ~AStringArg();

	AStringArg( const std::string& flag,
              const std::string& name,
              const std::string& desc,
              bool req,
              std::string value,
              const std::string& typeDesc);

};

} /* namespace elps */
#endif /* AVALUEARG_H_ */
