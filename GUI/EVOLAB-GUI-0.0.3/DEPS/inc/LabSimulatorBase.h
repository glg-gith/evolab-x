/*
 * LabSimulatorBase.h
 *
 *  Created on: 12 sept. 2012
 *      Author: golgauth
 */

#ifndef LABSIMULATORBASE_H_
#define LABSIMULATORBASE_H_


// Handle stack trace
#ifdef __WIN32__

	#include <windows.h>
	#include <stdio.h>
	#include <imagehlp.h>

	#define WIN32_LEAN_AND_MEAN
	#include <stdlib.h>
	#include <signal.h>

#else

	#include <execinfo.h>
	#include <signal.h>

	#include <errno.h>
	extern char *program_invocation_name;
	extern char *program_invocation_short_name;

	#define MAX_STACK_FRAMES 64
	static void *stack_traces[MAX_STACK_FRAMES];

#endif

// Handle Cython specifics
#ifndef NOPYTHON
	#include <Python.h>
	#include <LabCythonUtils.h>
#endif


//#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>
#include <time.h>


#include <LabConsts.h>

#include <LabIBinarizable.h>
#include <LabFileUtils.h>

#include <LabAttributesStack.h>

#include <LabBindingBase.h>

//using namespace std;

namespace elps {


/**
 * Base class to be derived when writing a simulation.<br/>
 * It provides a layout covering all the basic features required by a
 * simulation. It is based on common hooks (InitSimulation(), StepSimulation(), ...)
 * which can be used (overridden) or not by the user. Most of those hooks are
 * virtual or empty (excepted the one concerning the main loop - have a look at
 * RunSimulation() content to have a good preliminary overview on how the other
 * hooks might be used / organized).
 *
 * <b>Note :</b> The hooks concerning the basic implementation of the binarization
 *        (aka : dump / saving the current state of the simulation) are also pre- written).
 *
 * The inner code of RunSimulation() could look like this :
 *
 * \code
 * 	// Default simulation loop behavior
 * 	void LabSimulatorBase::RunSimulation()
 * 	{
 *		InitSimulation();
 *		InitUnBinarize();
 *		BeforeRun();
 *		do
 *		{
 *			OutputSimulation();
 *			BeforeStep();
 *			StepSimulation();
 *			AfterStep();
 *			StepBinarize();
 *		}
 *		while (! EndSimulation());
 *		OutputSimulation();			        // One more time for final state
 *		AfterRun();
 * 	}
 * \endcode
 *
 *
 * COMMON USAGE :
 * ==============
 *
 * - Create a subclass of LabSimulatorBase
 * - Feel free to override the hooks (virtual) functions fitting your needs
 *
 *
 * All the hooks that where not mentioned above remain empty in this base class,
 * but what they where designed for is described below...
 *
 */
class LIB_CLASS LabSimulatorBase : public LabBindingBase {

public:
	LabSimulatorBase();

#ifndef NOPYTHON
	/**
	 * Cython usage only : DO NOT USE !
	 * @param obj The PyLabSimulatorBase Python object related to this
	 */
	LabSimulatorBase(PyObject *obj);
#endif

	virtual ~LabSimulatorBase();

	/**
	 * Get the number of trials (aka : number of times the simulation must be entirely completed
	 * until the break condition - See EndSimulation() - is reached) for the whole process.
	 * @return The number of times the simulation will be repeated.
	 */
	int GetNbRepetitions();
	/**
	 * Set the number of trials for the whole process.
	 * If ignored, the simulation will run only once (as the default returning value
	 * of EndSimulation() is True, meaning that the inner while loop of the
	 * RunSimulation() hook while break at the end of the first iteration).
	 * @param nb_rep The number of times the simulation will be repeated.
	 */
	void SetNbRepetitions(int nb_rep);
	/**
	 * Catch the index of the iteration currently being run.
	 * @return The index, starting at 0 (index of the first iteration).
	 */
	int GetCurrentRepetition();
	/**
	 * WARNING ! Should not be forced by hand during a run. This functions was designed to
	 * allow recovering the last state of the simulation before interruption of the process
	 * by the user or a crash. See the "UNbinarization" features (UnBinarizeAll()).
	 * Consequently, it should only be used to restart a simulation at a specific point.
	 * @param rep_num The repetition number to restart from.
	 */
	void SetCurrentRepetition(int rep_num);

