/*
 * LabSiteBase.h
 *
 *  Created on: 18 sept. 2012
 *      Author: golgauth
 */

#ifndef LABSITEBASE_H_
#define LABSITEBASE_H_


#include <LabAttributesStack.h>
#include <LabStateStack.h>
#include <LabUserData.h>

#include <LabNetwork.h>

#include <LabSiteSetsManager.h>

#include <LabSimulatorBase.h>


//// Cython !!!
#ifndef NOPYTHON
	#include <Python.h>
	//#include <LabCythonUtils.h>
#endif


namespace elps {




/**
 * Base class for handling user defined data on a given SITE.<br/>
 * - In a MASS ACTION context, the sites can be stored in an array (See LabArrayBinding).<br/>
 * - In a SPATIALIZED context, the sites would be stored in the node of a network (See LabNetBinding).<br/>
 *
 * This class is specialized in managing 2 stacks of ATTRIBUTES (See LabAttributesStack).
 * - The first one stores the attributes related to the INDIVIDUAL
 * currently residing on the SITE.<br/>
 * - The second one stores the attributes related to the SITE
 * itself (typically, if the individual moves or dies, those attributes would persist...).
 *
 * It's state (the state of its attributes at a given time) can be
 * binarized/dumped by implementing the features inherited from the
 * LabIBinarizable interface (See Binarize(), UnBinarize()).
 *
 * It manages also a stack of states. A SITE can switch from one state to another.
 *
 * STATES (See also : LabStateStack) :<br/>
 * A state (like ATTRIBUTES) can change in time. The differences are :
 * 		* It can have only integer values [0..N]
 * 		* They can be linked to each other (by giving the id of the next state coming after the current one)
 * 		* They can have a limited life time - optional timeout - before returning to an other specified state
 *
 * <b>Note :</b> A Site is <b>always</b> related to a simulator.
 *
 * USAGE :
 * =======
 *
 * A basic use case would look like follows :
 *
 * \code
 * 	LabSiteBase *a_site = new LabSiteBase(sim);		// 'sim' : the related simulator
 *
 * 	// ATTRIBUTES
 * 	// Indexed attribute : here, 3 sets/groups are created ("pop0", "pop1", "pop2")
 * 	// [In case the site was registered by the <b>Sets Manager</b> (See LabSiteSetsManager, LabSiteSetsCalculator)]
 * 	a_site->PushAttribute("pop", LabSiteBase::IND_DEP, true, 3);
 * 	// Classical attributes (doubles)
 * 	a_site->PushAttribute("x", LabSiteBase::IND_DEP);
 * 	a_site->PushAttribute("m", LabSiteBase::IND_DEP);
 *
 * 	// STATES
 * 	// We define 3 possible states :
 * 	// Parameters : 1, -1, -1 => next_id = 1, timeout_id = -1, timeout = -1.0
 * 	a_site->PushState("EMPTY",    1, -1, -1);
 * 	a_site->PushState("RESIDENT", 1, -1, -1);
 * 	a_site->PushState("MUTANT",   1, -1, -1);
 *
 * 	// Choose whether the current site is in EMPTY or RESIDENT state from the
 * 	// probability 'p_init'.
 * 	if(rng->GetNextDouble() < p_init)
 * 	{
 * 		// This site will be a member of the "RESIDENT" set
 * 		a_site->SetState(BdmLsim3::RESIDENT, GetTime());
 *
 * 		a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::POP, 1);
 * 		a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::X, x1);
 * 		a_site->SetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim3::M, mo1);
 *
 * 		popcnt++;
 * 	}
 * 	else
 * 	{
 * 		// This site will be a member of the "EMPTY" set
 * 		a_site->SetState(BdmLsim3::EMPTY, GetTime());
 *
 * 		a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::POP, 0);
 * 		a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::X, 0.0);
 * 		a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::M, 0.0);
 * 	}
 * \endcode
 *
 * <b>Note </b> about <b>Sets Management</b> :
 *
 * As mentioned in the source code above, in case the site was registered by the <b>Sets Manager</b>,
 * Indexed attribute would result in the creation of related sets / groups (ex. 3 groups ["pop0", "pop1", "pop2"]
 * are created from the indexed attribute "pop"...) - See LabSiteSetsManager, LabSiteSetsCalculator for more
 * details concerning Sets.
 *
 */
class LIB_CLASS LabSiteBase : public LabUserData, public LabIBinarizable {
public:

