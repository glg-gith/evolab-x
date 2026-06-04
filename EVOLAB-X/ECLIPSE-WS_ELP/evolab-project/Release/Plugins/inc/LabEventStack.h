/*
 * LabEvent.h
 *
 *  Created on: 13 sept. 2012
 *      Author: golgauth
 */

#ifndef LABEVENT_H_
#define LABEVENT_H_

#include <iostream>
#include <string>
#include <vector>

#include <gsl/gsl_rng.h>


#include <LabConsts.h>

#include <LabIBinarizable.h>
#include <LabBinaryUtils.h>
#include <LabCallBack.h>
#include <LabCallBack2.h>


using namespace std;


namespace elps {

/**
 * Definition of the prototype for the callback function that will
 * encapsulate the instructions for updating the Event's rates
 *
 * @param param (void*) implies we are free to pass any object of any complexity
 *              The param will have to be cast inside the given function to be used properly
 * @return Has to return a double. Feel free to return 0.0 if no return value is required
 */
typedef double (*RATE_CALLBACK_FUNC)(void* param);
/**
 * Definition of the prototype for the callback function that will
 * encapsulate the instructions to be executed when the Event is fired
 *
 * @param param (void*) implies we are free to pass any object of any complexity
 *              The param will have to be cast inside the given function to be used properly
 * @return Has to return a double. Feel free to return 0.0 if no return value is required
 */
typedef double (*REALIZE_CALLBACK_FUNC)(void* param);


/**
 * Pointer to any kind of object to be used in a callback
 */
typedef void* EventCallBackParam;
/**
 * Callback function called for updating the event's rates
 */
typedef LabCallBack2 RateCallBack;
/**
 * Callback function called for realizing the instructions related to a given event
 */
typedef LabCallBack2 RealizeCallBack;


/**
 * Stack of events that can occur at a given time (depending on the rate of each event)
 * in a TIME based simulation context.<br/>
 *
 * EVENTS :<br/>
 * An event is an action that can be fired at any time on the SITE.
 * 		* It has a given propensity to occur (which can change during the simulation life time)
 * 		* It comes with an action (method) which is fired (executed) when the event occurs
 *
 * USAGE :
 * =======
 *
 * - Instantiate
 * - Push event(s) (PushEvent() function)
 * - RealizeEvent() is used to fire an event
 * 	 (the current time of the event stack is updated during this operation)
 * 		- Create a function of the form "double func_name(void* param)"
 * 		  for the update of the event's rate
 * 		- Create a function of the form "double func_name(void* param)"
 * 		  to be executed when the event is fired
 * - The events rates MUST then be updated using the UpdateStackRates() function
 *
 * <b>Sample :</b><br/><br/>
 * First of all, get yourself an event stack :
 *
 * \code
 * 	LabEventStack *eventStack = new LabEventStack()
 * \endcode
 *
 * Secondly, define the method to be fired. This has to be done in two steps :
 * - The <b>rate</b> callback
 * - The <b>realize</b> callback
 *
 * <br/>
 *
 * \code
 * 	double MyCallbackRate(void *param)
 * 	{
 * 		cout << "CB fired !" << endl;
 * 		...
 * 		...                                         // DO SOMETHING with "param" (or not) ...
 * 		...
 * 		rate = 12.0;                                // Recompute the rate (constant in this ex.)
 * 		return rate;
 * 	}
 * 	...
 * 	double MyCallbackRealize(void *param)
 * 	{
 * 		cout << "CB fired !" << endl;
 * 		// DO SOMETHING with "param" here...
 * 		return 0.0;
 * 	}
 * \endcode
 *
 * <b>Note :</b> the <b>param</b> parameter can be any object (all you need is to cast it into the appropriate type).
 *
 * Then we can create the callbacks and push them to the stack.
 *
 * \code
 * 	// EVENT
 * 	...
 * 	// Push the event to be fired given an initial rate of 0.0.
 * 	LabCallBack2 *rate_callback = new LabCallBack2(MyCallbackRate);
 * 	LabCallBack2 *realize_callback = new LabCallBack2(MyCallbackRealize);
 * 	eventStack.PushEvent("AN_EVENT", 0.0, a_param, rate_callback, realize_callback);
 * 	...
 * \endcode
 *
 * Push one callback per possible event according to the needs of your simulation
 * for the given stack.
 *
 */
class LIB_CLASS LabEventStack : public LabIBinarizable {
public:

	/**
	 * An event info/data descriptor
	 */
	typedef struct {
		double rate;
	} t_event_data;

	/**
	 * A complete event descriptor (including above info)
	 */
	struct t_event {

		/** Event's name (better choose a unique one) */
		string name;

