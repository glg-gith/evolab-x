/*
 * LabSingletonDerivable.h
 *
 *  Created on: Feb 13, 2013
 *      Author: golgauth
 */

#ifndef LABSINGLETONDERIVABLE_H_
#define LABSINGLETONDERIVABLE_H_


#include <iostream>

//using namespace std;

namespace elps {

/**
 *
 * Derivable Singleton class (Not the best because the constructor is only protected...)
 *
 * USAGE :
 *
 * class DerivedSingleton : public LabSingletonDerivable {
 * public:
 *     static void initInstance() {
 *         new DerivedSingleton;  				// DerivedSingleton() calls LabSingletonDerivable(), which sets this as "the instance"
 *     }
 * protected:
 *     DerivedSingleton() { }   				// so we can't be instantiated by outsiders
 *     ~DerivedSingleton() { }  				// so random strangers can't delete me
 * };
 *
 */
class LabSingletonDerivable {

public:

	static LabSingletonDerivable *getInstance() {
		if (!theOneTrueInstance) initInstance();
		return theOneTrueInstance;
	}
	static void initInstance() { new LabSingletonDerivable; }

protected:
	LabSingletonDerivable() {
		if (theOneTrueInstance) throw std::exception("Instance already exists");
		theOneTrueInstance = this;
	}
	LabSingletonDerivable(LabSingleton const&);         // Don't Implement
    void operator=(LabSingletonDerivable const&);       // Don't implement
	virtual ~LabSingletonDerivable() { } 				// So random strangers can't delete me

private:
	static LabSingletonDerivable *theOneTrueInstance;

};

LabSingletonDerivable* LabSingletonDerivable::theOneTrueInstance = 0;



} /* namespace elps */
#endif /* LABSINGLETONDERIVABLE_H_ */
