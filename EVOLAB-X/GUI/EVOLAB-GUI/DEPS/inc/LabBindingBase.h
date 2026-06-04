/*
 * LabBindingBase.h
 *
 *  Created on: Feb 27, 2013
 *      Author: golgauth
 */

#ifndef LABBINDINGBASE_H_
#define LABBINDINGBASE_H_


#include <LabConsts.h>

namespace elps {

class LIB_CLASS LabBindingBase {
public:
	LabBindingBase();
	virtual ~LabBindingBase();


protected:
	/**
	 * Gets the total capacity (maximum number of Sites) of the binding
	 */
	unsigned int GetMaxSize();
	/**
	 * Sets the total capacity (maximum number of Sites) of the binding
	 * @param max_size The total number of Sites
	 */
	void SetMaxSize(int max_size);


private:
	unsigned int max_size_;
};

} /* namespace elps */
#endif /* LABBINDINGBASE_H_ */
