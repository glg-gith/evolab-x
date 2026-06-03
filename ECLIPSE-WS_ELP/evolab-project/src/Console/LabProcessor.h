/*
 * LabProcessor.h
 *
 *  Created on: 18 juil. 2012
 *      Author: golgauth
 */

#ifndef LABPROCESSOR_H_
#define LABPROCESSOR_H_

#include "Elp.h"

namespace elp {

class LabProcessor : public Singleton<LabProcessor> {
	friend class Singleton<LabProcessor>;
public:
	LabProcessor();
	virtual ~LabProcessor();

	void ProcessPlugin(string plug_name);
};

} /* namespace elp */
#endif /* LABPROCESSOR_H_ */