	/** Parent node for this Site (required only in a SPATIALIZED context) */
	LabNode *parent;

	//LabSiteBase();
	/**
	 * Basic constructor for use in a NON-SPATIALIZED context.
	 * @param sim The Simulator this Site is related to.
	 */
	LabSiteBase(LabSimulatorBase *sim);
	/**
	 * Basic constructor for use in a SPATIALIZED context.
	 * @param sim The Simulator this Site is related to.
	 * @param parent_node The network node (LabNode) this Site is attached to.
	 */
	LabSiteBase(LabSimulatorBase *sim, LabNode *parent_node);


	//// Cython !!!
#ifndef NOPYTHON
	/**
	 * Cython usage only : DO NOT USE !
	 * @param obj The PyLabSiteBase Python object related to this object
	 * @param sim  The LabSimulator* related to this object
	 */
	LabSiteBase(PyObject *obj, LabSimulatorBase *sim);
	/**
	 * Cython usage only : DO NOT USE !
	 * @param obj The PyLabSiteBase Python object related to this object
	 * @param sim  The Simulator this Site is related to.
	 * @param parent_node The network node (LabNode) this Site is attached to.
	 */
	LabSiteBase(PyObject *obj, LabSimulatorBase *sim, LabNode *parent_node);
#endif


	virtual ~LabSiteBase(); // Forbidden for now : throws an exception and
							// unfortunately terminates the program...

	/**
	 * Type / level of relationship / dependency between the attribute and the system
	 * (related to the individual, the site or the entire environment).
	 */
	typedef enum { IND_DEP = 1, SITE_DEP = 2/*, ENV_DEP = 4*/ } t_deptype;

	typedef set<string> t_string_set;
	// typedef vector<string> t_string_set;


	// TODO : Remove this getter => attributesStack access should
	// be restricted to 'private' scope
	/**
	 * Get the stack of attributes at the INDIVIDUAL level.
	 * @return A pointer to LabAttributesStack of the SITE.
	 */
	LabAttributesStack *GetIndAttributesStack();
	/**
	 * Get the stack of attributes at the SITE level.
	 * @return A pointer to LabAttributesStack of the SITE.
	 */
	LabAttributesStack *GetSiteAttributesStack();

	// Direct access to LabAttributesStack stuffs
	/**
	 * Add a new attribute on top of the specified stack (See LabAttributesStack::PushAttribute()).
	 * @param name Name for this attribute (better use a unique name for each one).
	 * @param dep_type Dependency type (See LabAttributesStack::t_deptype). Choose attribute stack.
	 * @param d_value Value for the attribute. Pass an <b>int</b> for indexed attributes.
	 * @param indexed If this attribute is an "INDEXED" one.
	 * @param nb_beans Number of distinct values the indexed attribute can take [0..nb_beans].
	 */
	void PushAttribute(
			string name,
			t_deptype dep_type,
			double d_value = 0.0,
			//int i_value = 0//,
			//void *o_value = NULL
			bool indexed = false,
			int nb_beans = 2				// 2 = boolean attribute
			);
	/**
	 * Another version of PushAttribute() for Cython compatibility.
	 * (Uses int instead of t_deptype enum).
	 */
	void PushAttribute(
			string name,
			int dep_type,
			double d_value = 0.0,
			//int i_value = 0
			bool indexed = false,
			int nb_beans = 2				// 2 = boolean attribute
			);
	/**
	 * Remove the last pushed attribute from the top of the stack (See LabAttributesStack::PopAttribute()).
	 * @param dep_type The dependency of the attribute to be popped.
	 */
	// TODO : Untested
	void PopAttribute(t_deptype dep_type);
	/**
	 * Another version of PopAttribute() for Cython compatibility.
	 * (Uses int instead of t_deptype enum).
	 */
	void PopAttribute(int dep_type);
////	vector<LabAttributesStack::t_attr> GetAttributes();
	/**
	 * Get the attributes related to the individual (See LabAttributesStack::GetIndAttributes()).
	 * @return List of attributes.
	 */
	vector<LabAttributesStack::t_attr>& GetIndAttributes();
	/**
	 * Get the attributes related to the SITE (node) (See LabAttributesStack::GetSiteAttributes()).
	 * @return List of attributes.
	 */
	vector<LabAttributesStack::t_attr>& GetSiteAttributes();
//	/*
//	 * Get the attributes related to the environment / SPACE (network) (See LabAttributesStack::GetEnvAttributes()).
//	 * @return List of attributes
//	 */
//	vector<LabAttributesStack::t_attr*> GetEnvAttributes();


