/*
 * LabArrayBinding.cpp
 *
 *  Created on: Feb 26, 2013
 *      Author: golgauth
 */

#include "LabArrayBinding.h"

namespace elps {

LabArrayBinding::LabArrayBinding() {
	setsManager = SiteSetsMgr::GetInstance();
	this->rng_ = new LabRng();

}

LabArrayBinding::~LabArrayBinding() {
	// TODO Auto-generated destructor stub
}


void LabArrayBinding::Populate(int max_size, vector<double>& dist, vector<LabSiteBase *>& model_sites, bool draw_progress) {


	max_size_ = max_size;

	//this->SetMaxPopulationSize(this->net->GetSize());

	cout << "Populating..." << endl;

	if (dist.size() != model_sites.size()) {
		LabConsts::E(LabConsts::MALFORMED_MODELS_DISTRIBUTION);
		return;
	}
	double sum_dist = 0.0;
	for (unsigned int i = 0; i< dist.size(); i++) {
		sum_dist += dist[i];
	}
	if (sum_dist != 1.0) {
		LabConsts::E(LabConsts::MALFORMED_DISTRIBUTION);
		return;
	}

	setsManager->ResizeAll(max_size_);
	//default_site_ = default_model_site;
	// Populate

	LabSiteBase *new_site;
	int model_id;
	LabSiteBase *a_model;
	for ( unsigned int i = 0; i < max_size_; i++ ) {

		// Choose model from distribution
		model_id = rng_->GetNextIntMultinomial(1.0, dist);
		a_model = model_sites[model_id];
		new_site = (LabSiteBase *)(a_model->New(NULL));

		new_site->SetId(i);
		new_site->Copy( a_model );

		// Add site to manager
		setsManager->AddSite(new_site);

		// Subscribe the newly created site to the 'UNSET' set (reserved name)
		//SetsManager::GetInstance()->Subscribe("UNSET", new_site, new_site->GetId());
		////new_site->SubscribeSet(UNSET);

		// Subscribe to the related sets
		//new_site->SubscribeAllSets(); // Done in LabSiteBase::Copy() now !!

		//new_site->PrintSets();

		if ( draw_progress && (!(i % 1000) || (i == max_size_-1)) )
			LabOutputerLine::DrawProgressBar(50, i / (double)(max_size_-1), true);

	}

	cout << endl << "Populating... Done !" << endl;

}

void LabArrayBinding::Populate(int max_size, vector<int>& dist, vector<LabSiteBase *>& model_sites, bool draw_progress) {


	max_size_ = max_size;

	//this->SetMaxPopulationSize(this->net->GetSize());

	cout << "Populating..." << endl;

	if (dist.size() != model_sites.size()) {
		LabConsts::E(LabConsts::MALFORMED_MODELS_DISTRIBUTION);
		return;
	}
	double sum_dist = 0.0;
	for (unsigned int i = 0; i< dist.size(); i++) {
		sum_dist += dist[i];
	}
	if (sum_dist != max_size_) {
		LabConsts::E(LabConsts::MALFORMED_DISTRIBUTION_NET);
		return;
	}
	//cout << "SUM = " << sum_dist << " | max size = " << max_size_ << endl;

	setsManager->ResizeAll(max_size_);
	//default_site_ = default_model_site;

	// Populate
	unsigned int id = 0;
	LabSiteBase *new_site;
	LabSiteBase *a_model;
	for (unsigned int i=0; i<dist.size(); i++) {
		a_model = model_sites[i];
		for (int j=0; j<dist[i]; j++) {
			new_site = (LabSiteBase *)(a_model->New(NULL));

			new_site->SetId(id);
			new_site->Copy( a_model );

			// Add site to manager
			setsManager->AddSite(new_site);

			// Subscribe the newly created site to the 'UNSET' set (reserved name)
			//SetsManager::GetInstance()->Subscribe("UNSET", new_site, new_site->GetId());
			////new_site->SubscribeSet(UNSET);

			// Subscribe to the related sets
			//new_site->SubscribeAllSets(); // Done in LabSiteBase::Copy() now !!

			if ( draw_progress && (!(id % 1000) || (id == max_size_-1)) )
				LabOutputerLine::DrawProgressBar(50, id / (double)(max_size_-1), true);

			id++;
		}
	}

	cout << endl << "Populating... Done !" << endl;

}


void LabArrayBinding::SetOneSiteAt(LabSiteBase *a_site, int index) {

//	max_size_ = max_size;
//	setsManager->ResizeAll(max_size_);
	if (!setsManager->GetSitesOpened()) {
		LabConsts::E(LabConsts::SET_SITES_CALL_BEGIN_REQUIRED);
		return;
	}
	if (index < 0 || index >= max_size_) {
		LabConsts::E(LabConsts::SET_SITES_BAD_INDEX);
		return;
	}

	LabSiteBase *new_site;

	// Generate a 'set trackable' site from model
	new_site = (LabSiteBase *)(a_site->New(NULL));

	new_site->SetId(index);
	new_site->Copy( a_site );

	// Add site to manager
	setsManager->AddSite(new_site);
}


void LabArrayBinding::BeginSetSites(int max_size) {
	max_size_ = max_size;
	setsManager->ResizeAll(max_size_);
	setsManager->SetSitesOpened(true);
}

void LabArrayBinding::EndSetSites() {
	bool all_set = true;
	unsigned int i;
	for (i=0; i<max_size_; i++)
	{
		if ( !(setsManager->GetAt(i)) ) {
			all_set = false;
			break;
		}
	}
	if (!all_set) {
		LabConsts::E(LabConsts::SET_SITES_UNSET_FOUND, &i);
		return;
	}
	setsManager->SetSitesOpened(false);
}



// UNTESTED !!!
void LabArrayBinding::CopySite(LabSiteBase *dest_site, LabSiteBase *src_site) {
	//dest_site->UnSubscribeAllSets(); // Done in LabSiteBase::Copy() now !!
	dest_site->Copy(src_site);
	//dest_site->SubscribeAllSets();
}

// UNTESTED !!!
void LabArrayBinding::SwapSite(LabSiteBase *l_site, LabSiteBase *r_site) {
//	l_site->UnSubscribeAllSets(); // Done in LabSiteBase::Copy() now !!
//	r_site->UnSubscribeAllSets();

	LabSiteBase *tmp_site = new LabSiteBase(NULL);////(LabSiteBase *)(l_site->New(l_site->parent));
	tmp_site->Copy(l_site);
	l_site->Copy(r_site);
	r_site->Copy(tmp_site);

	////tmp_site->UnSubscribeAllSets();

//	l_site->SubscribeAllSets();
//	r_site->SubscribeAllSets();
}


int LabArrayBinding::GetSize() {
	return this->setsManager->GetSitesCount();
}

LabSiteBase **LabArrayBinding::GetSites() {
	return this->setsManager->GetSites();
}



} /* namespace elps */
