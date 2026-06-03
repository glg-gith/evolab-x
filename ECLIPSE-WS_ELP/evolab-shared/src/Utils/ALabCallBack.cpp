/*
 * ALabCallBack.cpp
 *
 *  Created on: 9 oct. 2012
 *      Author: golgauth
 */

#include "ALabCallBack.h"

namespace elps {


ALabCallBack::ALabCallBack() {
	is_cy_call = true;
};

ALabCallBack::~ALabCallBack() {
};

ALabCallBack::ALabCallBack(Method method, void *user_data) {
	is_cy_call = true;
	_method = method;
	_user_data = user_data;
};

double ALabCallBack::cy_execute(void *parameter)
{
	return _method(parameter, _user_data);
};


} /* namespace elps */
