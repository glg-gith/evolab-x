/*
 * LabSiteEvent.h
 *
 *  Created on: 18 sept. 2012
 *      Author: golgauth
 */

#ifndef LABSITEEVENT_H_
#define LABSITEEVENT_H_

#include <LabSiteBase.h>

#include <LabSimulatorBase.h>

#include <LabEventStack.h>


//// Cython !!!
#ifndef NOPYTHON
	#include <Python.h>
	//#include <LabCythonUtils.h>
#endif


namespace elps {

/**
 * This class handles ATTRIBUTES and STATES (since it inherits from LabSiteBase), but also <b>EVENTS</b>.
 *
 * <b>Note :</b> The concept of SITE (data of a node in a network - SPATIALIZED context) has been kept
 * for MASS ACTION context for preserving genericity.
 *
 * All the features related to ATTRIBUTES and STATES are deeply described in LabSiteBase, since
 * LabSiteEvent inherits from this class.
 *
 * EVENTS (See also : LabEventStack) :
 *
 * An event is an action that can be fired at any time on the SITE (locally).
 *
 * - It has a given propensity to occur (which can change during the simulation life time)
 * - It comes with an action (method) which is fired (executed) when the event occurs
 *
 * The LabSiteEvent class is nothing else but an aggregate of the attributes, states and events stacks :
 * See LabAttributesStack, LabStateStack and LabEventStack.
 *
 * <b>Note :</b> A Site is <b>always</b> related to a simulator.
 *
 * USAGE :
 * =======
 *
 * A basic use case would look like follows (assuming the setup for ATTRIBUTES and STATES has already
 * been done - See LabSiteBase) :
 *
 * First of all define the callback methods to be fired. Those must be of the form :
 *
 * \code
 * 	double BirthRate(EventCallBackParam node)                                   // We choose to pass a LabNode
 * 	{
 * 		LabNode *a_node = (LabNode *)node;
 * 		double rate;
 * 		BdmLsim4::t_site *a_site = BdmLsim4::GetSiteData(a_node);               // Get the Site embedded in this node
 * 		rate = b * a_site->GetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::X);    // Compute the return value
 * 		if(a_site->GetState() == BdmLsim4::MUTANT)
 * 			rate *= 0.9;
 * 		return rate;                                                            // return a float
 * 	}
 *
 * 	double RealizeBirth(EventCallBackParam node)
 * 	{
 * 		cout << "BIRTH fired !" << endl;
 * 		LabNode *a_node = (LabNode *)node;
 * 		// DO SOMETHING with node here...
 * 		return 0.0;
 * 	}
 * \endcode
 *
 * <b>Note :</b> the <b>node</b> parameter can be any object (here we assume we are in a SPATIALIZED
 * context, that's why we use the "node" term).
 *
 * Then we can create the callbacks and assign them to the Site :
 *
 * \code
 * 	// EVENTS :
 * 	//
 * 	rate_callback = new RateCallBack(BirthRate);
 * 	realize_callback = new RealizeCallBack(RealizeBirth);
 * 	a_site->PushEvent("BIRTH", 0, NULL, rate_callback, realize_callback);
 * 	//
 * 	rate_callback = new RateCallBack(DeathRate);
 * 	realize_callback = new RealizeCallBack(RealizeDeath);
 * 	a_site->PushEvent("DEATH", 0, NULL, rate_callback, realize_callback);
 * 	// ...
 * 	// ... etc.
 * \endcode
 *
 * Push one callback per possible event according to the needs of your simulation
 * for the given Site.
 *
 */
class LIB_CLASS LabSiteEvent: public LabSiteBase {
public:
	//	LabSiteEvent();
	/**
	 * Basic constructor for use in a NON-SPATIALIZED context.
	 * @param sim The Simulator this Site is related to.
	 */
	LabSiteEvent(LabSimulatorBase * sim);
	/**
	 * Basic constructor for use in a SPATIALIZED context.
	 * @param sim The Simulator this Site is related to.
	 * @param parent_node The network node (LabNode) this Site is attached to.
	 */
	LabSiteEvent(LabSimulatorBase * sim, LabNode *parent_node);
	//virtual ~LabSiteEvent();


	//// Cython !!!
#ifndef NOPYTHON
	/**
	 * Cython usage only : DO NOT USE !
	 * @param obj The PyLabSiteEvent Python object related to this object
	 * @param sim  The LabSimulator* related to this object
	 */
	LabSiteEvent(PyObject *obj, LabSimulatorBase *sim);
	/**
	 * Cython usage only : DO NOT USE !
	 * @param obj The PyLabSiteEvent Python object related to this object
	 * @param sim  The Simulator this Site is related to.
	 * @param parent_node The network node (LabNode) this Site is attached to.
	 */
	LabSiteEvent(PyObject *obj, LabSimulatorBase *sim, LabNode *parent_node);
#endif



