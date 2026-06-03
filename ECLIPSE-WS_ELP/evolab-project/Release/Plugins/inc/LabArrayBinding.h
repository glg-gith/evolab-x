/*
 * LabArrayBinding.h
 *
 *  Created on: Feb 26, 2013
 *      Author: golgauth
 */

#ifndef LABARRAYBINDING_H_
#define LABARRAYBINDING_H_


#include <LabConsts.h>

#include <LabBindingBase.h>

#include <LabSiteBase.h>

#include <LabOutputerLine.h>

namespace elps {

/**
 * This class embeds a list of sites and provides features to populate it
 * (for use in a <b>MASS ACTION</b> context).
 *
 * All the <b>*Array</b> sub-classes of the simulator (See LabSimulatorBase)
 * also inherit from LabArrayBinding (See, for example,
 * LabSimulatorTimeArray for an illustration of this).
 *
 * USAGE :
 * =======
 *
 * First you will need an <b>array based</b> simulator :
 *
 * \code
 * 	int pop_max = 10000;
 * 	LabSimulatorTimeArray *sim = LabSimulatorTimeArray();        // Get yourself a simulator
 * 	sim->SetMaxPopulationSize(pop_max);                          // Set the simulator's capacity
 * \endcode
 *
 * Then fill the array binding with the corresponding number of sites.
 * The array must contain as many Sites as the simulator's capacity (10000 in the above
 * example).
 * For this you can use ready made ones (ex. LabSiteBase, ... or derivatives) and
 * assign them manually. But, LabArrayBinding provides methods to ease the filling
 * of the array from <b>sites model objects</b> (See LabArrayBinding.Populate() functions).
 *
 * Another big advantage of using those methods is that it turns on the <b>Sets Manager</b>
 * (See LabSiteSetsCalculator, LabSiteSetsManager ...).
 *
 * The usages are pretty much same as in LabNetBinding.
 *
 * <b>The half-manual way :</b>
 *
 * \code
 * 	sim->BeginSetSites(pop_max);                                // Mandatory to open the sites in writing
 * 	...
 * 	LabSiteBase *a_site = new LabSiteBase(sim);                 // Create a Site
 * 	sim->SetOneSiteAt(a_site, 0);                               // Mandatory to assign a site to a specified location in the
 * 	...                                                         // array (this enables the Sets Manager to monitor the Site)
 * 	...
 * 	...                                                         // Repeat the above for EVERY single site
 * 	...
 * 	sim->EndSetSites();                                         // Close the sites in writing
 * \endcode
 *
 * This allows to customize each site on the fly, but keep in mind that every single site in the array
 * has to be assigned before closure.
 *
 * <b>The automated way :</b>
 *
 * \code
 * 	vector<LabSiteBase *> models;
 * 	LabSiteBase *a_model = new LabSiteBase(sim);
 * 	model_sites.append(a_model);                                // Do this for each different kind of site you need
 * 	...
 * 	int d[] = {2000, 8000};                                     // Define a distribution (here 2 models to be spread)
 * 	vector<int> dist(d, d + sizeof(d) / sizeof(int) );          // Batch assignment (spread) of the sites over the network
 * 	sim->Populate(nb_sites, dist, model_sites, true);
 * \endcode
 *
 * <b>Note :</b> LabArrayBinding::Populate() functions take in charge
 * everything for you. Your role is restrained to the creation of the models, and the choice
 * of the spreading method.
 *
 */
class LIB_CLASS LabArrayBinding {
public:
	LabArrayBinding();
	virtual ~LabArrayBinding();


	// TODO : Fonction chapeau qui fait le café :
	//        void Populate(int total_nb_sites, int net_connection_type, [site_model, nb]);
	void Populate(int max_size, LabSiteBase *default_site, bool draw_progess = false);
	/**
	 * Populate randomly according to the given distribution and the models.
	 * @param max_size Maximum number of sites in the population (implies that all the 'max_size'
	 *                 sites have been instantiated before running properly the simulation)
	 * @param dist Cumulative sum must be 1.0.
	 * @param model_sites Vector of models / template sites.
	 * @param draw_progress If the progress bar must be displayed while populating.
	 */
	void Populate(int max_size, vector<double>& dist, vector<LabSiteBase *>& model_sites, bool draw_progress);
	/**
	 * Populate with exactly the given amounts given by dist and the models.
	 * @param max_size Maximum number of sites in the population (implies that all the 'max_size'
	 *                 sites have been instantiated before running properly the simulation)
	 * @param dist Cumulative sum must be the maximum population size.
	 * @param model_sites Vector of models / template sites.
	 * @param draw_progress If the progress bar must be displayed while populating.
	 */
	void Populate(int max_size, vector<int>& dist, vector<LabSiteBase *>& model_sites, bool draw_progress);

	/**
	 * Adds a single site to the sets manager.
	 * A call to this function must be enclosed between BeginSetSites(int max_size) and EndSetSites() calls.<br/>
	 * WARNING : All sites must be set before ending - See EndSetSites().
	 * @param a_site Site to be added to the binding's list.
	 * @param index Position in the list of the Site to be set (or replaced).
	 */
	void SetOneSiteAt(LabSiteBase *a_site, int index);
	/**
	 * Starts populating with a given number of sites.<br/>
	 * WARNING : This will reset all the sets that have been set earlier.<br/>
	 * Must be called before to start adding sites with SetOneSiteAt().
	 * @param max_size Maximum number of sites in the population (including empty sites).
	 */
	void BeginSetSites(int max_size);
	/**
	 * Ends populating sites operation. Checks if all sites have been set. If not, terminates the program.
	 * All 'max_size' sites must have been set before ending (See BeginSetSites(int max_size)).
	 */
	void EndSetSites();

//	void SetSite(LabSiteBase *src_site);
//	void UnSetSite(LabSiteBase *src_site);

	// TODO : Untested
	void CopySite(LabSiteBase *dest_site, LabSiteBase *src_site);
	// TODO : Untested
	void SwapSite(LabSiteBase *l_site, LabSiteBase *r_site);

	/**
	 * Gets the size of the binding's list (total number of Sites handled).
	 */
	int GetSize();
	/**
	 * Gets the list of all the binded Sites.
	 */
	LabSiteBase **GetSites();

private:

	////vector<LabSiteBase *> sites;

	LabSiteBase *default_site_;

	SiteSetsMgr *setsManager;

	LabRng *rng_;

	unsigned int max_size_;

};

} /* namespace elps */
#endif /* LABARRAYBINDING_H_ */
