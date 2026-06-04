/*
 * LabSetsBinding.h
 *
 *  Created on: Feb 11, 2013
 *      Author: golgauth
 */

#ifndef LABSETSBINDING_H_
#define LABSETSBINDING_H_


#include <string>
#include <vector>
#include <map>
#include <set>


//#include <LabSingleton.h>

#include <LabSet.h>
#include <LabConsts.h>

using namespace std;


namespace elps {

/**
 * Default UNIVERSE Set (reserved Set name)
 * All the sites of the manager / simulation belongs to it.
 */
static const string U_SET = "U";
/** Default UNSET Set (reserved Set name)
 * Not in use, may be one day...
 */
static const string UNSET = "UNSET";



/**
 * Get the "Set Theory" manager (mainly used to group individuals in logical sets and subsets,
 * according to their state and attributes).
 */
template < typename T >
class LIB_CLASS LabSiteSetsManager/*: public LabSingleton*/ {
public:

	/** Type of storage used for the Sets */
	typedef typename map<string, LabSet *>::iterator t_set_iter;


	//LabSiteSetsManager();				//  Coz I'm a Singleton !
	/**
	 * Gets the instance of the sets manager.
	 * @return A sets manager.
	 */
    static LabSiteSetsManager *GetInstance()
    {
        if (instance == NULL) {
        	instance = new LabSiteSetsManager();
        }
        return instance;
    }

    /**
     * Add a site to the manager.
     * @param site The site to be added.
     * @return True if the site has been successfully added.
     */
    bool AddSite(T site);
    //    void SortSites();
    /**
     * Get a Site given its index in the manager's list of sites.
     * @param id The index of the site.
     * @return A site.
     */
    T GetAt(int id);
    /**
     * Get the current maximum capacity of the manager (number of sites)
     * @return The current number of sites.
     */
    int GetSitesCount();
    /**
     * Get the list of the sites (raw pointer, sorry...
     * Use a call to GetSitesCount() to obtain its size).
     * @return
     */
    T *GetSites();

    /**
     * Create a new Set
     * @param size Capacity of the new Set at creation time. (Can be resized
     *             afterwards - See ResizeAll()).
     * @param set_name The name for the new Set.
     * @return True if everything went OK.
     */
    bool CreateSet(int size, string set_name);
    /**
     * Test if a given Set exists.
     * @param set_name Name of the Set to look for.
     * @return True if the Set was found.
     */
    bool SetExists(string set_name);
    /**
     * Register a Site to the specified Set.
     * @param set_name Name of the Set to subscribe the site to.
     * @param site Site to be registered.
     * @param id Index of the site in the global manager's list.
     * @return True if everything went OK.
     */
    bool Subscribe(string set_name, T site, int id);
    /**
     * UnRegister a Site from the specified Set.
     * @param set_name Name of the Set to unsubscribe the site from.
     * @param site Site to be unregistered.
     * @param id Index of the site in the global manager's list.
     * @return True if everything went OK.
     */
    bool UnSubscribe(string set_name, T site, int id);

    /**
     * Get a set by its name
     * @param set_name Name of the Set to look for.
     * @return NULL if not a clue was found under that name !!
     */
    LabSet& GetSet(string set_name);

	//	int        CNT (LabSet<t_elt> const&);
	//	double     SUM (LabSet<t_elt> const&, string attr_name);
	//	double     AVG (LabSet<t_elt> const&, string attr_name);

    /**
     * Allows to resize the whole manager's capacity.
     * @param sizeinbits New capacity of the manager (number of sites it can manage).
     */
    void ResizeAll(unsigned int sizeinbits);

    /**
     * Print the list of created Sets.
     */
    void PrintSets();

    /**
     * WARNING : Internal usage only, USE IT ONLY IF KNOW WHAT TO DO WITH IT.<br/>
     * Lock / Unlock the sites manager for writing (new / erase) sites.
     * @param opened True to unlock.
     */
    void SetSitesOpened(bool opened);
    /**
     * Test if the manager is open for writing (new / erase) sites.
     * @return True if it is.
     */
    bool GetSitesOpened();

private:
    /** Unique instance of this manager */
	static LabSiteSetsManager *instance;

	// Singleton : so constructor and desctructor are private
	/**
	 * Default constructor forbidden.
	 */
	LabSiteSetsManager() {
		sets.insert(pair<string, LabSet *>(U_SET, new LabSet(0)));
		cout << "Created set : \"U\" (universe) !" << endl;
		sets.insert(pair<string, LabSet *>(UNSET, new LabSet(0)));
		cout << "Created set : \"UNSET\" !" << endl;
		sites = NULL;

		set_sites_opened_ = false;
	}
    virtual ~LabSiteSetsManager() { }
    /**
     * Copy constructor forbidden.
     * @param
     */
	LabSiteSetsManager(LabSiteSetsManager const&) { }        	// Don't Implement
	/**
	 * Assignment operator forbidden.
	 * @param
	 */
    void operator=(LabSiteSetsManager const&) { }				// Don't implement

