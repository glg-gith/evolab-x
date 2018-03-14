/*
 * LabSimulatorTime.h
 *
 *  Created on: 14 sept. 2012
 *      Author: golgauth
 */

#ifndef LABSIMULATORTIME_H_
#define LABSIMULATORTIME_H_

#include <LabSimulatorBase.h>
#include <LabActionStack.h>

#include <math.h>

#include "gsl/gsl_rng.h"
#include <LabRngUtils.h>


namespace elps {

/**
 * A basic <b>TIME based</b> subclass of LabSimulatorBase.
 *
 * By default the time evolves continuously as follows :
 *
 * 	t += -log(p) / ter
 * 	where "p" is a random float number [0.0, 1.0] and "ter" is the current total event rate.
 *
 * The way the time is computed / evolves can be customized by overriding the hook TimeStep()
 * (See LabSimulatorTime::TimeStep() for more details).
 *
 * This class comes with additional dedicated hooks to be overridden.
 * (See ResetTime(), TimeStep(), FireEvent(), ... for an exhaustive list).
 *
 * <b>Note :</b>
 *
 * 	We will often use the term "event(s) rate(s)" in time based contexts, since
 * 	this class provides a canvas for firing events accordingly to time during the
 * 	simulation. By default in LabSimulatorTime, the events are fired relatively
 * 	to their rate (or propensity to occur).
 *
 * Of course, the default event based behavior of this class can be customized by overriding
 * the appropriate hooks.
 *
 * SPECIFICITIES :
 * ===============
 *
 * The default inner code of RunSimulation() is an override of the LabSimulatorBase::RunSimulation() hook.
 *
 * This hook can be overridden, but by default it consists in a re-implementation
 * of LabSimulatorBase::RunSimulation() for the need of a TIME based process.
 *
 * The only differences with base version are :
 *
 * - We add a reset of the time properties (ResetTime()) each time we call a new run
 * (right after the InitSimulation() call)
 * - This method is no more responsible for managing the output, since
 * StepSimulation() is
 *
 * \code
 * 	// Default simulation loop behavior (time based version)
 * 	void LabSimulatorTime::RunSimulation()
 * 	{
 * 		InitSimulation();
 * 		ResetTime();                    // -- ADDED --
 *
 * 		InitUnBinarize();
 *
 * 		BeforeRun();
 * 		do
 * 		{
 * 			// OutputSimulation();      // -- REMOVED --
 * 			BeforeStep();               // 'UpdateEventsRate()' must be called in there !!
 * 			StepSimulation();
 * 			AfterStep();
 * 			StepBinarize();
 * 		}
 * 		while (!EndSimulation());
 * 		OutputSimulation();             // One more time for final state
 * 		AfterRun();
 *
 * 	}
 * \endcode
 *
 * <b>Note :</b> Also be aware that the total events rate is theoretically in constant evolution
 * and by the way, has to be updated in the stepping process.
 *
 * StepSimulation() method is specialized too, and is very important : this is where the
 * decisions for firing new events are taken. Consequently, you can keep it "AS IS" but you
 * will have to override the FireEvent() hook, because this is where everything happen.
 *
 * COMMON USAGE :
 * ==============
 *
 * - Create a subclass of L{PyLabSimulatorTime}
 * - Feel free to override the hooks (virtual) functions fitting your needs
 *
 */
class LabSimulatorTime: public LabSimulatorBase {
public:
	LabSimulatorTime();

#ifndef NOPYTHON
	/**
	 * Cython usage only : DO NOT USE !
	 * @param obj The PyLabSimulatorTime Python object related to this
	 */
	LabSimulatorTime(PyObject *obj);
#endif

	virtual ~LabSimulatorTime();

	// TODO : many of those methods should be in the 'protected' scope !!!

	// Additional features
	/**
	 * Reset the current time of the simulation, by reinitializing all the properties related to
	 * time.
	 */
	virtual void ResetTime();
	/**
	 * Define the way the time should evolve. The default is :<br/>
	 *
	 * 	t += -log(p)/ter
	 * 	where "p" is a random float number [0.0, 1.0] and "ter" is the current total event rate.
	 *
	 * @return The delta of time (starting from the current time t) that should elapse before the
	 *         next occurrence of an event (for short : FireEvent() will be called at time = t + StepTime()).
	 */
    virtual double TimeStep();					// The function for time (ex. "t += -log(prob)/tot_evt_rate;")
    /**
     * Hook where things happen. This hook is designed to be overridden by the user. Anything
     * can happen in here, depending on the behavior required in this specific instance of simulation.
     */
    virtual void FireEvent() {};// = 0; // Should be 'pure' we need it callable in Cython

