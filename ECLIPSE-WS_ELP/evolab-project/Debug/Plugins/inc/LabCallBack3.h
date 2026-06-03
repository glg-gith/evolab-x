/*
 * LabCallBack3.h
 *
 *  Created on: 16 sept. 3013
 *      Author: golgauth
 */

#ifndef LABCALLBACK3_H_
#define LABCALLBACK3_H_

#include <ALabCallBack.h>



namespace elps {


/**
 * Class facility to deal with callbacks.
 *
 * See LabCallBack2 for a more Cython compliant implementation (Non-templatized version).
 *
 */
template < typename ReturnType, typename Parameter >
class LabCallBack3: public ALabCallBack
{
public:

	typedef void* Ptr;
	typedef ReturnType (*Method)(Parameter);

	LabCallBack3() { };
	/**
	 * Constructor that must be used when the callback is used in pure C++ project context
	 */
	LabCallBack3(Method _method) {
		is_cy_call = false;
		method = _method;
	};
	/**
	 * Constructor that must be used when the callback is used in Cython extension project context
	 */
	LabCallBack3(Method method, void *user_data): ALabCallBack(method, user_data) {};

	/**
	 * Build the function from the method and the parameter
	 * @param parameter Parameter passed to the method
	 * @return The return value of the method
	 */
	ReturnType operator()(Parameter parameter)
	{
		return (*method)(parameter);
	};

	/**
	 * Execute the method
	 * @param parameter Parameter passed to the method
	 * @return The return value of the method
	 */
	ReturnType execute(Parameter parameter)
	{
		return operator()(parameter);
	};


private:

	/** The method to be executed */
	Method  method;

};


}  // namespace elps

#endif /* LABCALLBACK3_H_ */