	/**
	 * Get the number of attributes at the given dependency level using a filter.
	 * @param dep_filter Filter can be any combination of t_deptype s.
	 * @return A number.
	 */
	int GetNbAttributes(int dep_filter = IND_DEP | SITE_DEP);

	/**
	 * Shortcut to get the integer value of an attribute specified by its id in the
	 * attributes stack and its dependency level.
	 * @param dep_filter Dependency level of the targeted attribute.
	 * @param id Identifier in the list relatively to the specified dependency level.
	 * @return The integer value of the attribute.
	 */
	int GetAttrInt(t_deptype dep_filter, int id);
	/**
	 * Another version of GetAttrInt() for Cython compatibility.
	 * (Uses int instead of t_deptype enum).
	 */
	int GetAttrInt(int dep_filter, int id);
	//int GetIndexedAttr(int dep_filter, int id);
	/**
	 * Shortcut to get the double value of an attribute specified by its id in the
	 * attributes stack and its dependency level.
	 * @param dep_type Dependency level of the targeted attribute.
	 * @param id Identifier in the list relatively to the specified dependency level.
	 * @return The double value of the attribute.
	 */
	double GetAttrDouble(t_deptype dep_type, int id);
	/**
	 * Another version of GetAttrDouble() for Cython compatibility.
	 * (Uses int instead of t_deptype enum).
	 */
	double GetAttrDouble(int dep_type, int id);


	/*
	 * Shortcut to set the integer value of an attribute specified by its id in the
	 * attributes stack and its dependency level.
	 * @param dep_type Dependency level of the targeted attribute
	 * @param id Identifier in the list relatively to the specified dependency level
	 * @param value The new integer value of the attribute
	 */
	////void SetAttrInt(LabAttributesStack::t_deptype dep_type, int id, int value);
	/*
	 * Another version of SetAttrInt() for Cython compatibility.
	 * (Uses int instead of t_deptype enum).
	 */
	////void SetAttrInt(int dep_type, int id, int value);

	/**
	 * Shortcut to set the double value of an attribute specified by its id in the
	 * attributes stack and its dependency level.
	 * @param dep_type Dependency level of the targeted attribute
	 * @param id Identifier in the list relatively to the specified dependency level
	 * @param value The new double value of the attribute
	 */
	void SetAttrDouble(t_deptype dep_type, int id, double value);
	/**
	 * Another version of SetAttrDouble() for Cython compatibility.
	 * (Uses int instead of t_deptype enum).
	 */
	void SetAttrDouble(int dep_filter, int id, double value);


	/**
	 * Get the states stack.
	 * @return A stack of states.
	 */
	LabStateStack *GetStateStack();

