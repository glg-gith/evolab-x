/*
 * LabOutputer.cpp
 *
 *  Created on: 13 sept. 2012
 *      Author: golgauth
 */

#include "LabOutputer.h"

namespace elps {


const string LabOutputer::TAG_BEGIN = "<RT>";
const string LabOutputer::TAG_END = "</RT>";


LabOutputer::LabOutputer() {
	// TODO Auto-generated constructor stub
}

LabOutputer::~LabOutputer() {
	// TODO Auto-generated destructor stub
}

bool LabOutputer::OpenFile()
{
	f_out.open(outFilename.c_str());
	//of_created = true;
	return true;
}

bool LabOutputer::AppendFile()
{
	f_out.open(outFilename.c_str(), fstream::in | fstream::out | fstream::app);
	return true;
}

void LabOutputer::CloseFile()
{
	f_out.close();
}


void LabOutputer::PrepareFile(bool erase) {
	// Start a new fresh file
	if (erase)
	{
		OpenFile();
		//	WriteHeader();
		CloseFile();
	}
	cout << "File \"" << outFilename << "\" : Ready for writing !" << endl;
}

void LabOutputer::FinalizeFile() {
	OpenFile();
	//	WriteFooter();
	CloseFile();
}

void LabOutputer::SetFilename(const string filename, bool erase)
{
	outFilename = filename;
	PrepareFile(erase);
}

string LabOutputer::GetFilename()
{
	return outFilename;
}

void LabOutputer::SetSeparator(string sep)
{
	separator = sep;
}

void LabOutputer::SetOuputMode(t_output mode) {
	outputMode = mode;
}

void LabOutputer::SetOuputMode(int mode) {
	outputMode = (t_output)mode;
}


// Default behaviour : Write the content of the whole dataset and clear it
bool LabOutputer::WriteData(/*vector<double> dataLine, int step*/)
{
	bool write_file = ( (outputMode == LabOutputer::TO_FILE || outputMode == LabOutputer::TO_BOTH) &&
			f_out.good() );
	bool write_std = ( (outputMode == LabOutputer::TO_STD || outputMode == LabOutputer::TO_BOTH) &&
			cout.good() );

	if (write_file) { AppendFile(); }

	vector<vector<double> >::iterator it;
	vector<double>::iterator it2;
	vector<double> a_line;
	for (it = dataSet.begin(); it != dataSet.end(); it++)
	{
		a_line = (*it);
		for (it2 = a_line.begin(); it2 != a_line.end(); it2++)
		{
			// Write in file
			if (write_file) { f_out << (*it2) << separator; }
			// Write in std::cout
			if (write_std) { cout << (*it2) << separator; }
		}
		if (write_file) { f_out << endl; }
		if (write_std) { cout << endl; }
	}

	if (write_file) { CloseFile(); }
	//ResetDataSet();

	return true;
}

void LabOutputer::AddDataLine(vector<double>& dataLine)
{
	dataSet.push_back(dataLine);
}

void LabOutputer::ResetDataSet()
{
	dataSet.clear();
}

void LabOutputer::InsertText(const string text)
{
	bool write_file = ( (outputMode == LabOutputer::TO_FILE || outputMode == LabOutputer::TO_BOTH) &&
			f_out.good() );
	bool write_std = ( (outputMode == LabOutputer::TO_STD || outputMode == LabOutputer::TO_BOTH) &&
			cout.good() );

	if (write_file) {  AppendFile(); }

	if (write_file) { f_out << text; }
	if (write_std) { cout << text; }

	if (write_file) { CloseFile(); }
}


void LabOutputer::Binarize(ofstream *ostream)
{
	// Insert a new unique recovery tag to the output file
	InsertRecoveryTag();
	// Add the current tag to ostream
	LabBinaryUtils::WriteValueToStream(this->cur_rec_tag_val, ostream);
}

void LabOutputer::UnBinarize(ifstream *istream) {
	// Read the recovery tag from istream
	LabBinaryUtils::ReadValueFromStream(this->cur_rec_tag_val, istream);
	// Go to this point in the output file
	if (GotoLastRecoveryTag())
		cout << "Recovery point found - id : " + this->cur_rec_tag_val;
}

void LabOutputer::InsertRecoveryTag(/*const string rec_tag = NULL*/)
{
	// Generate a unique recovery tag
	this->cur_rec_tag_val++;
	stringstream full_tag;
	full_tag << TAG_BEGIN << this->cur_rec_tag_val << TAG_END << endl;
	// Insert it into the output file
	InsertText(full_tag.str());
}

bool LabOutputer::GotoLastRecoveryTag(/*const string rec_tag*/)
{
	bool recovered = false;		// If the current output file has been recovered/truncated

	// Copy the output file line by line until the recovery tag is reached
	// (keeping the line containing the tag)
	stringstream full_tag;
	full_tag << TAG_BEGIN << this->cur_rec_tag_val << TAG_END;
	string temp_file_name = this->GetFilename() + ".tmp";

	CloseFile();
	ifstream *f_in = new ifstream(this->GetFilename().c_str(), ios::in);
	ofstream *f_out_tmp = new ofstream(temp_file_name.c_str(), ios::app | ios::out);

	string a_line;
	do
	{
		if (!f_in->eof())
		{
			std::getline(*f_in, a_line);
			*f_out_tmp << a_line << endl;
		}
	}
	while (a_line != full_tag.str() && !f_in->eof());

	f_out_tmp->close();

	// Then erase the current output file with the truncated one
	if (!f_in->eof()) {
		remove(this->GetFilename().c_str());
		rename(temp_file_name.c_str(), this->GetFilename().c_str());
		recovered = true;
	}
	return recovered;
}

//void LabOutputer::SetCurrentRecoveryTag(string rec_tag)
//{
//	this->cur_rec_tag = rec_tag;
//}

void LabOutputer::DrawProgressBar(int len, double percent, bool erase, bool text_only) {
	if (erase)
	{
		cout << "\x1B[2K"; // Erase the entire current line.
		cout << "\x1B[0E"; // Move to the beginning of the current line.
	} else {
		cout << endl;
	}
	string progress;
	for (int i = 0; i < len; ++i) {
		if (i < static_cast<int>(len * percent)) {
			progress += "=";
		}else if (i == (static_cast<int>(len * percent))) {
			progress += ">";
		}
		else {
			progress += "-";
		}
	}
	if (!text_only) cout << "[" << progress << "] "; else cout << "Progress... ";
	cout << (static_cast<int>(floor(0.5 + 100.0 * percent))) << "%";
	flush(cout); // Required.
}

}  // namespace elps