		/** Event's data/info */
		t_event_data data;

		/** Parameter to be passed to both the event's rate callback and realize callback */
		EventCallBackParam param;
		/** Event's rate callback */
		RateCallBack *rate_callback;
		/** Event's realize callback */
		RealizeCallBack *realize_callback;

//		t_event() {};
//		t_event(const t_event& rhs) {
////			cout << "t_event copy ctor called !!!!!!!" << endl;
//			name = rhs.name;
//			data.rate = rhs.data.rate;
//			param = rhs.param;
//			rate_callback = rhs.rate_callback;
//			realize_callback = rhs.realize_callback;
//		}
	};

	LabEventStack();
	virtual ~LabEventStack();

	/**
	 * Add an event to the stack.
	 * @param name Name given to the event (better choose a unique one).
	 * @param initial_rate Rate at which the event can be fired at start <br/>
	 * 			(can be updated during the simulation - See SetEventRate()).
	 * @param param Parameter which will be passed to both rate and realize callbacks when fired.
	 * @param rate_callback Function to be called when updating the event's rate.
	 * @param realize_callback Function to be called when realizing/firing the event.
	 */
	void PushEvent(string name, double initial_rate,
			EventCallBackParam param,
			RateCallBack *rate_callback,
			RealizeCallBack *realize_callback);
	/**
	 * Remove the last added event from the stack.
	 */
	void PopEvent();
	/**
	 * TODO : free allocated callbacks...
	 */
	void Clear();
	/**
	 * Get the size of the stack.
	 * @return Number of events stored.
	 */
	int GetSize();
	/**
	 * Number of events fired since the stack was instantiated.
	 * @return
	 */
	int NbFiredEvents() { return nb_fired_evts; }
	string GetStats();

	/**
	 * Get the rate of the given event.
	 * @param id Identifier of the event in the stack.
	 * @return The rate as a double.
	 */
	double GetEventRate(int id);
	/**
	 * Update the event's rate.
	 * @param id Identifier of the event in the stack.
	 * @param rate New rate.
	 */
	void SetEventRate(int id, double rate);
	/**
	 * Get the name of an event.
	 * @param id Identifier of the event in the stack.
	 * @return The name.
	 */
	string GetEventName(int id);
	/**
	 * Get an ordered list of all the events present in the stack.
	 * @return List of all the events (in the order they've been pushed in the stack).
	 */
	vector<t_event>& GetEvents();

	/**
	 * Get the global stack rate (Computed as explained above - See UpdateStackRates()).
	 * @return The cumulative stack rate.
	 */
	double GetStackRate();
	/**
	 * Reset the global stack rate (to 0.0).
	 */
	void ResetStackRate();

	/**
	 * Update the current time of the stack.
	 * @param time The current time.
	 */
	void SetTime(double time);
	/**
	 * Get the current time of the stack.
	 * @return The time expressed as a double value.
	 */
	double GetTime();

	/**
	 * Update the global stack rate. Obtained by :
	 * - executing the rate callback of each event (updating its rate).
	 * - then, summing the events new rates.
	 */
	void UpdateStackRates();
	/**
	 * Call/fire the given event's realize callback.
	 * @param id Identifier of the event in the stack.
	 * @param time The given time becomes the current time of the stack.
	 */
	void RealizeEvent(int id, double time);
	/**
	 * Choose an event given a probability (Multinomial low), then realize it.
	 * @param p1 The probability for an event to be chosen among the others.
	 * @param time The given time becomes the current time of the stack.
	 */
	void RealizeEvent(double p1, double time);
	/**
	 * Choose an event given a RNG (Multinomial low), then realize it.<br/>
	 * Same than RealizeEvent(int id, double time), but the probability is computed using
	 * the specified random number generator.
	 * @param r The Random Number Generator.
	 * @param time The given time becomes the current time of the stack.
	 */
	void RealizeEvent(gsl_rng *r, double time);

	/**
	 * Copy the properties of a stack to this one.
	 * @param src_stack The source stack to copy from.
	 */
	void Copy(LabEventStack *src_stack);

	// Implement virtual
	/**
	 * Save the current state of the stack to a given output stream (binary format).
	 * @param ostream Output stream to backup to.
	 */
	void Binarize(ofstream *ostream);
	/**
	 * Restore the stack from a given input stream (binary format).
	 * @param istream Input stream from which to get the info required to restore the stack.
	 */
	void UnBinarize(ifstream *istream);


private:

	/** Current time to refer to for firing actions */
	double t;
	/** Events handler */
	vector<t_event> events;
	/** Global stack rate */
	double stackRate;

	int nb_fired_evts;
	double delta_t;
};


}  // namespace elps

#endif /* LABEVENT_H_ */




