/*
 * LabSimulatorBase.cpp
 *
 *  Created on: 12 sept. 2012
 *      Author: golgauth
 */

#include "LabSimulatorBase.h"


namespace elps {


LabSimulatorBase::LabSimulatorBase() {
	Init(false);
}

#ifndef NOPYTHON
LabSimulatorBase::LabSimulatorBase(PyObject *obj) {
	Init(true);
	this->m_obj = obj;
}
#endif

void LabSimulatorBase::Init(bool is_cython) {
	_is_cython = is_cython;

	// Stack trace handling...
	//!
#ifdef __WIN32__
	set_signal_handler();
#else
	set_signal_handler();
#endif


	bin_filter = ios::app | ios::out | ios::in | ios::binary;
	dump_prefix = "dump.bin.";
	cur_rep = 0;


#ifndef NOPYTHON
	//#define ALLOWCYTHON 1
	this->m_obj = NULL;
#else
	//#define ALLOWCYTHON 0
#endif

}

LabSimulatorBase::~LabSimulatorBase() {
	// TODO Auto-generated destructor stub
}


int LabSimulatorBase::GetNbRepetitions() {
	return this->nbRepetitions;
}

void LabSimulatorBase::SetNbRepetitions(int nb_rep) {
	this->nbRepetitions = nb_rep;
}

int LabSimulatorBase::GetCurrentRepetition()
{
	return cur_rep;
}

void LabSimulatorBase::SetCurrentRepetition(int rep_num)
{
	cur_rep = rep_num;
}

// Default simulation loop behaviour
void LabSimulatorBase::RunSimulation()
{
	if (!this->_is_cython) {
		InitSimulation();

		InitUnBinarize();

		BeforeRun();
		do
		{
			OutputSimulation();
			BeforeStep();
			StepSimulation();
			AfterStep();
			StepBinarize();
		}
		while (!EndSimulation());
		OutputSimulation();			// One more time for final state
		AfterRun();
	}
	else
	{
#ifndef NOPYTHON
		CyInitSimulation();

		CyInitUnBinarize();

		CyBeforeRun();
		do
		{
			CyOutputSimulation();
			CyBeforeStep();
			CyStepSimulation();
			CyAfterStep();
			CyStepBinarize();
		}
		while (!CyEndSimulation());
		CyOutputSimulation();			// One more time for final state
		CyAfterRun();
#endif
	}

	cur_rep++;
}

// Default simulator behaviour
void LabSimulatorBase::Run()
{
	for (int i=0; i<this->nbRepetitions; i++)
	{
		if (!this->_is_cython) {
			RunSimulation();
		}
		else
		{
#ifndef NOPYTHON
			CyRunSimulation();
#endif
		}
	}
}




LabAttributesStack *LabSimulatorBase::GetEnvAttributesStack() {
	return envAttributesStack;
}


void LabSimulatorBase::SetBinaryStreams(ofstream *ostream, ifstream *istream)
{
	os = ostream;
	is = istream;
}

//void LabSimulatorBase::SetBinaryStreams(string s_ostream, string s_istream)
//{
//	os = new ofstream(s_ostream.c_str());
//	is = new ifstream(s_istream.c_str());
//}
//
//void LabSimulatorBase::SetBinaryStreams()
//{
//	os = new ofstream();
//	is = new ifstream();
//}


void LabSimulatorBase::InitUnBinarize()
{
	struct timeval tv_cur;
	gettimeofday(&tv_cur, NULL);
	t_sys_cur = tv_cur.tv_sec;
	stringstream ss;

	if (do_start_from_dump)
	{
		// Read the penultimate binary dump file (to make sure the integrity
		// of the file we are working with is preserved, we don't pick up the
		// very last one, as it could have been corrupted...)
		dump_id = GetLastDumpFileIdAndClean();
		if (dump_id != -1)
		{
			ss << dump_prefix << dump_id;
			cout << "Reloading interrupted simulation from dump : \"" << ss.str() << "\" ..." << endl;
			is->open(ss.str().c_str(), bin_filter);
			UnBinarizeAll();
			is->close();
		}
		else
		{
			cerr << "Error: Recovery from dump files failed !" << endl;
			exit(-1);
		}
	}
	else
	{
		CleanAllDumpFiles();
		dump_id = 0;
	}
}



void LabSimulatorBase::StepBinarize()
{
	struct timeval tv_cur;
	gettimeofday(&tv_cur, NULL);
	double t_sys = tv_cur.tv_sec;
	stringstream ss;

	if (do_dump && t_sys > t_sys_cur + t_dump_step)
	{
		dump_id++;
		ss << dump_prefix << dump_id;
		cout << "[Add] >>> " << ss.str() << endl;
		os->open(ss.str().c_str(), bin_filter);
		BinarizeAll();
		os->close();
		CleanUnusedDumpFile();
		t_sys_cur = t_sys;
	}

}

//void LabSimulatorBase::OpenBinaryIOStreams(ofstream *ostream, ifstream *istream)
//{
//	os = ostream;
//	is = istream;
//
//	os->open("backup.bin.0", bin_filter);
//	is->open("backup.bin.1", bin_filter);
//}
//
//void LabSimulatorBase::CloseBinaryIOStreams(ofstream *ostream, ifstream *istream)
//{
//	os->close();
//	is->close();
//}

// Read the penultimate binary dump file (to make sure the integrity
// of the file we are working with is preserved, we don't pick up the
// very last one, as it could have been corrupted...)
int LabSimulatorBase::GetLastDumpFileIdAndClean()
{
	// List the dump files (the ones starting with "dump.bin.")
	vector<string> dump_files;
	LabFileUtils::ListDir(".", dump_files, true, dump_prefix);

	string file;

	// No dump file => abort !
	if (dump_files.size() == 0)
	{
		cout << "Error: No dump file to load from !" << endl;
		return -1;
	}
	// Try reloading from unique & possibly corrupted file
	else if (dump_files.size() == 1)
	{
		file = dump_files[0];
	}
	// Reload from secure penultimate dump file
	else
	{
		for (unsigned int i=0; i<dump_files.size(); i++)
			cout << ">> Found dump file : " << dump_files[i] << endl;
		file = dump_files[dump_files.size()-2];
		// Remove most recent backup as soon as the secure one is identified
		cout << "[Rem] >>> " << dump_files[dump_files.size()-1] << endl;
		remove(dump_files[dump_files.size()-1].c_str());
	}

	// Return the penultimate file's id
	int dot_pos = file.find_last_of('.');
	string s_id = file.substr(dot_pos + 1, file.length() - dot_pos);
	if (s_id != "")
	{
		cout << "RECOVERY from dump file of id : '" << s_id << "' ..." << endl;
		int id;
		istringstream iss(s_id);
		iss >> id;
		return id;
	}
	else
	{
		return -1;
	}
}

// Remove unused dump file while stepping
void LabSimulatorBase::CleanUnusedDumpFile()
{
	vector<string> dump_files;
	LabFileUtils::ListDir(".", dump_files, true, dump_prefix);
	if (dump_files.size() > 2)
	{
		cout << "[Rem] >>> " << dump_files[0] << endl;
		remove(dump_files[0].c_str());
	}
}

// Remove all dump files (the ones starting with "dump.bin.")
void LabSimulatorBase::CleanAllDumpFiles()
{
	vector<string> dump_files;
	LabFileUtils::ListDir(".", dump_files, true, dump_prefix);
	for (unsigned int i=0; i<dump_files.size(); i++)
	{
		cout << "[Rem] >>> " << dump_files[i] << endl;
		remove(dump_files[i].c_str());
	}
}

double LabSimulatorBase::GetTimeDumpStep()
{
	return this->t_dump_step;
}

void LabSimulatorBase::SetTimeDumpStep(double time)
{
	this->t_dump_step = time;
}

bool LabSimulatorBase::GetDoDump()
{
	return this->do_dump;
}

void LabSimulatorBase::SetDoDump(bool do_it)
{
	this->do_dump = do_it;
}

bool LabSimulatorBase::GetDoStartFromDump()
{
	return this->do_start_from_dump;
}

void LabSimulatorBase::SetDoStartFromDump(bool do_it)
{
	this->do_start_from_dump = do_it;
}




unsigned int LabSimulatorBase::GetMaxPopulationSize() {

	if (max_pop_size_ <= 0) LabConsts::E(LabConsts::BAD_PREV_POP_ALLOCATION);

	return max_pop_size_;
}

void LabSimulatorBase::SetMaxPopulationSize(int pop_size) {

	if (pop_size <= 0) LabConsts::E(LabConsts::BAD_POP_ALLOCATION);

	max_pop_size_ = pop_size;

	SetMaxSize(max_pop_size_);
}




//// Cython !!!
#ifndef NOPYTHON

PyObject * LabSimulatorBase::CyGetPyObj() {
	return this->m_obj;
}

void LabSimulatorBase::CySetPyObj(PyObject *obj) {
	//if (obj == NULL) { this->m_obj = NULL; return; }
	//if (this->m_obj) Py_XDECREF(this->m_obj);
	this->m_obj = obj;
	//Py_XINCREF(this->m_obj);
}

void LabSimulatorBase::CyInitSimulation()
{
//	if (this->m_obj) {
//		int error;
//		cy_call_func_void(this->m_obj, (char*)"InitSimulation", &error);
//		if (error) LabSimulatorBase::InitSimulation();
//	}
	if (this->m_obj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(this->m_obj, "InitSimulation", error);
		if (error) LabSimulatorBase::InitSimulation();
	}
}

void LabSimulatorBase::CyStepSimulation()
{
	if (this->m_obj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(this->m_obj, "StepSimulation", error);
		if (error) LabSimulatorBase::StepSimulation();
	}
}

void LabSimulatorBase::CyBeforeStep()
{
	if (this->m_obj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(this->m_obj, "BeforeStep", error);
		if (error) LabSimulatorBase::BeforeStep();
	}
}

void LabSimulatorBase::CyAfterStep()
{
	if (this->m_obj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(this->m_obj, "AfterStep", error);
		if (error) LabSimulatorBase::AfterStep();
	}
}

void LabSimulatorBase::CyOutputSimulation()
{
	if (this->m_obj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(this->m_obj, "OutputSimulation", error);
		if (error) LabSimulatorBase::OutputSimulation();
	}
}

bool LabSimulatorBase::CyEndSimulation()
{
	if (this->m_obj) {
		bool error;
		bool result = LabCythonUtils::CyCallFuncBool(this->m_obj, "EndSimulation", error);
		if (error) result = LabSimulatorBase::EndSimulation();
		return result;
	}
	return true;
}

void LabSimulatorBase::CyInitUnBinarize()
{
	if (this->m_obj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(this->m_obj, "InitUnBinarize", error);
		if (error) LabSimulatorBase::InitUnBinarize();
	}
}

void LabSimulatorBase::CyStepBinarize()
{
	if (this->m_obj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(this->m_obj, "StepBinarize", error);
		if (error) LabSimulatorBase::StepBinarize();
	}
}

void LabSimulatorBase::CyRunSimulation()
{
	if (this->m_obj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(this->m_obj, "RunSimulation", error);
		if (error) LabSimulatorBase::RunSimulation();
	}
}

void LabSimulatorBase::CyBeforeRun()
{
	if (this->m_obj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(this->m_obj, "BeforeRun", error);
		if (error) LabSimulatorBase::BeforeRun();
	}
}

void LabSimulatorBase::CyAfterRun()
{
	if (this->m_obj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(this->m_obj, "AfterRun", error);
		if (error) LabSimulatorBase::AfterRun();
	}
}
//
//void LabSimulatorBase::Run()
//{
//	if (this->m_obj) {
//		int error;
//		cout << "Runnin'..." << endl;
//		cy_call_func_void(this->m_obj, (char*)"Run", &error);
//		if (error) LabSimulatorBase::Run();
//	}
//}

void LabSimulatorBase::CyBinarizeAll()
{
	if (this->m_obj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(this->m_obj, "BinarizeAll", error);
		if (error) LabSimulatorBase::BinarizeAll();
	}
}

void LabSimulatorBase::CyUnBinarizeAll()
{
	if (this->m_obj) {
		bool error;
		LabCythonUtils::CyCallFuncVoid(this->m_obj, "UnBinarizeAll", error);
		if (error) LabSimulatorBase::UnBinarizeAll();
	}
}

#endif


}  // namespace elps



