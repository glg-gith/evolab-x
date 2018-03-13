/*
 * LabSiteSetsCalculator.h
 *
 *  Created on: Feb 19, 2013
 *      Author: golgauth
 */

#ifndef LABSITESETSCALCULATOR_H_
#define LABSITESETSCALCULATOR_H_


#include <LabConsts.h>

#include <LabSiteSetsManager.h>
#include <LabSiteBase.h>


namespace elps {


//class LabSetSite: public LabSet<LabSiteBase *> {
//	//
//};

/**
 * This class gives methods to deal with the "Sets" of Sites handled by
 * the <b>Sets Manager</b> (See LabSiteSetsManager).<br/>
 *
 * It allows to :
 * 	- Get a set (See GS())
 * 	- Get a site (See GI())
 * 	- Perform basic operations on the sites of a set (See AVG(), ...)
 *
 * USAGE :
 * =======
 *
 * \code
 * 	LabSiteSetsCalculator *_C = LabSiteSetsCalculator();        // Get a calculator
 *
 * 	LabSet A = _C.GS("SET_A");                                  // Get a set from the manager
 * 	LabSet B = _C.GS("SET_B");
 *
 * 	LabSet C = A + B;                                           // Get the Union
 *
 * 	LabSiteBase *S = _C.GI(12);                                 // Get '12'th site (from manager's list of sites)
 *
 * 	A.Set(12);                                                  // Put the '12'th site in set 'A'
 * 	A.Set(84);
 *
 * 	double avg = _C.AVG(A, LabSiteBase::IND, 0);                // Compute the average of attribute '0' aver all set 'A'
 * \endcode
 *
 */
class LIB_CLASS LabSiteSetsCalculator {
public:

	/**
	 * Default constructor.
	 */
	LabSiteSetsCalculator();
	virtual ~LabSiteSetsCalculator();

	/**
	 * Get a Set from the sets manager (See LabSiteSetsManager).
	 * @param a_set_name The name of the set.
	 * @return The Set object corresponding to that name.
	 */
    LabSet& 	  GS(string a_set_name);

    /**
     * Get a Site from the sites list of the LabSiteSetsManager (in the order
     * it has been added to the manger ! See LabUserData.SetId() and LabSiteSetsManager.AddSite(),
     * which are used in the various "Populate()" functions available from LabArrayBinding and
     * LabNetBinding).
     * @param id The index in the sites list.
     * @return Returns a pointer on the Site found at this index.
     */
    LabSiteBase * GI(int id);

    /**
     * Get the total number of sites registered in the manager (Basically the maximum size of the
     * whole individuals global international population).
     * @return The capacity of the manager (number of sites being managed).
     */
    int GetSitesCount();

    /**
     * Count.
     * @param a_set
     * @return
     */
    int			CNT(LabSet a_set);
    /**
     * Average.
     * @param a_set
     * @param dep_type
     * @param attr_id
     * @return
     */
    double		   AVG(LabSet a_set, LabSiteBase::t_deptype dep_type, int attr_id);
    inline double  AVG(LabSet a_set, int dep_type, int attr_id) { return AVG(a_set, (int)dep_type, attr_id); };
    /**
     * Multiple average.
     * @param a_set
     * @param dep_type
     * @param attr_ids List of the indices of the attributes to be averaged.
     * @return A list of the averages in the same order of the list of indices.
     */
    vector<double>      AVG(LabSet a_set, vector<int>& dep_types, vector<int>& attr_ids);
    /**
     * Lazy average (considers that all the individuals of the population have
     * the same value for the given attribute).
     * @param a_set
     * @param dep_type
     * @param attr_id
     * @return
     */
    double		        LAVG(LabSet a_set, LabSiteBase::t_deptype dep_type, int attr_id);
    inline double		LAVG(LabSet a_set, int dep_type, int attr_id) { return LAVG(a_set, (int)dep_type, attr_id); };
    /**
     * Random average (considers that all the individuals of the population have
     * the value of one individual randomly chosen among the set).
     * @param a_set
     * @param dep_type
     * @param attr_id
     * @return
     */
    double		        RAVG(LabSet a_set, LabSiteBase::t_deptype dep_type, int attr_id);
    inline double		RAVG(LabSet a_set, int dep_type, int attr_id) { return RAVG(a_set, (int)dep_type, attr_id); };
    /**
     * Covariance.
     * @param a_set
     * @param dep_type_1
     * @param attr_id_1
     * @param dep_type_2
     * @param attr_id_2
     * @return
     */
    double		COV(LabSet a_set, LabSiteBase::t_deptype dep_type_1, int attr_id_1,
    											 LabSiteBase::t_deptype dep_type_2, int attr_id_2);
    /**
     * Correlation coeff.
     * @param a_set
     * @param dep_type_1
     * @param attr_id_1
     * @param dep_type_2
     * @param attr_id_2
     * @return
     */
    double 		COR(LabSet a_set, LabSiteBase::t_deptype dep_type_1, int attr_id_1,
												 LabSiteBase::t_deptype dep_type_2, int attr_id_2);
    /**
     * Return an array of two doubles (one standard deviation per attribute).
     * @param a_set
     * @param dep_type_1
     * @param attr_id_1
     * @param dep_type_2
     * @param attr_id_2
     * @return
     */
    double* 	DEV(LabSet a_set, LabSiteBase::t_deptype dep_type_1, int attr_id_1,
												 LabSiteBase::t_deptype dep_type_2, int attr_id_2);

    /*
     * Neighbors stuffs (ONLY available if the LabSiteBase (or a derived class)
     * was created with a NON-NULL "parent").
     */

    //vector<LabNode *>  		GET_NGH_1(LabSiteBase *ind, int state, int ngh_depth = 1);
    /**
     * Get the number of direct neighbors for a given state
     * @param ind
     * @param state
     * @return
     */
    int 		CNT_NGH(LabSiteBase *ind, int state);
    /**
     * Get the number of direct neighbors for a given indexed attribute.
     * @param ind
     * @param dep_type
     * @param indexed_attr_id
     * @param attr_value
     * @return
     */
    int 		CNT_NGH(LabSiteBase *ind, LabSiteBase::t_deptype dep_type, int indexed_attr_id, unsigned int attr_value);
    inline int  CNT_NGH(LabSiteBase *ind, int dep_type, int indexed_attr_id, unsigned int attr_value) { return CNT_NGH(ind, (int)dep_type, indexed_attr_id, attr_value); };
    /**
     * Get the average of a given attribute in the direct neighborhood.
     * @param ind
     * @param dep_type
     * @param attr_id
     * @return
     */
    double 		  AVG_NGH(LabSiteBase *ind, LabSiteBase::t_deptype dep_type, int attr_id);
    double inline AVG_NGH(LabSiteBase *ind, int dep_type, int attr_id) { return AVG_NGH(ind, (int)dep_type, attr_id); };

private:
    SiteSetsMgr *setsManager;

    LabRng *rng;
};


} /* namespace elps */
#endif /* LABSITESETSCALCULATOR_H_ */
