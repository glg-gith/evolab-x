/*
 * LabCallback.h
 *
 *  Created on: 15 sept. 2012
 *      Author: golgauth
 */

#ifndef LABCALLBACK_H_
#define LABCALLBACK_H_


namespace elps {


template < class Class, typename ReturnType, typename Parameter >
class LabCallBack
{
public:

 typedef ReturnType (Class::*Method)(Parameter);

 LabCallBack(Class* _class_instance, Method _method)
 {
    class_instance = _class_instance;
    method         = _method;
 };

 ReturnType operator()(Parameter parameter)
 {
    return (class_instance->*method)(parameter);
 };

 ReturnType execute(Parameter parameter)
 {
    return operator()(parameter);
 };

private:

 Class*  class_instance;
 Method  method;
};


}  // namespace elps

#endif /* LABCALLBACK_H_ */
