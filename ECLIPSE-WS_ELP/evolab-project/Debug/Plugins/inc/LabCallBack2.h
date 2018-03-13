/*
 * LabCallBack2.h
 *
 *  Created on: 16 sept. 2012
 *      Author: golgauth
 */

#ifndef LABCALLBACK2_H_
#define LABCALLBACK2_H_

#include <ALabCallBack.h>



namespace elps {

/**
 * Utility class to deal with callbacks.<br/>
 * (Non templatized version of LabCallBack3 for ease of use with Cython).
 *
 *  2 versions of the constructor :
 *
 * 	* For C usage (1 param): LabCallBack2(Method2 _method)
 *	* For Cython usage (2 param): LabCallBack2(Method method, void *user_data)
 *
 * This version of LabCallBack obey to the following pattern :
 *
 * 	double (*method)(void* parameter)
 *
 * That is to say that it returns a double and nothing else. Return 0.0
 * if you do not need to return anything...
 *
 * USAGE :
 * =======
 *
 * First of all define the callback methods to be fired :
 *
 * \code
 * 	double MyMethod(void *a_param)
 * 	{
 * 		// ...
 * 		double rate = a_param * 2.0;                 // Do something with "a_param" (can be any object)
 * 		// ...
 * 		return rate;                                 // Return a float
 * 	}
 * \endcode
 *
 * Then we can create the callback :
 *
 * \code
 * 	LabCallBack2 *my_callback = new LabCallBack2(MyMethod);
 * \endcode
 *
 * And call it later on :
 *
 * \code
 * 	cout << my_callback.execute(25.0) << endl;        // Writes : 50.0
 * \endcode
 *
 */
class LabCallBack2: public ALabCallBack
{
public:

	typedef double (*Method2)(void*);

	LabCallBack2() { };
	/**
	 * Constructor that must be used when the callback is used in pure C++ project context.
	 */
	LabCallBack2(Method2 _method) {
		is_cy_call = false;
		method = _method;
	};
	/**
	 * Constructor that must be used when the callback is used in Cython extension project context.
	 */
	LabCallBack2(Method method, void *user_data): ALabCallBack(method, user_data) {};

	/**
	 * Build the function from the method and the parameter.
	 * @param parameter Parameter passed to the method.
	 * @return The return value of the method.
	 */
	double operator()(void* parameter)
	{
		return (*method)(parameter);
	};

	/**
	 * Execute the method.
	 * @param parameter Parameter passed to the method.
	 * @return The return value of the method.
	 */
	double execute(void* parameter)
	{
		return operator()(parameter);
	};

	Method2& GetMethod2() { return method; }


private:

	/** The method to be executed */
	Method2  method;

};


}  // namespace elps

#endif /* LABCALLBACK2_H_ */
