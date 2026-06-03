/*
 * ALabCallBack.h
 *
 *  Created on: 9 oct. 2012
 *      Author: golgauth
 */

#ifndef ALABCALLBACK_H_
#define ALABCALLBACK_H_


#include <iostream>


#include <LabConsts.h>

using namespace std;

namespace elps {

//template < typename ReturnType, typename Parameter >
/**
 * Kind of interface class from which LabCallBack2 and LabCallBack3
 * inherit from. Designed for compatibility with Cython.
 */
class LIB_CLASS ALabCallBack {
public:

	typedef double (*Method)(void *param, void *user_data);

	ALabCallBack();
	ALabCallBack(Method method, void *user_data);
	virtual ~ALabCallBack();

	/**
	 * Execute the callback method from the Cython development context
	 * @param parameter The parameter to pass to the method
	 * @return The return value of the method
	 */
	double cy_execute(void *parameter);

	/**
	 * Test if we here in the Cython development context
	 * @return True if the method is called  in the Cython development context
	 */
	bool IsCythonCall()
	{
		return is_cy_call;
	}

//	void Copy(ALabCallBack *src_cb)
//	{
//		is_cy_call = src_cb->is_cy_call;
//		_method = src_cb->_method;
//		_user_data = src_cb->_user_data;
//	}

	Method& GetMethod()   { return _method; }
	void *  GetUserData() { return _user_data; }

protected:

	/** If the method/callback is called in the Cython development context, or not. */
	bool is_cy_call;

private:

	//void *_param;
	/** The method to be called */
	Method _method;
	/** The additional data to be passed (additionally to the parameter) */
	void *_user_data;

};


} /* namespace elps */
#endif /* ALABCALLBACK_H_ */
