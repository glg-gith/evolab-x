/*
 * LabActionStack.h
 *
 *  Created on: 17 sept. 2012
 *      Author: golgauth
 */

#ifndef LABACTIONSTACK_H_
#define LABACTIONSTACK_H_

#include <iostream>
#include <vector>

#include <LabConsts.h>

#include <LabCallBack2.h>
#include <LabIBinarizable.h>
#include <LabBinaryUtils.h>

using namespace std;



namespace elps {

/**
 * Definition of the prototype for the callback function that will
 * encapsulate the instructions to be executed when the Action is fired
 *
 * @param param (void*) implies we are free to pass any object of any complexity
 *              The param will have to be cast inside the given function to be used properly
 * @return Has to return a double. Feel free to return 0.0 if no return value is required
 */
typedef double (*ACTION_CALLBACK_FUNC)(void* param);

/**
 * Pointer to any kind of object to be used in a callback
 */
typedef void* ActionCallBackParam;
/**
 * Callback
 */
typedef LabCallBack2 ActionCallBack;


/**
 * Stack of events that can be fired during the simulation at any time
 * in a global context (the user is free to determine what has to be done -
 * it can be of any kind; aka : on an individual, on the entire network, ...).
 *
 * USAGE :
 *
 * - Instantiate
 * - Push action(s) to be fired (PushTimeoutAction(), PushIntervalAction() functions)
 *
 * 		* Create a function of the form "double func_name(void* param)"
 * 		  to be executed when the action is fired
 *
 * - TryFire() action(s) recurrently (or not) in the simulation loop
 * 		(This also updates the current time of the stack, at first)
 *
 * (Those events/actions come in addition to events that can be fired at Site level.
 * See LabEventStack class).
 */
class LIB_CLASS LabActionStack : public LabIBinarizable {
public:

	/**
	 * Status of a given planned action/event
	 */
	typedef enum { WAITING, RUNNING, TERMINATED } t_status;

	/**
	 * An action info/data descriptor
	 */
	typedef struct {

		/** Action's execution status */
		t_status status;

		/** Time at which the action can be performed. Nothing can append before this date. */
		double t_start;
		/** Expiration date for this action. Useful only if the action is not a "one-shot" one. */
		double timeout;
		/** Time interval/frequency at which the action must be repeated. <br/>
		 	Useful only if the action is not a "one-shot" one. */
		double interval;

		/** Used to record the last time the action has been successfully fired. */
		double t_last_fired;

	} t_action_data;

	/**
	 * A complete action descriptor (including above info)
	 */
	typedef struct {

		/** Name of the action (better choose a unique name) */
		string name;

		/** Data/info related to the action */
		t_action_data data;

		/** Parameter to be passed to the action's callback */
		ActionCallBackParam param;
		/** Action's callback */
		ActionCallBack *action_callback;

	} t_action;


	LabActionStack();
	virtual ~LabActionStack();

	/**
	 * Add an action on top of the stack which will be fired ONCE at the given time
	 * @param name Give a name to the action
	 * @param param Parameter to be passed to the callback function to be executed<br/>
	 *        Must be casted to the expected type inside the callback function
	 * @param action_callback Function to be executed
	 * @param start_time Time at which the action will be fired
	 */
	void PushTimeoutAction(string name,
			ActionCallBackParam param,
			ActionCallBack *action_callback,
			double start_time);

	/**
	 * Add an action to the stack which will be fired every 'interval', given
	 * a starting time and a timeout (-1 : never ends until the end of the simulation)
	 * @param name Give a name to the action (Better use a unique name for each)
	 * @param param Parameter to be passed to the callback function to be executed<br/>
	 *        Must be casted to the expected type inside the callback function
	 * @param action_callback Function to be executed
	 * @param start_time Time at which the action will be fired
	 * @param interval Delta of time between each firing of the action
	 * @param timeout End time : after that the action is considered as TERMINATED
	 */
	void PushIntervalAction(string name,
			ActionCallBackParam param,
			ActionCallBack *action_callback,
			double start_time,
			double interval,
			double timeout = -1);					// -1 => NEVER

	/**
	 * Remove the last action pushed in the stack
	 */
	void PopAction();

	/**
	 * Get the size of the stack
	 * @return Number of actions stored in the stack
	 */
	int GetSize();

	/**
	 * Get the current time of the stack
	 * @return Return a double expressed in the user defined way<br/>
	 *         Note : the time has to be set by the user in his simulation loop using SetTime()
	 */
	double GetTime();
	/**
	 * Update the current time of the stack
	 * @param time A double expressed as desired by the user
	 */
	void SetTime(double time);

	/**
	 * Get the name of the action by its id in the stack
	 * @param id Position in the stack of the targeted action
	 * @return A name given to the action when it has been pushed to the stack
	 */
	string GetActionName(int id);
	/**
	 * Get the stack
	 * @return A vector containing all the actions
	 */
	vector<t_action>& GetActions();

	/**
	 * If time as come to fire all the actions, does it. Do nothing otherwise.
	 * @param current_time The current time (of the simulation) at which we try to fire the Action
	 * @return True if at least one action has been fired.
	 */
	bool TryFire(double current_time);					// Try firing all actions given the current time
	/**
	 * If time as come to fire the given action, does it. Do nothing otherwise.
	 * @param current_time The current time (of the simulation) at which we try to fire the Action
	 * @param id The id of the action to be fired.
	 * @return True if the action has been fired.
	 */
	bool TryFireAction(double current_time, int id);	// Try firing an action given the current time
	/**
	 * Force firing all the actions without any further time checking
	 */
	void FireNow();											// Fire all the events instantaneously
	/**
	 * Force firing the given action without any further time checking
	 */
	void FireActionNow(int id);								// Fire an event instantaneously

	/**
	 * Copy the properties of a stack to this one.
	 * @param src_stack The source stack to copy from.
	 */
	void Copy(LabActionStack *src_stack);

	// Implement virtual
	/**
	 * Save the current state of the stack to a given output stream (binary format).
	 * @param ostream Output stream to backup to.
	 */
	void Binarize(ofstream *ostream);
	/**
	 * Restore the stack from a given input stream (binary format).
	 * @param istream Input stream from which to get the info required to restore the stack
	 */
	void UnBinarize(ifstream *istream);


private:

	/**
	 * Current time to refer to for firing actions
	 */
	double t;

	/**
	 * Actions handler
	 */
	vector<t_action> actions;

	/**
	 * Process firing a given action
	 * @param current_time Time at which to try firing
	 * @param ptr_act Handler to the action to be fired
	 * @param success Set to True if the action has been successfully fired
	 */
	void FireAction(double current_time, t_action *ptr_act, bool& success);

};



}  // namespace elp


#endif /* LABACTIONSTACK_H_ */
