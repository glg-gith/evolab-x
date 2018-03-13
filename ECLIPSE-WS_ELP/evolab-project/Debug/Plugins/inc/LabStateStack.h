/*
 * LabStateStack.h
 *
 *  Created on: 13 sept. 2012
 *      Author: golgauth
 */

#ifndef LABSTATESTACK_H_
#define LABSTATESTACK_H_

#include <string>
#include <vector>

#include <LabConsts.h>

#include <LabIBinarizable.h>
#include <LabBinaryUtils.h>

using namespace std;


namespace elps {


/**
 *
 * STATE :<br/>
 * A state is :
 * 		* Expressed as an integer value (which should be UNIQUE for each one)
 * 		* They can be linked to each other (by giving the id of the next state coming after the current one)
 * 		* They can have a limited life time - optional timeout - before returning to another specified state
 * 		* They can be forced to a specified value at any time
 *
 *
 * USAGE :
 *
 * - Instantiate
 * - Push state(s) (PushState() function)
 * - The state can be forced (SetCurrentState()), or shifted (GotoNextState())
 * - The timeout of the current state MUST be recurrently checked to take effect (TryDoTimeout())
 *
 */
class LIB_CLASS LabStateStack : public LabIBinarizable {
public:

	//typedef enum { NEXT, FINAL } t_next;
	/**
	 * A state info/data descriptor
	 */
	typedef struct {
		/** The delta of time (the state's life duration) after which the state switches to
		 *  the "timeout_id" state. (timeout < 0 : no timeout)*/
		double timeout;
		/** The logical next state after this one (next_id < 0 : final, non-looping state).
		 *  (Index of the state in the stack). */
		int next_id;
		/** The next state in case the given timeout is reached */
		int timeout_id;
//		/** If this state should be taken in account by the SetsManager **/
//		bool indexed;
	} t_state_data;

	/**
	 * A complete event descriptor (including above info)
	 */
	struct t_state {
		/** State's name (better choose a unique one) */
		string name;
		/** State's data/info */
		t_state_data data;

//		t_state() {};
//		t_state(const t_state& rhs) {
////			cout << "t_state copy ctor called !!!!!!!" << endl;
//			name = rhs.name;
//			data.timeout = rhs.data.timeout;
//			data.next_id = rhs.data.next_id;
//			data.timeout_id = rhs.data.timeout_id;
//		}
	};


	LabStateStack();
	virtual ~LabStateStack();

	/**
	 * Add a state on top of the stack
	 * (By default : no next logical state and no timeout).
	 * @param name Name given to the state (better choose a unique one)
	 * @param next_id The next logical state, this state should switch to (Index in the stack)
	 *                (ex: in S->I->R models, the next logical state of S is I, the next of I is R,
	 *                 and the next of R is '-1', aka : none).
	 *                WARNING : if used (!= 0), the id MUST exist in the stack ([0, nb_states-1])
	 * @param timeout_id The next state to switch to when the timeout of the current state is reached (Index in the stack)
	 *                   WARNING : if used (!= 0), the id MUST exist in the stack ([0, nb_states-1])
	 * @param timeout Optional (default is -1.0). As long as this value is less than 0.0, the timeout is not activated.
	 *                Aka : the state remains the same unless it is manually switched to another one.
	 */
	void PushState(string name, /*bool indexed = false,*/ int next_id = -1, int timeout_id = -1, double timeout = -1.0); 	//t_next loop_mode);
	/**
	 * Remove the last pushed state from the top of the stack
	 */
	t_state& PopState();
	/**
	 * Get the size of the stack.
	 * @return Number of states in the stack
	 */
	int GetSize();
	/**
	 * Get the current state of the SITE
	 * @return An integer value
	 */
	int GetCurrentState();
	/**
	 * Change the current state given its integer value (this value/state MUST exist be an id in
	 * the stack : [0, nb_states-1]), otherwise the state won't be changed), and the current
	 * time of the simulation.
	 * @param id The new index of the current state in the stack
	 * @param time The time at which the SITE has been turned into this state (usually the current time
	 *             of the simulation)
	 */
	void SetCurrentState(int id, double time);
	/**
	 * Get the time at which the SITE has been turned into the current state.
	 * @return A time expressed relatively to the algorithm used by the simulation to manage the time
	 */
	double GetCurrentStateTime();
	/**
	 * Get the time elapsed since the current state was entered.
	 * @param time The reference time (usually the current time of the simulation)
	 * @return The time elapsed
	 */
	double GetCurrentStateElapsedTime(double time);
	/**
	 * Get the time remaining until the current state will be quit (timeout reached).
	 * @param time The reference time (usually the current time of the simulation)
	 * @return The time remaining
	 */
	double GetCurrentStateRemainingTime(double time);
	/**
	 * Get the name of the given state
	 * @param id The index of the state in the stack
	 * @return A name
	 */
	string GetStateName(int id);
	/**
	 * Check if the given state id is in the stack [0, stack size - 1]
	 * @param id The state id
	 * @return
	 */
	bool IsValidStateId(int id);
	/**
	 * Get the list of the states
	 * @return A list of states
	 */
	vector<t_state>& GetStates();

	//double GetStatesSequence(int id);
	/**
	 * Shift the current state to the next logical one (current state becomes the state given by "next_id").
	 * @param time The time at which the SITE has been turned into the next one (usually the current simulation time)
	 */
	void GotoNextState(double time);

	/**
	 * Try to end the current state according to its timeout
	 * @param time Time of the attempt (usually the current simulation time)
	 */
	void TryDoTimeout(double time);

	/**
	 * Copy the properties of a stack to this one (default : time not reset).
	 * @param src_stack The source stack to copy from
	 * @param reset_time If the copy should reset the time (aka : the copy is considered as a change of state)
	 * @param time If "reset_time" is set to "True", this value MUST be specified (usually the current simulation time),
	 *             in order to give a new full life time (for states that do have a timeout)
	 */
	void Copy(LabStateStack *src_stack, bool reset_time = false, double time = -1.0);

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

	/** List of all the states in the stack */
	vector<t_state> states;
	/** Current state of the stack */
	int curState;
	/** The time at which the SITE has been turned into the current state
	 *  expressed in the simulation's time algorithm context */
	int curStateTime;
};


}  // namespace elps

#endif /* LABSTATESTACK_H_ */
