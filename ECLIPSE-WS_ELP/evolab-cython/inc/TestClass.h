/*
 * TestClass.h
 *
 *  Created on: 2 oct. 2012
 *      Author: golgauth
 */

#ifndef TESTCLASS_H_
#define TESTCLASS_H_


namespace elps {

class TestClass {

public:
	TestClass();
	virtual ~TestClass();

    int getA() { return this->a; };
    virtual int override_me() { return 2; };
    int calculate(int a) { return a * this->override_me(); }

private:
    int a;

};

} /* namespace elps */
#endif /* TESTCLASS_H_ */
