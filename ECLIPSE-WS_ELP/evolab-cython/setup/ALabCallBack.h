/*
 * ALabCallBack.h
 *
 *  Created on: 9 oct. 2012
 *      Author: golgauth
 */

#ifndef ALABCALLBACK_H_
#define ALABCALLBACK_H_

// Created by Cython when providing 'public api' keywords
#include "../cyelp_api.h"

#include "../../inc/LabCallBack2.h"

#include <iostream>

using namespace std;

namespace elps {

//template < typename ReturnType, typename Parameter >
class ALabCallBack: public LabCallBack2<double, void*> {
public:



	typedef double (*Method)(void *param, void *user_data);

	PyObject *m_obj;

	ALabCallBack();

	ALabCallBack(Method method, void *user_data);

	// double operator()(void * parameter)
	// {
	//	 cout << "EXEC 1 !" << endl;
	//	 cout << method << endl;
	//	 cout << (*method) << endl;
	//	 (*method)(parameter);
	//	 cout << "EXEC 2 !" << endl;
	//    return (*method)(parameter);
	// };

	double cy_execute(/*Method func, void *user_data,*/ void *param)
	{
		//return operator()(parameter);
		cout << "Exec !!!" << endl;
//		PyObject * param = new PyObject();
//		void * v = (void*)m_obj;
		return _method(param, _user_data);
		cout << "Exec !!!" << endl;
	};

	// void SetMethod(Method, void* func) {
	//	 method         = (Method)func;
	//	 cout << func << endl;
	// };

private:

	//void *_param;
	Method _method;
	void *_user_data;

};


} /* namespace elps */
#endif /* ALABCALLBACK_H_ */
