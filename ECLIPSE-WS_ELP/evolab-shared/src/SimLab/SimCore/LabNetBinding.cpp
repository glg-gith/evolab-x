/*
 * LabNetBinding.cpp
 *
 *  Created on: 14 sept. 2012
 *      Author: golgauth
 */

#include "LabNetBinding.h"


namespace elps {


LabNetBinding::LabNetBinding() {
	setsManager = SiteSetsMgr::GetInstance();
	this->rng_ = new LabRng();
}

LabNetBinding::~LabNetBinding() {
	// TODO Auto-generated destructor stub
}


LabNetwork *LabNetBinding::GetNetwork() {
	return this->net;
}

void LabNetBinding::SetNetwork(LabNetwork *net) {
	this->net = net;
	//this->SetPopulationSize(this->net->GetSize());
}



void LabNetBinding::SaveNetwork(string filename)
{
	this->net->write_edgelist(filename);
}

void LabNetBinding::LoadNetwork(string filename)
{
	//if (this->net != NULL) delete(this->net);
	this->net = new LabNetwork();
	this->net->read_edgelist(filename, ',');
}

//---

//#include <LabTimeUtils.h>
//
//LabTimer *t = new LabTimer();
//
//void LabNetBinding::Populate(LabSiteBase *default_model_site, bool draw_progress) {
//
//	//this->SetPopulationSize(this->net->GetSize());
//
//	cout << "Populating..." << endl;
//
//
//	if (default_model_site != NULL)
//	{
//		setsManager->ResizeAll(this->net->GetSize());
//		default_site_ = default_model_site;
//		// Populate
//		int id = 0;
//		vector<LabNode*>::iterator it;
//		for ( it = this->net->GetNodes().begin(); it != this->net->GetNodes().end(); it++ ) {
//
//			LabSiteBase *new_site = (LabSiteBase *)(default_model_site->New((LabNode *)(*it)));
//
//			//t->Start();
//			new_site->Copy( default_model_site );
//			//t->Stop(); t->Print();
//
//			new_site->SetId(id);
//			// Set user data
//			(*it)->SetUserData(new_site);
//			// Add site to manager
//			setsManager->AddSite(new_site);
//			// Subscribe the newly created site to the 'UNSET' set (reserved name)
//			//SetsManager::GetInstance()->Subscribe("UNSET", new_site, new_site->GetId());
//			new_site->SubscribeSet(UNSET);
//
//			//new_site->PrintSets();
//
//			if ( draw_progress && (!(id % 1000) || (id == this->net->GetSize()-1)) )
//				LabOutputerLine::DrawProgressBar(50, id / (double)(this->net->GetSize()-1), true);
//
//			++id;
//		}
//	}
//	else
//	{
//		throw "The default site cannot be 'NULL' !";
//	}
//
//	cout << endl << "Populating... Done !" << endl;
//}


void LabNetBinding::Populate(vector<double>& dist, vector<LabSiteBase *>& model_sites, bool draw_progress) {

	//this->SetMaxPopulationSize(this->net->GetSize());

	cout << "Populating..." << endl;

	if (dist.size() != model_sites.size() || dist.size() == 0) {
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

	cout << "Populating...1" << endl;
	setsManager->ResizeAll(this->net->GetSize());
	//default_site_ = default_model_site;
	// Populate
	int id = 0;
	LabSiteBase *new_site;
	int model_id;
	LabSiteBase *a_model;
	vector<LabNode*>::iterator it;
	for ( it = this->net->GetNodes().begin(); it != this->net->GetNodes().end(); it++ ) {

		if ( (id == 0) || (draw_progress && ((id == 0) || !(id % 1000) || (id == this->net->GetSize()-1))) )
			LabOutputerLine::DrawProgressBar(50, id / (double)(this->net->GetSize()-1), true);

		// Choose model from distribution
		model_id = rng_->GetNextIntMultinomial(1.0, dist);
		a_model = model_sites[model_id];
		new_site = (LabSiteBase *)(a_model->New(*it));

		//t->Start();
		new_site->SetId((*it)->get_id());
		new_site->Copy( a_model );
		//t->Stop(); t->Print();

		// Set user data
		(*it)->SetUserData(new_site);
		// Add site to manager
		setsManager->AddSite(new_site);

		// Subscribe the newly created site to the 'UNSET' set (reserved name)
		//SetsManager::GetInstance()->Subscribe("UNSET", new_site, new_site->GetId());
		////new_site->SubscribeSet(UNSET);

		// Subscribe to the related sets
		//new_site->SubscribeAllSets(); // Done in LabSiteBase::Copy() now !!

		//new_site->PrintSets();

		++id;
	}

	cout << endl << "Populating... Done !" << endl;

//	for ( it = this->net->GetNodes().begin(); it != this->net->GetNodes().end(); it++ )
//	{
//		((LabSiteBase *)(*it)->GetUserData())->PrintSets();
//	}
}

void LabNetBinding::Populate(vector<int>& dist, vector<LabSiteBase *>& model_sites, bool draw_progress) {

	//this->SetMaxPopulationSize(this->net->GetSize());

	cout << "Populating..." << endl;

	if (dist.size() != model_sites.size() || dist.size() == 0) {
		LabConsts::E(LabConsts::MALFORMED_MODELS_DISTRIBUTION);
		return;
	}
	double sum_dist = 0.0;
	for (unsigned int i = 0; i< dist.size(); i++) {
		sum_dist += dist[i];
	}
	if (sum_dist != this->net->GetSize()) {
		LabConsts::E(LabConsts::MALFORMED_DISTRIBUTION_NET);
		return;
	}

	setsManager->ResizeAll(this->net->GetSize());
	//default_site_ = default_model_site;


	// Not yet treated sites indices
	vector<int> indices;
	for (int i=0; i<this->net->GetSize(); i++) { indices.push_back(i); }

	// Populate
	int k;
	int id = 0;
	LabSiteBase *new_site;
	LabSiteBase *a_model;
	LabNode *a_node;
	for (unsigned int i=0; i<dist.size(); i++) {
		a_model = model_sites[i];
		for (int j=0; j<dist[i]; j++) {
			k = rng_->GetNextInt(indices.size());
			a_node = this->net->GetNodes()[indices[k]];

			new_site = (LabSiteBase *)(a_model->New(a_node));

			new_site->SetId(a_node->get_id());
			new_site->Copy( a_model );


			// Set user data
			a_node->SetUserData(new_site);
			// Add site to manager
			setsManager->AddSite(new_site);

			// Subscribe the newly created site to the 'UNSET' set (reserved name)
			//SetsManager::GetInstance()->Subscribe("UNSET", new_site, new_site->GetId());
			////new_site->SubscribeSet(UNSET);

			// Subscribe to the related sets
			//new_site->SubscribeAllSets(); // Done in LabSiteBase::Copy() now !!

			if ( draw_progress && (!(id % 1000) || (id == this->net->GetSize()-1)) )
				LabOutputerLine::DrawProgressBar(50, id / (double)(this->net->GetSize()-1), true);
			++id;


			indices.erase(indices.begin() + k);
		}
	}

	cout << endl << "Populating... Done !" << endl;

//	vector<LabNode*>::iterator it;
//	for ( it = this->net->GetNodes().begin(); it != this->net->GetNodes().end(); it++ )
//	{
//		((LabSiteBase *)(*it)->GetUserData())->PrintSets();
//	}

}


void LabNetBinding::SetOneSiteAt(LabSiteBase *a_site, int node_index) {

//	setsManager->ResizeAll(this->net->GetSize());
	if (!setsManager->GetSitesOpened()) {
		LabConsts::E(LabConsts::SET_SITES_CALL_BEGIN_REQUIRED);
		return;
	}
	if (node_index < 0 || node_index >= this->net->GetSize()) {
		LabConsts::E(LabConsts::SET_SITES_BAD_NODE_INDEX);
		return;
	}

	LabNode *a_node = this->net->GetNodes()[node_index];
	LabSiteBase *new_site;

	// Generate a 'set trackable' site from model
	new_site = (LabSiteBase *)(a_site->New(a_node));

	new_site->SetId(a_node->get_id());
	new_site->Copy( a_site );

	// Set user data
	a_node->SetUserData(new_site);
	// Add site to manager
	setsManager->AddSite(new_site);
}

void LabNetBinding::BeginSetSites() {
	setsManager->ResizeAll(this->net->GetSize());
	setsManager->SetSitesOpened(true);
}

void LabNetBinding::EndSetSites() {
	bool all_set = true;
	int i;
	for (i=0; i<this->net->GetSize(); i++)
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
void LabNetBinding::CopySite(LabSiteBase *dest_site, LabSiteBase *src_site) {
	//dest_site->UnSubscribeAllSets(); // Done in LabSiteBase::Copy() now !!
	dest_site->Copy(src_site);
	//dest_site->SubscribeAllSets();
}

// UNTESTED !!!
void LabNetBinding::SwapSite(LabSiteBase *l_site, LabSiteBase *r_site) {
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

int LabNetBinding::GetSize() {
	return this->setsManager->GetSitesCount();
}

LabSiteBase **LabNetBinding::GetSites() {
	return this->setsManager->GetSites();
}


//void LabNetBinding::SetSite(LabSiteBase *model_site) {
//	if (model_site != NULL)
//	{
//		// Choose a site among the empty ones
//		// Get lucky...
//		////t->Start();
//		int unset_id = setsManager->GetSet(UNSET).ChooseOne();
//		////t->Stop(); t->Print();
//
//
//		LabSiteBase *target_site = (LabSiteBase*)(this->net->GetNodes()[unset_id])->GetUserData();
//		// Initialize it with the model
//		if (typeid(target_site).name() == typeid(default_site_).name()) {
//
//			// Set site
//			target_site->Copy(model_site);
//			//
//			target_site->SubscribeAllSets();
//			// Unsubscribe from "UNSET"
//			target_site->UnSubscribeSet(UNSET);
//
//			//target_site->PrintSets();
//		} else {
//			cerr << typeid(target_site).name() << " != " << typeid(default_site_).name() << endl;
//			throw ("The model/input site MUST be of type " + string(typeid(default_site_).name()));
//		}
//	}
//	else
//	{
//		throw "The 'model_site' cannot be 'NULL' !";
//	}
//}
//
//void LabNetBinding::UnSetSite(LabSiteBase *site) {
//	if (site != NULL)
//	{
//		// Un-subscribe from all but "U"
//		site->UnSubscribeAllSets();
//		// Return to default site state
//		site->Copy(default_site_);
//		// Re-subscribe to "UNSET" set
//		setsManager->Subscribe(UNSET, site, site->GetId());
//	}
//	else
//	{
//		cerr << "The 'site' to remove cannot be 'NULL' !" << endl;;
//		throw "The 'site' to remove cannot be 'NULL' !";
//	}
//}


//void LabNetBinding::BinarizeNetwork(ofstream *ostream)
//{
////    vector<Edge*> edges;
////    vector<Node*> node_list = this->net->get_nodes();
////    for (unsigned int i = 0; i < node_list.size(); i++) {
////        edges = node_list[i]->get_edges_out();
////        for (unsigned int e = 0; e < edges.size(); e++) {
////            int start_id = edges[e]->start->id;
////            int end_id   = edges[e]->end->id;
////            if (!this->net->is_directed() and start_id > end_id) continue;
////            if (!this->net->is_directed() and start_id == end_id) {
////                Edge* comp = edges[e]->get_complement();
////                if (edges[e]->id > comp->id) continue;
////            }
////            *ostream << start_id << end_id << endl;
////        }
////        if (node_list[i]->deg() == 0)
////        	*ostream << node_list[i]->id << endl;
////    }
//}

//void LabNetBinding::UnBinarizeNetwork(ifstream *istream)
//{
////    std::stringstream ss;
////    map<string,Node*> idmap;
////
////    if (istream->is_open())
////    {
////        string line;
////
////        while ( getline(myfile,line) ) {
////            //split string based on "," and store results into vector
////            vector<string> fields;
////            split(line, sep, fields);
////            const char whitespace[] = " \n\t\r";
////
////            //format check
////            if (fields.size() > 2 ) {
////                cerr << "Skipping line: too many fields: " << line << endl;
////                continue;
////            } else if (fields.size() == 1) {
////                Node* node = this->add_new_node();
////                string name1 = strip(fields[0],whitespace);
////                cerr << "Found single node " << name1 << endl;
////                node->name = name1;
////                idmap[name1] = node;
////                continue;
////            } else if (fields.size() < 1) {
////                continue;
////            } else { // there are exactly 2 nodes
////
////                string name1 = strip(fields[0],whitespace);
////                string name2 = strip(fields[1],whitespace);
////
////                //cerr << line << endl;
////                //if(idmap.count(name1)) cerr << name1 << " " << idmap[name1] << endl ;
////                //if(idmap.count(name2)) cerr << name2 << " " << idmap[name2] << endl ;
////                //cerr << "---" << endl;
////
////                //new node;
////                if(idmap.count(name1)==0) {
////                    //allocate memory for new node
////                    Node* node = this->add_new_node();
////                    node->name = name1;
////                    idmap[name1] = node;
////                }
////
////                //new node;
////                if(idmap.count(name2)==0) {
////                    //allocate memory for new node
////                    Node* node = this->add_new_node();
////                    node->name = name2;
////                    idmap[name2]=node;
////                }
////
////                idmap[name1]->connect_to(idmap[name2]);
////                //Node *n1 = idmap[name1];
////                //Node *n2 = idmap[name2];
////                //n1->connect_to(n2);
////            }
////        }
////    }
////
//}


}  // namespace elps