	// To be absolutely overridden :						// '=0' for 'pure virtual' methods
	/**
	 * This hook is designed to be overridden. It should contain all the instructions/treatments
	 * that will occur only once, at the very beginning of the simulation.
	 */
	virtual void InitSimulation() {};// = 0; // Should be 'pure', but we need it callable in Cython
	/**
	 * This hook is designed to be overridden. It should contain all the instructions/treatments
	 * that will occur at each step of the simulation.
	 */
	virtual void StepSimulation() {};// = 0; // Should be 'pure', but we need it callable in Cython
	/**
	 * This hook is designed to be overridden. It should contain all the instructions/treatments
	 * required before stepping (See StepSimulation())
	 */
	virtual void BeforeStep() { };
	/**
	 * This hook is designed to be overridden. It should contain all the instructions/treatments
	 * required after having stepped (See StepSimulation())
	 */
	virtual void AfterStep() { };
	// Use preferably the 'LabOutputer' utility in this hook
	/**
	 * This hook is designed to be overridden. It should contain all the instructions/treatments
	 * handling the output informations. For example writing intermediate states of the
	 * simulation process into text files.<br/>
	 * <b>Note :</b> See the LabOutputer class for an helper to generate text files (as lines of numbers or
	 *        as histograms)
	 */
	virtual void OutputSimulation() {};// = 0; // Should be 'pure', but we need it callable in Cython
	/**
	 * This hook is designed to be overridden. It takes care of ending the simulation and can
	 * be any kind of condition (by example : a given number of repetitions is reached, a population
	 * entirely passed away, ...).
	 * @return True by default (aka : the simulation steps only once). One must return true to terminate
	 *         the simulation.
	 */
	virtual bool EndSimulation() { return true; };// = 0; // Should be 'pure', but we need it callable in Cython

	/**
	 * Designed to recover from a previously recorded/binarized state of the simulation process.
	 * The default behavior (in this base class) is to try to recover from a given dump file
	 * (if existing - See do_start_from_dump member attribute) or to clean previous dump files
	 * (if do_start_from_dump is set to false).
	 */
	virtual void InitUnBinarize();
	/**
	 * Called at the very end of a simulation step (by default). This hook can be overridden to
	 * deal with any kind of requirement in term of dumping the system. Ignore (aka : do not call it
	 * in RunSimulation()) if you do not plan to use binarization features.
	 */
	virtual void StepBinarize();

	//virtual double StepTime();							// See 'LabSimulatorTime'
	/**
	 * Override this hook only if the default behavior doesn't match your needs.
	 * See the default content in the above header description of this class.
	 */
	virtual void RunSimulation();
	/**
	 * This hook is designed to be overridden. It should contain all the instructions/treatments
	 * that should precede a step of the simulation. By default, it is placed
	 * right before starting the main stepping 'while loop'.
	 */
	virtual void BeforeRun() { };
	/**
	 * This hook is designed to be overridden. It should contain all the instructions/treatments.
	 * that should occur at the very end of the simulation process (aka : the end of RunSimulation()).
	 */
	virtual void AfterRun() { };
	/**
	 * WARNING : This is not a hook. This function is the entry point for running the entire process.<br/>
	 * By default in this base class, it is designed to repeat a given number of times the entire
	 * simulation (See SetNbRepetitions()). This default behavior can be overridden anyway, but
	 * absolutely needs to contain at least one call to RunSimulation().
	 *
	 * \code
	 * 	// Default simulator behavior
	 * 	void LabSimulatorBase::Run()
	 * 	{
	 * 		for (int i=0; i<this->nbRepetitions; i++)
	 * 		{
	 * 			RunSimulation();
	 * 		}
	 * 	}
	 * \endcode
	 */
	virtual void Run();



	/**
	 * Get the global (whole environment scope) attributes.
	 * @return The global environment's stack of attributes.
	 */
	LabAttributesStack *GetEnvAttributesStack();