    // Reimplemented features
    /**
     * This hook can be overridden, but by default it consists in a re-implementation
     * of LabSimulatorBase.StepSimulation() for the need of a TIME based process.<br/>
     *
     * It fires events (FireEvent()) every TimeStep() and takes care of writing
     * the output (by calling the inherited hook OutputSimulation()).<br/>
     *
     * <b>Note :</b> the step of time for writing the output can be specified using SetOutputTimeStep()
     *
     * Here's StepSimulation()'s default behavior:
     *
     * \code
     * void LabSimulatorTime::StepSimulation()
     * {
     *
     * 	// If time for output
     * 	if(t_nextout <= t_nextevent && t_nextout <= t_end)
     * 	{
     * 		t = t_nextout;
     *
     * 		OutputSimulation();                     // OutputSimulation() fired every "t_outstep"
     * 		                                        // Feel free to override it...
     * 		t_nextout = t + t_outstep;
     * 	}
     *
     * 	// If time for firing event
     * 	if(t_nextevent <= t_nextout && t_nextout <= t_end)
     * 	{
     * 		t = t_nextevent;
     *
     * 		FireEvent();                            // FireEvent() fired every "TimeStep()"
     * 		                                        // Feel free to override it...
     * 		nb_fired_events++;
     *
     * 		t_nextevent = t + TimeStep();
     * 	}
     *
     * 	// End reached = force t
     * 	if(t_nextout > t_end) t = t_end;
     *
     * }
     * \endcode
     *
     */
    virtual void StepSimulation();
    /**
     * This hook can be overridden, but by default it consists in a re-implementation
     * of LabSimulatorBase.EndSimulation() for the need of a TIME based process.<br/>
     *
     * It ends when the timeout (fixed using SetEndTime()) is reached.
     *
     * @return True if the end time is reached.
     */
    virtual bool EndSimulation();
    /**
     * This hook can be overridden, but by default it consists in a re-implementation
     * of LabSimulatorBase.RunSimulation() for the need of a TIME based process.<br/>
     *
     * The only differences with base version are :
     * - We add a reset of the time properties (ResetTime()) each time we call a new run
     * (right after the InitSimulation() call)
     * - This method is no more responsible for managing the output, since
     * StepSimulation() is
     *
     * \code
     * 	// Default behavior
     * 	void LabSimulatorTime::RunSimulation()
	 * 	{
	 * 		InitSimulation();
	 * 		ResetTime();                    // -- ADDED --
	 *
	 * 		InitUnBinarize();
	 *
	 * 		BeforeRun();
	 * 		do
	 * 		{
	 * 			// OutputSimulation();      // -- REMOVED --
	 * 			BeforeStep();               // 'UpdateEventsRate()' must be called in there !!
	 * 			StepSimulation();
	 * 			AfterStep();
	 * 			StepBinarize();
	 * 		}
	 * 		while (!EndSimulation());
	 * 		OutputSimulation();             // One more time for final state
	 * 		AfterRun();
     * 	}
     * \endcode
     *
     * <b>Note :</b> Also be aware that the total events rate is theoretically in constant evolution
     *        and by the way, has to be updated in the stepping process.
     *
     */
    virtual void RunSimulation();

    //...
    /**
     * Update the total events rate (value used to compute the "time to next event" - See TimeStep()).
     * @param rate The new total events rate.
     */
	void UpdateEventsRate(double rate);
    /**
     * Set the maximum time for the simulation (better use a value related to the way chosen to
     * compute time - See TimeStep()).
     * @param end_time The final time expressed relatively to the TimeStep() method's inner algorithm.
     */
    void SetEndTime(double end_time);
    /**
     * Set step of time for writing the output (better use a value related to the way chosen to
     * compute time - See TimeStep()).
     * @param out_time_step The time step expressed relatively to the TimeStep() method's inner algorithm.
     */
    void SetOutputTimeStep(double out_time_step);
    /**
     * Get the current time of this simulation.
     * @return The current time expressed relatively to the TimeStep() method's inner algorithm.
     */
    double GetTime();
    /**
     * Force the current time (useful when starting the simulation from a given time instead of from
     * the beginning - when recovering from an interruption, by example).
     * @param time The time at which to start expressed relatively to the TimeStep() method's inner algorithm.
     */
    void SetTime(double time);
    /**
     * Get the number of events having been fired since the simulation has begun.
     * @return A number of events.
     */
    int GetNbFiredEvents();

    /**
     * Get the action stack. Can be useful for firing actions from the global environment scope.
     * @return An action stack.
     */
    LabActionStack *GetActionStack();


	//// Cython !!!
#ifndef NOPYTHON

//	PyObject * CyGetPyObj();
//	void CySetPyObj(PyObject *obj);

	// Additional features
	virtual void CyResetTime();
    virtual double CyTimeStep();
    virtual void CyFireEvent();			// Should be pure virtual,
    									// but ALabSimulatorTime is to be instantiated
    // Reimplemented features
    virtual void CyStepSimulation();
    virtual bool CyEndSimulation();
    virtual void CyRunSimulation();

    // Pasted from Base //
	// To be absolutely overridden :
	virtual void CyInitSimulation();
	virtual void CyBeforeStep();
	virtual void CyAfterStep();
	virtual void CyOutputSimulation();

	virtual void CyInitUnBinarize();
	virtual void CyStepBinarize();

	// Override it only if the default behaviour doesn't match your needs :
	virtual void CyBeforeRun();
	virtual void CyAfterRun();
	////virtual void Run();

	// Override it only if you need the binary features
	virtual void CyBinarizeAll();
	virtual void CyUnBinarizeAll();


#endif

private:

    /** The current time - expressed relatively to the TimeStep() method's inner algorithm */
    double t;
    /** The end time - expressed relatively to the TimeStep() method's inner algorithm */
    double t_end;
    /** The time at which the next event will be fired - expressed relatively to the TimeStep() method's inner algorithm */
    double t_nextevent;
    /** The time at which the next output will be dumped - expressed relatively to the TimeStep() method's inner algorithm */
    double t_nextout;
    /** The delta of time between two output dumps - expressed relatively to the TimeStep() method's inner algorithm */
    double t_outstep;

    /** Current number of fired events */
    int nb_fired_events;
    /** Current total events rate */
    double tot_evt_rate;

    /** Random number generator - used in the default TimeStep() method's inner algorithm */
    LabRng *r;

    LabActionStack *actionStack;


	void Init();

};




}  // namespace elps

#endif /* LABSIMULATORTIME_H_ */
