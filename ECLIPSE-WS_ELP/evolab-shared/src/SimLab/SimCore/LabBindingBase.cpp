/*
 * LabBindingBase.cpp
 *
 *  Created on: Feb 27, 2013
 *      Author: golgauth
 */

#include "LabBindingBase.h"

namespace elps {

LabBindingBase::LabBindingBase() {
	// TODO Auto-generated constructor stub
	max_size_ = -1;
}

LabBindingBase::~LabBindingBase() {
	// TODO Auto-generated destructor stub
}

unsigned int LabBindingBase::GetMaxSize() {

	////if (max_size_ <= 0) LabConsts::E(LabConsts::BAD_PREV_POP_ALLOCATION);

	return max_size_;
}

void LabBindingBase::SetMaxSize(int max_size) {

	////if (pop_size <= 0) LabConsts::E(LabConsts::BAD_POP_ALLOCATION);

	max_size_ = max_size;
}


} /* namespace elps */
