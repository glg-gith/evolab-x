/*
 * LabSiteSetsCalculator.cpp
 *
 *  Created on: Feb 19, 2013
 *      Author: golgauth
 */

#include "LabSiteSetsCalculator.h"

namespace elps {

LabSiteSetsCalculator::LabSiteSetsCalculator() {
	setsManager = SiteSetsMgr::GetInstance();
	rng = new LabRng();
}

LabSiteSetsCalculator::~LabSiteSetsCalculator() {
	// TODO Auto-generated destructor stub
}



LabSet& LabSiteSetsCalculator::GS(string a_set_name) {
	//return setsManager->GS(a_set_name);//->GetSet();
	return setsManager->GetSet(a_set_name);//->GetSet();
}

LabSiteBase * LabSiteSetsCalculator::GI(int id) {

	if (setsManager->GetSitesOpened()) {
		LabConsts::E(LabConsts::SET_SITES_NOT_ENDED);
		return 0;
	}

	return setsManager->GetAt(id);
}

int LabSiteSetsCalculator::GetSitesCount() {
	return setsManager->GetSitesCount(); //sites.size();
}


int LabSiteSetsCalculator::CNT(LabSet a_set) {
	return a_set.Cardinality();
}

double LabSiteSetsCalculator::AVG(LabSet a_set, LabSiteBase::t_deptype dep_type, int attr_id) {

	// TODO : Here should be tested if "attr_id" is an "indexed" attribute !!!

	double avg = 0.0;

	LabSet::iter it(a_set);
	for (int i = it.next(); i != it.end(); i = it.next()) {
		avg += GI(i)->GetAttrDouble(dep_type, attr_id);
	}
	return avg / (double)a_set.Cardinality();

}

vector<double> LabSiteSetsCalculator::AVG(LabSet a_set, vector<int>& dep_types, vector<int>& attr_ids) {
	//double *avgs = (double *)malloc(size * sizeof(double));
	vector<double> avgs(dep_types.size(), 0.0);

	if (dep_types.size() != attr_ids.size()) LabConsts::E(LabConsts::BAD_DEPTYPE_ATTR_SIZES);

	LabSet::iter it(a_set);
	for (int i = it.next(); i != it.end(); i = it.next()) {
		for (int j = 0; j < dep_types.size(); j++)
		{
			avgs[j] += GI(i)->GetAttrDouble(dep_types[j], attr_ids[j]);
		}
	}
	for (int k = 0; k < dep_types.size(); k++)
	{
		avgs[k] /= (double)a_set.Cardinality();
	}
	return avgs;
}

double LabSiteSetsCalculator::LAVG(LabSet a_set, LabSiteBase::t_deptype dep_type, int attr_id) {

	double avg = 0.0;
	int i = a_set.GetFirst();
	if (i != LabSet::iter::END) avg = GI(i)->GetAttrDouble(dep_type, attr_id);

	return avg;
}

double LabSiteSetsCalculator::RAVG(LabSet a_set, LabSiteBase::t_deptype dep_type, int attr_id) {

	double avg = 0.0;
	int i = a_set.ChooseOne(rng);
	if (i != LabSet::iter::END) avg = GI(i)->GetAttrDouble(dep_type, attr_id);

	return avg;
}


// Neighbors stuffs //


int LabSiteSetsCalculator::CNT_NGH(LabSiteBase *ind, int state) {
	vector<LabNode *> nghs = ind->parent->GetNeighbors();
	int cnt = 0;
	for (unsigned int i = 0; i < nghs.size() ; i++) {
		if ( ((LabSiteBase *)(nghs[i]->GetUserData()))->GetState() == state ) {
			cnt++;
		}
	}
	return cnt;
}

int LabSiteSetsCalculator::CNT_NGH(LabSiteBase *ind, LabSiteBase::t_deptype dep_type, int indexed_attr_id, unsigned int attr_value) {
	vector<LabNode *> nghs = ind->parent->GetNeighbors();
	int cnt = 0;
	for (unsigned int i = 0; i < nghs.size() ; i++) {
		if ( ((LabSiteBase *)(nghs[i]->GetUserData()))->GetAttrDouble(dep_type, indexed_attr_id) == attr_value ) {
			cnt++;
		}
	}
	return cnt;
}

double LabSiteSetsCalculator::AVG_NGH(LabSiteBase *ind, LabSiteBase::t_deptype dep_type, int attr_id) {
	vector<LabNode *> nghs = ind->parent->GetNeighbors();
	double sum = 0.0;
	for (unsigned int i = 0; i < nghs.size() ; i++) {
		sum += ((LabSiteBase *)(nghs[i]->GetUserData()))->GetAttrDouble(dep_type, attr_id);
	}
	return sum / (double)nghs.size();
}

//vector<LabNode *> LabSiteSetsCalculator::GET_NGH_1(LabSiteBase *ind, int state, int ngh_depth) {
//	vector<LabNode *> nghs, nghs2;
//
//	for (unsigned int i = 0; i < ind->parent->GetNeighbors().size() ; i++) {
//		if ( ((LabSiteBase *)(ind->parent->GetNeighbors()[i]->GetUserData()))->GetState() == state ) {
//			nghs.push_back(ind->parent->GetNeighbors()[i]);
//		}
//	}
//	//nghs = ind->parent->GetNeighbors();
//	if (ngh_depth > 1)
//	{
//		while (ngh_depth > 1) {
//			nghs2.clear();
//			for (unsigned int i = 0; i < nghs.size() ; i++) {
//				for (unsigned int j = 0; j < nghs[i]->GetNeighbors().size() ; j++) {
//					if ( ((LabSiteBase *)(nghs[i]->GetNeighbors()[j]->GetUserData()))->GetState() == state ) {
//						nghs2.push_back(nghs[i]->GetNeighbors()[j]);
//					}
//				}
//			}
//			nghs.clear();
//			nghs = nghs2;
//			ngh_depth--;
//		}
//	}
//
//	return nghs;
//}
//
//
//int LabSiteSetsCalculator::CNT_NGH(LabSiteBase *ind, int state, int ngh_depth, bool leaf_only) {
//	unsigned int i, cnt = 0;
//
//
//	vector<LabNode *> nodes = GET_NGH_1(ind, state, ngh_depth);
//	for (i = 0; i < nodes.size() ; i++) {
//		if ( ((LabSiteBase *)(nodes[i]->GetUserData()))->GetState() == state ) {
//			cnt ++;
//		}
//	}
//
//	return cnt;
//}


} /* namespace elps */