	// Direct access to LabStateStack stuffs
	/**
	 * Get the current state of this SITE.
	 * @return An integer value (state is expressed as an index in the states stack).
	 */
	int GetState();
	/**
	 * Change the current state of the SITE.
	 * @param state New state (new index in the states stack).
	 * @param time Time at which this change is performed (usually the current simulation time).
	 */
	void SetState(int state, double time);
	/**
	 * Shortcut to add a new state to the state stack (See LabStateStack::PushState()).
	 * (By default : no next logical state and no timeout).
	 * @param name Name of the new state.
	 * @param next_id Next logical state to this state (if any).
	 * @param timeout_id Index of the state to switch to when timeout is reached (if any timeout).
	 * @param timeout Delta of time before switching to the "timeout_id" state (if any timeout).
	 */
	void PushState(string name, /*bool indexed = false,*/ int next_id = -1, int timeout_id = -1, double timeout = -1.0); 	//t_next loop_mode);
	/**
	 * Shortcut to remove the state located on top of the states stack.
	 */
	// TODO : Untested
	void PopState();
	/**
	 * Shortcut to switch into the next logical state (See LabStateStack::GotoNextState()).
	 * @param time The time at which the switch ocurrs (usually the current simulation time).
	 */
	void GotoNextState(double time);
	/**
	 * Shortcut to try to terminate a state (See LabStateStack::TryDoTimeoutState()).
	 * @param time The time of the attempt (usually the current simulation time).
	 */
	void TryDoTimeoutState(double time);
	/**
	 * Get the number of states in the states stack.
	 * @return Number of states currently stored.
	 */
	int GetNbStates();
	/**
	 * Get the time elapsed since the current state was entered.
	 * @param time The reference time (usually the current time of the simulation).
	 * @return The time elapsed.
	 */
	double GetCurrentStateElapsedTime(double time);
	/**
	 * Get the time remaining until the current state will be quit (timeout reached).
	 * @param time The reference time (usually the current time of the simulation).
	 * @return The time remaining.
	 */
	double GetCurrentStateRemainingTime(double time);

	/**
	 * Self factory. DO NOT USE it unless you know what you're doing !
	 * @param parent_node The parent node in SPATIALIZED context.
	 * @return A pointer to LabSiteBase (casted into LabUserData *).
	 */
	virtual LabUserData *New(LabNode *parent_node = NULL);
	/**
	 * Shortcut to copy the whole attributes stack from another SITE (See LabAttributesStack::Copy()).
	 * @param src_site The site (LabSiteBase) to copy from.
	 * @param dep_filter Filter used to select which attributes will be copied<br/>
	 * 			(ex1 : dep_filter = IND_DEP | SITE_DEP, means : copies only non-environmental attributes<br/>
	 * 			 ex2 : dep_filter = IND_DEP | SITE_DEP | ENV_DEP : copies all).
	 */
	virtual void Copy(LabUserData *src_site, int dep_filter = IND_DEP | SITE_DEP);

	// Implement virtual
	/**
	 * Shortcut to dump the current state of the SITE (See LabAttributesStack::Binarize()).
	 * @param ostream Output stream to dump to.
	 */
	void Binarize(ofstream *ostream);
	/**
	 * Shortcut to restore the current state of the SITE from a dump (See LabAttributesStack::UnBinarize()).
	 * @param ostream Input stream to restore from.
	 */
	void UnBinarize(ifstream *istream);



	/**
	 * Subscribe this Site to a specified Set.
	 * (This feature is only available if the Site was instantiated via its protected constructor :
	 * See LabSiteBase(LabSimulatorBase *sim, LabNode *parent_node, bool set_trackable)...
	 *
	 * DO NOT USE it unless you know what you're doing !
	 *
	 * @param set_name The name of the Set to subscribe to.
	 */
	void SubscribeSet(string set_name);
	/**
	 * UnSubscribe this Site from a specified Set.
	 * Afterward this Site belong to this Set.
	 * (This feature is only available if the Site was instantiated via its protected constructor :
	 * See LabSiteBase(LabSimulatorBase *sim, LabNode *parent_node, bool set_trackable)...
	 *
	 * DO NOT USE it unless you know what you're doing !
	 *
	 * @param set_name The name of the Set to unsubscribe from.
	 */
	void UnSubscribeSet(string set_name);
	/**
	 * Subscribe this Site to all the Sets it belongs to.
	 * Afterward this Site doesn't belong to this Set anymore.
	 * (This feature is only available if the Site was instantiated via its protected constructor :
	 * See LabSiteBase(LabSimulatorBase *sim, LabNode *parent_node, bool set_trackable)...
	 *
	 * DO NOT USE it unless you know what you're doing !
	 */
	void SubscribeAllSets();
	/**
	 * UnSubscribe this Site from all the Sets it belongs to.
	 * (This feature is only available if the Site was instantiated via its protected constructor :
	 * See LabSiteBase(LabSimulatorBase *sim, LabNode *parent_node, bool set_trackable)...
	 *
	 * DO NOT USE it unless you know what you're doing !)
	 */
	void UnSubscribeAllSets();