    ////map<string, LabSet<IndType> *> sets;
    /** List of the Sets */
	map<string, LabSet *> sets;

	/** Array of the sites */
	T *sites;
	/** Total capacity */
	unsigned int sizeinbits_;

	/** If the manager is opened for writing (setting sites) */
	bool set_sites_opened_;
};



template < typename T >
LabSiteSetsManager<T>* LabSiteSetsManager<T>::instance = NULL;


template < typename T >
bool LabSiteSetsManager<T>::AddSite(T site) {
	////if (std::find(sites.begin(), sites.end(), site) == sites.end()) {
		//site->SetId(individuals.size());

		////sites.push_back(site);
		sites[site->GetId()] = site;

		// Done in Populate() now !!!
		//ResizeAllSets(individuals.size());
		//ResizeAllSets(100000);

		// Automatically subscribe to "U" (the universe)
		Subscribe(U_SET, site, site->GetId());

		return true;
	////}
	return false;
}

//
//template < typename T >
//void LabSiteSetsManager<T>::SortSites() {
//	std::sort(sites.begin(), sites.end(), compare_id);
//}


template < typename T >
void LabSiteSetsManager<T>::ResizeAll(unsigned int sizeinbits) {

	if (sizeinbits_ != sizeinbits) {

		sizeinbits_ = sizeinbits;
		t_set_iter it = sets.begin();
		while (it != sets.end()) {
			(*it).second->Resize(sizeinbits_);
			it++;
		}
		if (sites) delete [] sites;
		sites = new T[sizeinbits_]; //(T*)malloc(sizeinbits_ * sizeof(T)); // .resize(sizeinbits);
		std::fill_n(sites, sizeinbits_, (T)0); // Nullify the sites
	}
}

template < typename T >
T LabSiteSetsManager<T>::GetAt(int id) {
	if (id < 0 || id >= GetSitesCount()) { return 0; }
	return sites[id]; //.at(id);
}

template < typename T >
int LabSiteSetsManager<T>::GetSitesCount() {
	return sizeinbits_; //sites.size();
}

template < typename T >
T *LabSiteSetsManager<T>::GetSites() {
//	vector<T> ret;
//	for (unsigned int i = 0; i < sizeinbits_; i++) {
//		ret.push_back(sites[i]);
//	}
//	return ret;
	return sites;
}

template < typename T >
bool LabSiteSetsManager<T>::CreateSet(int size, string set_name) {

	// Forbidding using reserved name "U"
	if (set_name == U_SET) { LabConsts::E(LabConsts::U_SET_RESERVED); }
	//else if (set_name == "UNSET") { LabConsts::E(LabConsts::UNSET_SET_RESERVED); }
	else if (set_name != "")
	{
		//cout << "Creating set : " << set_name << endl;

		t_set_iter it = sets.find(set_name);
		// Set exists
		if (it != sets.end()) { /*cout << "Set already exists : " << set_name << endl;*/ return false; }
		// Set not yet created
		sets.insert(pair<string, LabSet *>(set_name, new LabSet(size)));

		cout << "Created set : " << set_name << endl;
	} else {
		LabConsts::E(LabConsts::INVALID_SET_NAME);
	}
	return true;
}

template < typename T >
bool LabSiteSetsManager<T>::SetExists(string set_name) {
	return (sets.find(set_name) != sets.end());
}

template < typename T >
bool LabSiteSetsManager<T>::Subscribe(string set_name, T site, int id) {
	t_set_iter it = sets.find(set_name);
	// Set does not exist
	if (it == sets.end()) { return false; }
	// Add site to set (if set exists)
	(*it).second->Set(id);
	////cout << ">>>  Current sets : "; this->PrintSets();
	return true;
}

template < typename T >
bool LabSiteSetsManager<T>::UnSubscribe(string set_name, T site, int id) {
	t_set_iter it = sets.find(set_name);
	// Set does not exist
	if (it == sets.end()) { return false; }
	// Remove site from set
	(*it).second->UnSet(id);
	////cout << ">>>  Current sets : "; this->PrintSets();
	return true;
}


template < typename T >
LabSet& LabSiteSetsManager<T>::GetSet (string set_name) {
//	// TODO : Do we really need this "find" check ? Is it time consuming ?
//	t_set_iter it = sets.find(set_name);
//	// Set does not exist
//	if (it == sets.end()) { return 0; }
	return *(sets.at(set_name));//sets.at(set_name);////->GetSet();
}

template < typename T >
void LabSiteSetsManager<T>::PrintSets() {
	t_set_iter it;
	cout << "{";
	for (it = sets.begin(); it != sets.end(); ) {
		cout << " " << (*it).first << "|" << (*it).second->Cardinality();
		std::advance(it, 1);
	}
	cout << " }" << endl;
}


template < typename T >
void LabSiteSetsManager<T>::SetSitesOpened(bool opened) {
	this->set_sites_opened_ = opened;
}
template < typename T >
bool LabSiteSetsManager<T>::GetSitesOpened() {
	return this->set_sites_opened_;
}

} /* namespace elps */
#endif /* LABSETSBINDING_H_ */
