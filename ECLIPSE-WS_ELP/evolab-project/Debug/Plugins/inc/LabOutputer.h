/*
 * LabOutputer.h
 *
 *  Created on: 13 sept. 2012
 *      Author: golgauth
 */

#ifndef LABOUTPUTER_H_
#define LABOUTPUTER_H_


#include <iostream>
#include <string>
#include <vector>

#include <fstream>

#include <sstream>

#include <math.h>

#include <LabConsts.h>

#include <LabIBinarizable.h>
#include <LabBinaryUtils.h>


using namespace std;


namespace elps {


// TODO : !!! > See the StochKit 'StandardDriverOutput.h' for a more sophisticated implementation

/**
 * Basic implementation for output management :
 *
 * Basically this class provides the features to produce output of data into a file, the standard
 * output, or both. It manipulates DATASETs (sets of data) which are simple matrixes of doubles.
 *
 * DATASET :<br/>
 * 		* Each set of data is expressed as a list of list of doubles
 * 		* Each time the set of data is written, it is cleared (waiting to be filled again
 * 		  with the next data). It behaves as a buffer.
 *
 * We also can customize the header and footer (SetHeader(), SetFooter()) of the output
 * as well as write free entries on the fly (InsertText() - in this case the text is inserted
 * immediately, unlike DATSET which are buffered until asked for writing).
 *
 * USAGE :
 * =======
 * 		* Instantiate
 * 		* Set the associated file (only required if planned to write into files)
 * 		* Set the separator (which will be used to separate the data of a column)
 * 		* Define the mode of output (SetOuputMode())
 * 		* Buffer some data lines (AddDataLine())
 * 		* Ask for writing the current set of data at your convenience (WriteData())
 * 		* Or insert some text on the fly (InsertText())
 *
 * <b>Note :</b> The "statistics" and "histogram" features are not yet implemented.
 *
 */
class LIB_CLASS LabOutputer: public LabIBinarizable {
public:
	LabOutputer();
	virtual ~LabOutputer();

	// Flags
	/**
	 * Output type : 1 : only to file, 2 only to standard output, 4 both file and standard output
	 */
	typedef enum { TO_FILE = 1, TO_STD = 2, TO_BOTH = TO_FILE | TO_STD } t_output;

	// Recovery tag tag
	/** Recovery tag opening tag */
	static const string TAG_BEGIN;
	/** Recovery tag closing tag */
	static const string TAG_END;


	// File
	/**
	 * Set the file name associated to this outputer (not required if we work only on standard output).
	 * @param filename The full path of the file
	 * @param erase If set to True, the given will be erased (if it already exists)
	 */
	void SetFilename(const string filename, bool erase);
	/**
	 * Get the full path of the associated output file.
	 * @return The full path.
	 */
	string GetFilename();
	/**
	 * Separator being used between written values/data (';', TAB, or else...).
	 * @param sep Can be one or more characters
	 */
	void SetSeparator(string sep);

	// To be overridden :
	/**
	 * Hook designed to be overridden. Set the header text of the output (columns/fields names).
	 * @param headerCols Ordered list of columns names
	 * @return
	 */
	virtual bool SetHeader(vector<string>& headerCols) = 0;
	/**
	 * Write the current data buffer to file (Write current dataset as text using the separator - SetSeparator()).
	 * @return
	 */
	virtual bool WriteData(/*vector<double> dataLine, int step*/);
	/**
	 * Hook designed to be overridden. Write some user defined statistics (Make stats from the current dataset).
	 * @return
	 */
	virtual bool WriteStats(/*vector<vector<double> > dataLines, int step*/) = 0;
	/**
	 * Hook designed to be overridden. Write some user defined histogram (Build histogram from the current dataset).
	 * @return
	 */
	virtual bool WriteHistogram(/*vector<vector<double> > dataLines, int step*/) = 0;
	/**
	 * Hook designed to be overridden. Write the final footer text (Ex. The simulation total time, ...).
	 * @param footer
	 * @return
	 */
	virtual bool SetFooter(string footer) = 0;