	/**
	 * Return the list of sets this Site belongs to.
	 * @return A std::set<string> containing the names of the Sets.
	 */
	inline t_string_set& GetSubscribedSets() { return subscribed_sets; };
	/**
	 * Print the list of sets (names) this Site belongs to.
	 */
	void PrintSubscribedSets();

	/**
	 * Get the Simulator (LabSimulatorBase *) this Site is related to.
	 * @return A pointer to the simulator instance.
	 */
	inline LabSimulatorBase *GetSimulator() { return simulator; }


	/**
	 * Mainly for use from Cython : DO NOT USE unless you know what you're doing !
	 * Get the type of this Site (can be SiteBase, SiteEvent), according
	 * to what was set using SetSiteModelType().
	 *
	 * @return The type in its string representation.
	 */
	inline string CyGetSiteModelType() { return site_model_type; }
	/**
	 * Mainly for use from Cython : DO NOT USE unless you know what you're doing !
	 * Set the type of this Site (can be SiteBase, SiteEvent, ...).
	 *
	 * @param model_type The string representation chosen for this type.
	 */
	inline void CySetSiteModelType(string model_type) { site_model_type = model_type; }

//	/**
//	 * Mainly for use from Cython : DO NOT USE unless you know what you're doing !
//	 * Get the Python representation of this Site as a Python object (PyLabSiteBase,
//	 * PyLabSiteEvent, ...).
//	 *
//	 * @return A Python object (as a void *), must be casted to PyObject * to be usable.
//	 */
//	inline void *GetCyPyObj() { return cy_py_obj_; }
//	/**
//	 * Mainly for use from Cython : DO NOT USE unless you know what you're doing !
//	 * Store the Python representation of this Site as a Python object (PyLabSiteBase,
//	 * PyLabSiteEvent, ...).
//	 *
//	 * @param cy_py_obj The Python object (PyObject *).
//	 */
//	inline void SetCyPyObj(void *cy_py_obj) { cy_py_obj_ = cy_py_obj; }


protected:
	/** Reference to the Simulator related to this Site */
	LabSimulatorBase *simulator;

	/**
	 * Constructor used to instantiate a LabSiteBase such that it is <b>set_trackable</b>
	 * (Aka : it is registered to the LabSiteSetsManager).
	 * @param sim The Simulator this Site is related to.
	 * @param parent_node The network node (LabNode) this Site is attached to.
	 * @param set_trackable If this site is meant to be managed by the LabSiteSetsManager.
	 */
	LabSiteBase(LabSimulatorBase *sim, LabNode *parent_node, bool set_trackable);

private:

	/** Pointer to the attributes stack of this Site at the INDIVIDUAL level */
	LabAttributesStack *indAttributesStack;
	/** Pointer to the attributes stack of this Site at the SITE level */
	LabAttributesStack *siteAttributesStack;

	/** Pointer to the states stack of this Site */
	LabStateStack *stateStack;


	/** Shortcut to instantiate the states stack */
	void CreateStateStack();

	/** Handler to the LabSiteSetsManager singleton instance */
	LabSiteSetsManager<LabSiteBase *> *setsManager;

	/** The list of sets this Site belongs to */
	t_string_set subscribed_sets;	// A subset of "sets" (from LabSiteSetsManager)
									// (used for fast removal/unsubscribe of a site)
	/** If this site is meant to be managed by the LabSiteSetsManager */
	bool setTrackable;

	/**
	 * General initializations shared by all the constructors
	 * @param sim The Simulator this Site is related to.
	 * @param parent_node The network node (LabNode) this Site is attached to.
	 * @param set_trackable If this site is meant to be managed by the LabSiteSetsManager.
	 */
	void Init(LabSimulatorBase *sim, LabNode *parent_node, bool set_trackable);

	/** The string representation of the type of this Site (for Cython usage !) */
	string site_model_type;

	/** Storage of the Python object representation for this Site */
	void *cy_py_obj_;
};


typedef LabSiteSetsManager<LabSiteBase *> SiteSetsMgr;



} /* namespace elps */
#endif /* LABSITEBASE_H_ */