	/**
	 * Possibility to pass the output and input streams to respectively dump and restore using the
	 * binarization features.
	 * @param ostream The file output stream to write to.
	 * @param istream The file input stream to read from.
	 */
	void SetBinaryStreams(ofstream *ostream, ifstream *istream);
	// Override it only if you need the binary features
	/**
	 * This hook is designed to be overridden. It should contain all the instructions/treatments
	 * required to dump the current state of the system/simulation. Can be ignored : override
	 * it only if you need to binarize/dump to file.<br/>
	 * See also the Binarize() and UnBinarize() methods for all the object classes
	 * implementing the interface LabIBinarizable (including the LabOutputer class - see
	 * OutpuSimulation() hook - which can handle recovery points).
	 */
	virtual void BinarizeAll() { };
	/**
	 * This hook is designed to be overridden. It should contain all the instructions/treatments
	 * required to restore the last dumpped state of the system/simulation. Can be ignored :
	 * override it only if you need to unbinarize/restore from file.<br/>
	 * See also the Binarize() and UnBinarize() methods for all the object classes
	 * implementing the interface LabIBinarizable (including the LabOutputer class - see
	 * OutpuSimulation() hook - which can handle recovery points.
	 */
	virtual void UnBinarizeAll() { };

	/**
	 * Read the penultimate binary dump file (to make sure the integrity of the file we are
	 * working with is preserved, we don't pick up the very last one, as it could have
	 * been corrupted...).
	 *
	 * @return The id of the dump from which the system's state is to be recovered.
	 */
	int GetLastDumpFileIdAndClean();
	/**
	 * Remove the most ancient dump files during the stepping process (See StepBinarize()).
	 */
	void CleanUnusedDumpFile();
	/**
	 * Remove from disk all the previously generated dumps.
	 */
	void CleanAllDumpFiles();


	//---
	/**
	 * Get the delta of time between each dump (when 'do_dump' is set to true).
	 * @return A time interval expressed in seconds (See t_sys_cur).
	 */
	double GetTimeDumpStep();
	/**
	 * Set the interval between each dump (when 'do_dump' is set to true).
	 * @param time Interval of time expressed in seconds.
	 */
	void SetTimeDumpStep(double time);
	/**
	 * Check if the dumping system has been activated.
	 * @return True if it has.
	 */
	bool GetDoDump();
	/**
	 * Set if the dump of the system must be activated.
	 * @param do_it Pass 'true' to activate.
	 */
	void SetDoDump(bool do_it);
	/**
	 * Check if the system must be recovered from dump files.
	 * @return True if it must.
	 */
	bool GetDoStartFromDump();
	/**
	 * Set if the system must be recovered from dump files.
	 * @param do_it Pass 'true' to activate the recovery.
	 */
	void SetDoStartFromDump(bool do_it);



	/**
	 * Requires a preliminary call to SetMaxPopulationSize().
	 * @return Maximum number of SITES handled by the simulator.
	 */
	unsigned int GetMaxPopulationSize();
	/**
	 * Sets the maximum number of sites / individuals to be handled by the simulation.
	 * @param pop_size Should be at least one individual.
	 */
	void SetMaxPopulationSize(int pop_size);


	//// Cython !!!
#ifndef NOPYTHON

	PyObject * CyGetPyObj();
	void CySetPyObj(PyObject *obj);

	// To be absolutely overridden :
	virtual void CyInitSimulation();
	virtual void CyStepSimulation();
	virtual void CyBeforeStep();
	virtual void CyAfterStep();
	virtual void CyOutputSimulation();
	virtual bool CyEndSimulation();

	virtual void CyInitUnBinarize();
	virtual void CyStepBinarize();

	// Override it only if the default behaviour doesn't match your needs :
	virtual void CyRunSimulation();
	virtual void CyBeforeRun();
	virtual void CyAfterRun();
	////virtual void Run();

	// Override it only if you need the binary features
	virtual void CyBinarizeAll();
	virtual void CyUnBinarizeAll();

#endif

protected:

	bool IsCython() { return _is_cython; }


	/** Used to read/write files in binary mode (default : ios::app | ios::out | ios::in | ios::binary) */
	ios_base::openmode bin_filter;

	// TODO : implement getters/setters for those variable & shift them to private scope
	/** Output file stream to dump to */
	ofstream *os;
	/** Input file stream to recover from */
	ifstream *is;
	/** Interval in seconds between two dumps */
	double t_dump_step;
	/** If dumping is activated */
	bool do_dump;
	/** If recovering from dump is activated */
	bool do_start_from_dump;
	/** Prefix for the name of the dump files (default : "dump.bin.") */
	string dump_prefix;
	/** Postfix index to be appended to the name of the dump file */
	int dump_id;
	/** Current repetition index - starting at 0 */
	int cur_rep;

private:

	//// Cython !!!
	bool _is_cython;
	#ifndef NOPYTHON
		PyObject *m_obj;
	#endif

	/** Number of time the same simulation is planned to be repeated */
	int nbRepetitions;

	/**
	 * Stores the current time of our binarization system. It is updated to the
	 * computer clock each time a dump is generated (See StepBinarize() inner code).
	 */
	double t_sys_cur;

	LabAttributesStack *envAttributesStack;

	unsigned int max_pop_size_;

	void Init(bool is_cython);



	//!
	//// Handle stack trace
#ifdef __WIN32__
	static LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS * ExceptionInfo)
	{
		switch(ExceptionInfo->ExceptionRecord->ExceptionCode)
		{
		case EXCEPTION_ACCESS_VIOLATION:
			fputs("Error: REGULAR_HANDLED_EXCEPTION or EXCEPTION_ACCESS_VIOLATION\n", stderr);
			break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			fputs("Error: EXCEPTION_ARRAY_BOUNDS_EXCEEDED\n", stderr);
			break;
		case EXCEPTION_BREAKPOINT:
			fputs("Error: EXCEPTION_BREAKPOINT\n", stderr);
			break;
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			fputs("Error: EXCEPTION_DATATYPE_MISALIGNMENT\n", stderr);
			break;
		case EXCEPTION_FLT_DENORMAL_OPERAND:
			fputs("Error: EXCEPTION_FLT_DENORMAL_OPERAND\n", stderr);
			break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			fputs("Error: EXCEPTION_FLT_DIVIDE_BY_ZERO\n", stderr);
			break;
		case EXCEPTION_FLT_INEXACT_RESULT:
			fputs("Error: EXCEPTION_FLT_INEXACT_RESULT\n", stderr);
			break;
		case EXCEPTION_FLT_INVALID_OPERATION:
			fputs("Error: EXCEPTION_FLT_INVALID_OPERATION\n", stderr);
			break;
		case EXCEPTION_FLT_OVERFLOW:
			fputs("Error: EXCEPTION_FLT_OVERFLOW\n", stderr);
			break;
		case EXCEPTION_FLT_STACK_CHECK:
			fputs("Error: EXCEPTION_FLT_STACK_CHECK\n", stderr);
			break;
		case EXCEPTION_FLT_UNDERFLOW:
			fputs("Error: EXCEPTION_FLT_UNDERFLOW\n", stderr);
			break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			fputs("Error: EXCEPTION_ILLEGAL_INSTRUCTION\n", stderr);
			break;
		case EXCEPTION_IN_PAGE_ERROR:
			fputs("Error: EXCEPTION_IN_PAGE_ERROR\n", stderr);
			break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			fputs("Error: EXCEPTION_INT_DIVIDE_BY_ZERO\n", stderr);
			break;
		case EXCEPTION_INT_OVERFLOW:
			fputs("Error: EXCEPTION_INT_OVERFLOW\n", stderr);
			break;
		case EXCEPTION_INVALID_DISPOSITION:
			fputs("Error: EXCEPTION_INVALID_DISPOSITION\n", stderr);
			break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			fputs("Error: EXCEPTION_NONCONTINUABLE_EXCEPTION\n", stderr);
			break;
		case EXCEPTION_PRIV_INSTRUCTION:
			fputs("Error: EXCEPTION_PRIV_INSTRUCTION\n", stderr);
			break;
		case EXCEPTION_SINGLE_STEP:
			fputs("Error: EXCEPTION_SINGLE_STEP\n", stderr);
			break;
		case EXCEPTION_STACK_OVERFLOW:
			fputs("Error: EXCEPTION_STACK_OVERFLOW\n", stderr);
			break;
		default:
			fputs("Error: Unrecognized Exception\n", stderr);
			break;
		}
		fflush(stderr);

		/* If this is a stack overflow then we can't walk the stack, so just show
		where the error happened */
		if (EXCEPTION_STACK_OVERFLOW != ExceptionInfo->ExceptionRecord->ExceptionCode)
		{
			windows_print_stacktrace(ExceptionInfo->ContextRecord);
		}
		else
		{
			addr2line(__argv[0], (void*)ExceptionInfo->ContextRecord->Eip);
		}

		return EXCEPTION_EXECUTE_HANDLER;
	}