	// TODO : Remove this getter => stacks access should be restricted to 'private' scope
	/**
	 * Get the events stack.
	 * @return A stack of events.
	 */
	LabEventStack *GetEventStack();

	// Direct access to LabStateStack stuffs
	/**
	 * Shortcut to add a new event to the event stack (See LabEventStack::PushEvent()).
	 * @param name Name given to the event (better choose a unique one).
	 * @param initial_rate Rate at which the event can be fired at start.
	 * @param param Parameter which will be passed to both rate and realize callbacks when fired
	 *              Should be left NULL when sites are built from models (See LabNetBinding and
	 *              LabArrayBinding classes).
	 * @param rate_callback Function to be called when updating the event's rate.
	 * @param realize_callback Function to be called when realizing/firing the event.
	 */
	void PushEvent(string name,
			double initial_rate,
			EventCallBackParam param,
			RateCallBack *rate_callback,
			RealizeCallBack *realize_callback);
	/**
	 * Shortcut to remove the event located on top of the events stack.
	 */
	void PopEvent();
	/**
	 * Get the number of events in the events stack.
	 * @return Number of events currently stored.
	 */
	int GetNbEvents();
	/**
	 * Shortcut to access the total rate of the events stack.
	 * @return The cumulative stack rate.
	 */
	double GetEventStackRate();
	/**
	 * Shortcut to reset the total rate of the events stack.
	 */
	void ResetEventStackRate();
	/**
	 * Shortcut to update the current time of the events stack.
	 * @param time The current time (usually the current simulation time).
	 */
	void SetTimeEvent(double time);
	/**
	 * Shortcut to get the current time of the events stack.
	 * @return The time expressed as a double value.
	 */
	double GetTimeEvent();
	/**
	 * Shortcut to update the global stack rate (See LabEventStack::UpdateStackRates()).
	 */
	void UpdateEventStackRates();
	/**
	 * Shortcut to call/fire the given event's realize callback
	 * (See LabEventStack::RealizeEvent(int id, double time)).
	 * @param id Identifier of the event in the events stack.
	 * @param time The given time becomes the current time of the stack.
	 */
	void RealizeEvent(int id, double time);
	/**
	 * Shortcut to choose an event given a probability (Multinomial low), then realize it.
	 * (See LabEventStack::RealizeEvent(double p1, double time)).
	 * @param p1 The probability [0.0, 1.0] used for selecting the event.
	 * @param time The given time becomes the current time of the stack.
	 */
	void RealizeEvent(double p1, double time);

	/**
	 * Clear all the events (See PopEvent()).
	 */
	void ClearEvents();

	/**
	 * Self factory. DO NOT USE it unless you know what you're doing !
	 * @param parent The parent node.
	 * @return A "set managed" instance of LabSiteEvent.
	 */
	virtual LabUserData *New(LabNode *parent_node = NULL);
	/**
	 * Copy the 3 stacks (attributes, states, events) given a source SITE to this one.
	 * @param src_site The source site.
	 * @param dep_filter Dependency level - concerns only the attributes stack (See LabAttributesStack::Copy()).
	 */
	virtual void Copy(LabUserData *src_site, int dep_filter = IND_DEP | SITE_DEP);

	// Implement virtual
	/**
	 * Complete dump the current state of the SITE.
	 * @param ostream Output stream to dump to.
	 */
	void Binarize(ofstream *ostream);
	/**
	 * Restore the current state of the SITE from a dump.
	 * @param ostream Input stream to restore from.
	 */
	void UnBinarize(ifstream *istream);

	/**
	 * DO NOT USE ME ! [Cython side usage only]<br/>
	 * Update the current parameter used in the callbacks (usually the parent node)
	 * of this Site.
	 * @param i Position in the event stack.
	 * @param p Pointer on the object to be passed to the callbascks.
	 */
	void SetCallBackParamAt(int i, void *p);

private:
	/**
	 * Constructor used to instantiate a LabSiteBase such that it is <b>set_trackable</b>
	 * (Aka : it is registered to the LabSiteSetsManager).
	 * @param sim The Simulator this Site is related to.
	 * @param parent_node The network node (LabNode) this Site is attached to.
	 * @param set_trackable If this site is meant to be managed by the LabSiteSetsManager.
	 */
	LabSiteEvent(LabSimulatorBase * sim, LabNode *parent_node, bool set_trackable);
	/**
	 * General initializations shared by all the constructors.
	 * Comes in addition to instructions already done in LabSiteBase.Init().
	 */
	void Init();

	/** Pointer to the events stack */
	LabEventStack *eventStack;
};

} /* namespace elps */
#endif /* LABSITEEVENT_H_ */
