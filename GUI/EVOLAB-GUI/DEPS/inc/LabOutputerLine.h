/*
 * LabOutputerLine.h
 *
 *  Created on: 15 sept. 2012
 *      Author: golgauth
 */

#ifndef LABOUTPUTERLINE_H_
#define LABOUTPUTERLINE_H_


#include <LabOutputer.h>

#include <vector>


using namespace std;


namespace elps {


// TODO : !!! > See the StochKit 'StandardDriverOutput.h' for a more sophisticated implementation

/**
 * A simple line after line output writer
 */
class LIB_CLASS LabOutputerLine : public LabOutputer {
public:
	LabOutputerLine();
	virtual ~LabOutputerLine();

	bool SetHeader(vector<string>& headerCols) { };
	//bool WriteData(/*vector<double> dataLine, int step*/);					// Use the default
	bool WriteStats(/*vector<vector<double> > dataLines, int step*/) { };
	bool WriteHistogram(/*vector<vector<double> > dataLines, int step*/) { };
	bool SetFooter(string footer) { };
};


}  // namespace elps

#endif /* LABOUTPUTERLINE_H_ */