	/**
	 * Set the mode of output (See LabOutputer::t_output)
	 * @param mode Can be file, std or both
	 */
	void SetOuputMode(t_output mode);
	/**
	 * Cython version (uses int instead of LabOutputer::t_output).
	 */
	void SetOuputMode(int mode);
	// Dataset = set of dataLines
	/**
	 * Get the current set of data.
	 * @return List of data lines (each line is a vector of double)
	 */
	vector<vector<double> >& GetDataSet();
	/**
	 * Set the current set of data (useful to recover from a previous dumped state - interrupted simulation).
	 * @param dataSet The set of data to fill
	 */
	void SetDataSet(vector<vector<double> >& dataSet);
	/**
	 * Add a line to the current set of data.
	 * @param dataLine Line of data = list of double values
	 */
	void AddDataLine(vector<double>& dataLine);
	/**
	 * Clear the current set of data.
	 */
	void ResetDataSet();

	/**
	 * Add some text at any time (the user is responsible for carriage return with this feature).
	 * @param text The text to be immediately inserted (no delay).
	 */
	void InsertText(const string text);

	// Implement virtual
	/**
	 * Insert a recovery point/tag to the given output stream (binary format).
	 * @param ostream Output stream to backup to.
	 */
	void Binarize(ofstream *ostream);
	/**
	 * Restore the output file, restarting writing from the last recovery point/tag - from a
	 * given input stream (binary format).
	 * @param istream Input stream from which to get the tag required to continue writing at
	 */
	void UnBinarize(ifstream *istream);
	/**
	 * Insert the given recovery tag at the current position in the output file.
	 * Must be called in Binarize().
	 * @param rec_tag A unique tag for later recovery. Allows to restart writing into the
	 *                output file from that point.
	 */
	void InsertRecoveryTag(/*const string rec_tag*/);
	/**
	 * Find the given tag in the output file and places the readhead at this point (aka :
	 * delete the text which comes after that point).
	 * Must be called in UnBinarize().
	 * @param rec_tag A unique tag for the recovery. Allows to restart writing into the
	 *                output file from that point.
	 */
	bool GotoLastRecoveryTag(/*const string rec_tag*/);
//	string GetCurrentRecoveryTag();
//	/**
//	 * Set the recovery point/tag that will be used to restart from (when calling
//	 * GotoLastRecoveryTag()).
//	 * @param rec_tag
//	 */
//	void SetCurrentRecoveryTag(string rec_tag);


	// Static utilities
	/***
	 * Display an ASCII progress bar.
	 * @param len Length (expressed in number of characters).
	 * @param percent The current percentage.
	 * @param erase If the progress bar overrides itself at each iteration, or not (go to next line).
	 * @param only_text If the progress is only displayed as text (ex. "82%").
	 */
	static void DrawProgressBar(int len, double percent, bool erase = true, bool only_text = false);

private:

	/**
	 * Create or override if existing file.
	 * @return
	 */
	bool OpenFile();
	/**
	 * Append to file.
	 * @return
	 */
	bool AppendFile();
	/**
	 * Close an opened file
	 */
	void CloseFile();
	/**
	 * Write the predefined header.
	 */
	void WriteHeader();
	/**
	 * Write the predefined footer.
	 */
	void WriteFooter();
	/**
	 * Take care of creating the file if not existing or erasing it.
	 * @param erase Erase the file if it exists
	 */
	void PrepareFile(bool erase);
	/**
	 * Close all.
	 */
	void FinalizeFile();

	t_output outputMode;
	string outFilename;
	ofstream f_out;
	//bool of_created;
	string separator;
	string header;
	string footer;
	vector<vector<double> > dataSet;

	int /*long long -> does it cause problems in Python/Cython ??? */ cur_rec_tag_val;
};


}  // namespace elps

#endif /* LABOUTPUTER_H_ */
