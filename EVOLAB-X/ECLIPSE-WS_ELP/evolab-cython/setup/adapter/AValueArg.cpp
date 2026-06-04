/*
 * AValueArg.cpp
 *
 *  Created on: 16 oct. 2012
 *      Author: golgauth
 *
 *
 *  Just because Cython doesn't like templatized classes :
 *  We provide this adapter that handles only "doubles"
 *  (we can store int or bool in a double, so we can use
 *  this class for all types)
 *
 */

#include "AValueArg.h"

namespace elps {

//AValueArg::AValueArg() {
//	// TODO Auto-generated constructor stub
//
//}

ADoubleArg::~ADoubleArg() {
	// TODO Auto-generated destructor stub
}

ADoubleArg::ADoubleArg( const std::string& flag,
          const std::string& name,
          const std::string& desc,
          bool req,
          double value,
          const std::string& typeDesc): ValueArg<double>(flag, name, desc, req, value, typeDesc, NULL) { }

AIntArg::~AIntArg() {
	// TODO Auto-generated destructor stub
}

AIntArg::AIntArg( const std::string& flag,
          const std::string& name,
          const std::string& desc,
          bool req,
          int value,
          const std::string& typeDesc): ValueArg<int>(flag, name, desc, req, value, typeDesc, NULL) { }

AStringArg::~AStringArg() {
	// TODO Auto-generated destructor stub
}

AStringArg::AStringArg( const std::string& flag,
          const std::string& name,
          const std::string& desc,
          bool req,
          std::string value,
          const std::string& typeDesc): ValueArg<std::string>(flag, name, desc, req, value, typeDesc, NULL) { }


} /* namespace elps */
