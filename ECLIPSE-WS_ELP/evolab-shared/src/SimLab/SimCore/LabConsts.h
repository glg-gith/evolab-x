/*
 * LabConsts.h
 *
 *  Created on: Feb 26, 2013
 *      Author: golgauth
 */

#ifndef LABCONSTS_H_
#define LABCONSTS_H_

#include <stdexcept>


#include <iostream>
#include <sstream>

using namespace std;

namespace elps {

/** EVOLAB-X version used all over the program */
#define ELPS_VERSION 0.0.5
/*
 * This is how one gets a macro into quotes; an important one to keep
 * in all program templates.
 */
#define _QUOTEME(x) #x
#define QUOTEME(x) _QUOTEME(x)


/** Build without any Python dependency : for C usage only */
//#define NOPYTHON


// Stuffs required under Windoz to export classes properly
// out of the shared library...
// USAGE :
// 		- Add "-DBUILD_LIB" to the compiler options
//
#ifdef __WIN32__
	#ifdef BUILD_LIB
		#define LIB_CLASS __declspec(dllexport)
	#else
		#define LIB_CLASS __declspec(dllimport)
	#endif
#else
	#define LIB_CLASS		// Linux & other Unices : leave it blank !
#endif


/** Custom round math MACRO returning an integer */
#define RND(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))


/**
 * Main class for handling the program's constants and specially
 * all the errors messages (throws exceptions on demand).
 */
class LIB_CLASS LabConsts {
public:
	LabConsts() { };
	virtual ~LabConsts() { };

	// Massages

	// Warnings

	// Errors
	typedef enum {
		U_SET_RESERVED,
		UNSET_SET_RESERVED,
		INVALID_SET_NAME,
		NOT_SET_TRACKABLE,
		MALFORMED_DISTRIBUTION,
		MALFORMED_DISTRIBUTION_NET,
		MALFORMED_MODELS_DISTRIBUTION,
		BAD_POP_ALLOCATION,
		BAD_PREV_POP_ALLOCATION,
		BAD_DEPTYPE_ATTR_SIZES,

		SITE_EVENT_PARENT_NULL,

		SET_SITES_CALL_BEGIN_REQUIRED,
		SET_SITES_BAD_INDEX,
		SET_SITES_BAD_NODE_INDEX,
		SET_SITES_UNSET_FOUND,
		SET_SITES_NOT_ENDED
	} t_error;


	/**
	 * Throws an error after having displayed the appropriate message.
	 * @param e The right index in the possible errors list.
	 * @param args Storage of potential additional information required for treating the error.
	 */
	static void E(t_error e, void *args = NULL) {
		stringstream err;
		switch (e) {
		case U_SET_RESERVED: 				err << "'U' is the universe set. This name is reserved : DO NOT USE IT !"; break;
		case UNSET_SET_RESERVED: 			err << "'UNSET' is the unset sites set. This name is reserved : DO NOT USE IT !"; break;
		case INVALID_SET_NAME: 				err << "Empty string \"\" is not a valid name for a set !"; break;
		case NOT_SET_TRACKABLE: 			err << "This LabSite is not enabled for set storage !"; break;
		case MALFORMED_DISTRIBUTION: 		err << "Distribution cumulated sum MUST be equal to 1.0 !"; break;
		case MALFORMED_DISTRIBUTION_NET: 	err << "Distribution cumulated sum MUST be equal to the network's size !"; break;
		case MALFORMED_MODELS_DISTRIBUTION: err << "Distribution and models vectors MUST be the same size (with size > 0) !"; break;
		case BAD_PREV_POP_ALLOCATION: 		err << "No previous definition of the maximum population size found. Call SetMaxPopulationSize() first !"; break;
		case BAD_POP_ALLOCATION: 			err << "The maximum population size is at least ONE individual !"; break;
		case BAD_DEPTYPE_ATTR_SIZES: 		err << "Dependency types and attributes id vectors MUST be the same size !"; break;

		case SITE_EVENT_PARENT_NULL: 		err << "Parent node cannot be null : Use LabSiteEvent(LabSimulatorBase * sim, LabNode *parent_node) constructor !"; break;

		case SET_SITES_CALL_BEGIN_REQUIRED: err << "BeginSetSites() MUST be called before calling SetOneSiteAt() !"; break;
		case SET_SITES_BAD_INDEX: 			err << "The index must be in range [0, max_size-1] - See BeginSetSites(int max_size) !"; break;
		case SET_SITES_BAD_NODE_INDEX: 		err << "The given node index must be in range [0, network_size - 1] !"; break;
		case SET_SITES_UNSET_FOUND: 		err << "All the sites in range [0, max_size-1] must have been set before ending : site '" << *((int *)args) << "' unset - See SetOneSiteAt() !"; break;
		case SET_SITES_NOT_ENDED: 			err << "A BeginSetSites() call has not been ended - See EndSetSites() !"; break;

		default: err << "Unknown error type !"; break;
		}
		cerr << err.str() << endl;

		// Throw the exception
#ifdef __WIN32
		// Force unexpected to trigger !
		// TODO : (should have been a more subtle system...)
		std::set_unexpected(unexpected_error);
#else
		std::runtime_error(err.str());
#endif

	}

	/**
	 * Minimalist callback for "set_unexpected"
	 */
	static void unexpected_error() {
		throw std::runtime_error("Handled Exception !");
	}
};


} /* namespace elps */
#endif /* LABCONSTS_H_ */
