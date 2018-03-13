//============================================================================
// Name        : TestSets.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include <LabTimeUtils.h>
#include <LabSet.h>
using namespace elps;


int main() {

	LabTimer *t = new LabTimer();

	LabSet ls1(1000000);
	LabSet ls2(1000000);

	t->Start();

	for (int i=0; i < 1000000; i++) {
		ls1.Set(i);
		ls2.Set(999999 - i);
	}

	t->Stop();
	t->Print();


	return 0;
}