	static BOOL WINAPI ConsoleHandler(DWORD dwType)
	{
	    switch(dwType) {
	    case CTRL_C_EVENT:
	        printf("Program aborted, ctrl+c detected\n");
	        exit(0);
	        break;
	    case CTRL_BREAK_EVENT:
	        printf("Program aborted, break detected\n");
	        exit(0);
	        break;
	    default:
	        printf("Some other event\n");
	    }
	    return TRUE;
	}


#else
	static void posix_exception_handler(int sig)
	{
		// Catch exceptions
		switch(sig)
		{
		case SIGABRT:
			fputs("Caught SIGABRT: usually caused by an abort() or assert()\n", stderr);
			break;
		case SIGFPE:
			fputs("Caught SIGFPE: arithmetic exception, such as divide by zero\n",
					stderr);
			break;
		case SIGILL:
			fputs("Caught SIGILL: illegal instruction\n", stderr);
			break;
		case SIGINT:
			fputs("Caught SIGINT: interactive attention signal, probably a ctrl+c\n",
					stderr);
			break;
		case SIGSEGV:
			fputs("Caught SIGSEGV: segfault\n", stderr);
			break;
		case SIGTERM:
		default:
			fputs("Caught SIGTERM: a termination request was sent to the program\n",
					stderr);
			break;
		}

		// Print stacktrace (Ctrl+C interrupt => No backtrace)
		if (sig != (int)SIGINT)
		{
			// print out all the frames to stderr
			fprintf(stderr, "Error: signal %d:\n", sig);
			//backtrace_symbols_fd(array, size, 2);
			posix_print_stack_trace();
		}
		exit(sig);

	}
#endif

	static void set_signal_handler() {
#ifdef __WIN32__
		SetUnhandledExceptionFilter(windows_exception_handler);
		SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler,TRUE);
#else
		signal(SIGABRT, posix_exception_handler);
		signal(SIGFPE,  posix_exception_handler);
		signal(SIGILL,  posix_exception_handler);
		signal(SIGINT,  posix_exception_handler);
		signal(SIGSEGV, posix_exception_handler);
		signal(SIGTERM, posix_exception_handler);
#endif
	}

#ifdef __WIN32__
	static void windows_print_stacktrace(CONTEXT* context)
	{
		SymInitialize(GetCurrentProcess(), 0, true);

		STACKFRAME frame = { 0 };

		/* setup initial stack frame */
		frame.AddrPC.Offset         = context->Eip;
		frame.AddrPC.Mode           = AddrModeFlat;
		frame.AddrStack.Offset      = context->Esp;
		frame.AddrStack.Mode        = AddrModeFlat;
		frame.AddrFrame.Offset      = context->Ebp;
		frame.AddrFrame.Mode        = AddrModeFlat;

		while (StackWalk(IMAGE_FILE_MACHINE_I386 ,
				GetCurrentProcess(),
				GetCurrentThread(),
				&frame,
				context,
				0,
				SymFunctionTableAccess,
				SymGetModuleBase,
				0 ) )
		{
			addr2line(__argv[0], (void*)frame.AddrPC.Offset);
		}

		SymCleanup( GetCurrentProcess() );
	}

#else

	static void posix_print_stack_trace()
	{
		int i, trace_size = 0;
		char **messages = (char **)NULL;

		trace_size = backtrace(stack_traces, MAX_STACK_FRAMES);
		messages = backtrace_symbols(stack_traces, trace_size);

		/* skip the first couple stack frames (as they are this function and
		 our handler) and also skip the last frame as it's (always?) junk. */
		// for (i = 3; i < (trace_size - 1); ++i)
		// we'll use this for now so you can see what's going on
		//for (i = 0; i < trace_size; ++i)
		for (i = 3; i < trace_size-1; ++i)
		{
			if (addr2line(program_invocation_name, stack_traces[i]) != 0)
			{
				printf("  error determining line # for: %s\n", messages[i]);
			}
		}
		if (messages) { free(messages); }
	}


#endif


	/* Resolve symbol name and source location given the path to the executable
	   and an address */
	static int addr2line(char const * const program_name, void const * const addr)
	{
		char addr2line_cmd[512] = {0};

		/* have addr2line map the address to the relent line in the code */
		sprintf(addr2line_cmd,"addr2line -C -f -p -e %.256s %p", program_name, addr);

		/* This will print a nicely formatted string specifying the
	     function and source line of the address */
		return system(addr2line_cmd);
	}


};


}  // namespace elps

#endif /* LABSIMULATORBASE_H_ */
