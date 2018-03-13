/*
 * LabNetBinding.h
 *
 *  Created on: 14 sept. 2012
 *      Author: golgauth
 */

#ifndef LABNETBINDING_H_
#define LABNETBINDING_H_

#include "LabNetwork.h"


#include <LabConsts.h>

#include <LabSiteSetsManager.h>

#include <LabUserData.h>
#include <LabSiteBase.h>
#include <LabSiteEvent.h>

#include <LabOutputerLine.h>


#include <LabRngUtils.h>


#include <iostream>

#include <typeinfo>

using namespace std;



namespace elps {

/**
 * This class embeds a network (See LabNetwork) and provides features
 * to populate it (for use in a <b>SPATIALIZED</b> context). As well as short access
 * features to backup and restore the network (using text formatted files).
 *
 * All the <b>*Net</b> sub-classes of the simulator (See LabSimulatorBase)
 * also inherit from LabNetBinding (See, for example,
 * LabSimulatorTimeNet for an illustration of this).
 *
 * USAGE :
 * =======
 *
 * First you will need a network and a simulator :
 *
 * \code{.cpp}
 * 	int pop_max = 10000;
 * 	LabNetwork *net = new LabNetwork();                      // Get yourself a network
 * 	net.Populate(pop_max);                                   // Populate the network with nodes
 * 	...
 * 	LabSimulatorTimeNet *sim = new LabSimulatorTimeNet();    // Get yourself a simulator
 * 	sim.SetMaxPopulationSize(pop_max);                       // Set the simulator's capacity
 * 	sim.netBinding.SetNetwork(net);                          // Assign the network to the simulator
 * \endcode
 *
 * Then, as explained in LabNode, you'll need object(s) representing each Site.
 * Each node needs a Site.
 * For this you can use ready made ones (ex. LabSiteBase, ... or derivatives) and
 * assign them manually (See LabNode::SetUserData()). But, LabNetBinding provides
 * methods to ease the assignment of <b>sites model objects</b> to the nodes
 * (See LabNetBinding::Populate() functions).
 *
 * Another big advantage of using those methods is that it turns on the <b>Sets Manager</b>
 * (See LabSiteSetsCalculator, LabSiteSetsManager...).
 *
 * <b>The half-manual way :</b>
 *
 * \code
 * 	sim->BeginSetSites();                                     // Mandatory to open the sites in writing
 * 	...
 * 	LabSiteEvent *a_site = new LabSiteEvent(sim, a_node);     // Create a Site
 * 	sim->SetOneSiteAt(a_site, a_node.GetIndex());             // Mandatory to assign a site to a node
 * 	...                                                       // (this enables the Sets Manager to monitor the Site)
 * 	...
 * 	...                                                       // Repeat the above for EVERY single site
 * 	...
 * 	sim->EndSetSites();                                       // Close the sites in writing
 * \endcode
 *
 * This allows to customize each node on the fly, but keep in mind that every single node
 * has to be assigned before closure.
 *
 * <b>The automated way :</b>
 *
 * \code
 * 	vector<LabSiteEvent *> models;
 * 	LabSiteEvent *a_model = new LabSiteEvent(sim);
 * 	model_sites.push_back(a_model);                           // Do this for each different kind of site you need
 * 	...
 * 	double d[] = {0.2, 0.8};                                  // Define a distribution (here 2 models to be spread)
 * 	vector<double> dist(d, d + sizeof(d) / sizeof(double) );
 * 	sim->Populate(dist, model_sites, true);                   // Batch assignment (spread) of the sites over the network
 * \endcode
 *
 * <b>Note :</b> LabNetBinding::Populate() functions take in charge
 * everything for you. Your role is restrained to the creation of the models, and the choice
 * of the spreading method.
 *
 */
class LIB_CLASS LabNetBinding {
public:
	/**
	 * Default constructor.
	 */
	LabNetBinding();
	virtual ~LabNetBinding();

	/**
	 * Get the related LabNetwork.
	 * @return A pointer on the network.
	 */
	LabNetwork *GetNetwork();
	/**
	 * Link the binder to a given network.
	 * @param net The LabNetwork to link to.
	 */
	void SetNetwork(LabNetwork *net);

//	// ... TODO : more or less complex factory functions		=> Done in the LabNetwork class now !
//	LabNetwork CreateNetWork();
//	LabNetwork CreateNetWork(int nb_ind);

	//void SetNodeState(stateType state);
	//void AddState(stateType state);

	/**
	 * Save the network to a text file (defining the edges connections).
	 * @param filename The path of the file to save to (can be relative).
	 */
	void SaveNetwork(string filename);
	/**
	 * Restore the network from a text file (defining the edges connections).
	 * @param filename The path of the file to load from (can be relative).
	 */
	void LoadNetwork(string filename);
//
//	void BackupNetwork(ofstream *ostream);
//	void UnBinarizeNetwork(ifstream *istream);


	// TODO : Fonction chapeau qui fait le café :
	//        void Populate(int total_nb_sites, int net_connection_type, [site_model, nb]);
	void Populate(LabSiteBase *default_site, bool draw_progess = false);
	/**
	 * Populate randomly according to the given distribution and the models.
	 * Note : dist and model_sites must be the same size.
	 * @param dist Cumulative sum must be 1.0.
	 * @param model_sites Vector of models / template sites.
	 * @param draw_progress If the progress bar must be displayed while populating.
	 */
	void Populate(vector<double>& dist, vector<LabSiteBase *>& model_sites, bool draw_progress);
	/**
	 * Populate with exactly the given amounts given by dist and the models.
	 * @param dist Cumulative sum must be network's size.
	 * @param model_sites Vector of models / template sites.
	 * @param draw_progress If the progress bar must be displayed while populating.
	 */
	void Populate(vector<int>& dist, vector<LabSiteBase *>& model_sites, bool draw_progress);

	/**
	 * Adds a single site to the sets manager.
	 * A call to this function must be enclosed between BeginSetSites(int max_size) and EndSetSites() calls.
	 * WARNING : All sites must be set before ending - See EndSetSites().
	 * @param a_site Site to be set.
	 * @param index Index at which the Site is to be set (regarding the list of sites of the sets manager).
	 * 				See also LabSiteSetsManager.
	 */
	void SetOneSiteAt(LabSiteBase *a_site, int node_index);
	/**
	 * Starts populating with a given number of sites.
	 * WARNING : This will reset all the sets that have been set earlier.
	 * Must be called before to start adding sites with SetOneSiteAt().
	 */
	void BeginSetSites();
	/**
	 * Ends populating sites operation. Checks if all sites have been set. If not, terminates the program.
	 * All network's sites must have been set before ending.
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
	LabNetwork *net;

	LabSiteBase *default_site_;

	SiteSetsMgr *setsManager;

	LabRng *rng_;

};


}  // namespace elps

#endif /* LABNETBINDING_H_ */
