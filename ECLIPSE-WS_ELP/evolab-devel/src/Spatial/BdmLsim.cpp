/*
 * BdmLsim.cpp
 *
 *  Created on: 13 sept. 2012
 *      Author: golgauth
 */


/**********************************************/
/* BIRTH-DEATH-MIGRATION MODEL                */
/* Sebastien Lion                             */
/**********************************************/

// TODO : Try an implementation of this simulation with MOVEMENT  !!!!!!!!!!!!!!!!!!!!

#include "BdmLsim.h"



using namespace std;


//**  GLOBALS  **//			Lazy way : should be class attributes
string run_id	= "0";
int nb_sites 	= 100;//100000
int con_deg 	= 4;			// Number of connected neighbours per node
int nb_cols 	= 10;//100			// Number of columns for display

double p_init 	= 0.5;//0.01
int tt_out 		= 1;   			// output par pas de 1
int tt_out_st	= 5;
int tt_end 		= 50;  			// when simulation ends

int resynch		= 500; 			// recalculate totalrate periodically to avoid numerical drift

//double tt;
double totalrate;
unsigned int popcnt;           // total pop size



// Timer
struct timeval tvBegin, tvEnd, tvDiff;

// Inputs
double d, b, mu, x1, x2, L;

// RNG
gsl_rng *rng;


BdmLsim::BdmLsim() {
	// TODO Auto-generated constructor stub
	outputer = NULL;
	outputer2 = NULL;
	outputer3 = NULL;
}

BdmLsim::~BdmLsim() {
	// TODO Auto-generated destructor stub
}


// RATES CALLBACKS :

double NoneRate(EventCallBackParam node) { return 0.0; }

double BirthRate(EventCallBackParam node)
{
	Node *a_node = (Node *)node;
	double rate;
	BdmLsim::t_site a_site = *BdmLsim::GetSiteData(a_node);
	rate = b * a_site.x;
	if(a_site.stateStack->GetCurrentState() == BdmLsim::MUTANT)
		rate *= 0.9;
	return rate;
}

double DeathRate(EventCallBackParam node)
{
	Node *a_node = (Node *)node;
	BdmLsim::t_site a_site = *BdmLsim::GetSiteData(a_node);
	return d * a_site.x;
}

double MovementRate(EventCallBackParam node)
{
	Node *a_node = (Node *)node;
	double rate;
	rate = BdmLsim::GetSiteData(a_node)->m;
	rate *= 1.0 - (double)BdmLsim::NbOccupiedNghs(a_node) / (double)con_deg;
	return rate;
}

double MutationRate(EventCallBackParam node)
{
	//Node *a_node = (Node *)node;
	return mu;
}


// REALIZES CALLBACKS :

double RealizeNone(EventCallBackParam node) { cout << "NONE fired !" << endl; }

double RealizeBirth(EventCallBackParam node)
{
	Node *a_node = (Node *)node;
	cout << "BIRTH fired !" << endl;

	int birth_neighbour, birth_site;

	Node *n_birthsite;
	BdmLsim::t_site *s_data, *bs_data;

	// Reproduce globally with probability L
	if(gsl_rng_uniform(rng) < L)
	{
		do
		{
			birth_site = gsl_rng_uniform_int(rng, nb_sites);
			n_birthsite = a_node->get_network()->get_node(birth_site);
		}
		while (n_birthsite == a_node);
	}
	else
		// Locally with probability 1-L
	{
		birth_neighbour = gsl_rng_uniform_int(rng, con_deg);
		n_birthsite = a_node->get_neighbors()[birth_neighbour];
	}

	// Reproduction only if birth_site is empty
	s_data = BdmLsim::GetSiteData(a_node);
	bs_data = BdmLsim::GetSiteData(n_birthsite);
	if (bs_data->stateStack->GetCurrentState() == BdmLsim::EMPTY)
	{
		// Sites update
		bs_data->stateStack->SetCurrentState(s_data->stateStack->GetCurrentState(), s_data->eventStack->GetTime());
		bs_data->x = s_data->x;
		bs_data->m = s_data->m;
		//		graph[birth_site].occupied=OCCUPIED;
		//		graph[birth_site].state=graph[site].state;
		//		graph[birth_site].x=graph[site].x;
		//		graph[birth_site].m=graph[site].m;

		vector<Node*>::iterator it;
		vector<Node*> nghs;

		// Rates update

//		BdmLsim::UpdateRates(a_node);
//		//		update_rates(site);
//		nghs = a_node->get_neighbors();
//		for (it = nghs.begin(); it != nghs.end(); it++)
//		{
//			BdmLsim::UpdateRates(*it);
//		}
//		//		for(i=0; i<neighbours; i++)
//		//			update_rates(graph[site].neigh[i]);

		BdmLsim::UpdateRatesLocal(a_node);

//		BdmLsim::UpdateRates(n_birthsite);
//		//		update_rates(birth_site);
//		nghs = n_birthsite->get_neighbors();
//		for (it = nghs.begin(); it != nghs.end(); it++)
//		{
//			BdmLsim::UpdateRates(*it);
//		}
//		//		for(i=0; i<neighbours; i++)
//		//			update_rates(graph[birth_site].neigh[i]);

		BdmLsim::UpdateRatesLocal(n_birthsite);

		// Population size update
		popcnt++;
	}

}

double RealizeDeath(EventCallBackParam node)
{
	Node *a_node = (Node *)node;
	cout << "DEATH fired !" << endl;

	// Switch to EMPTY state
	BdmLsim::t_site *s_data;
	s_data = BdmLsim::GetSiteData(a_node);
	s_data->stateStack->SetCurrentState(BdmLsim::EMPTY, s_data->eventStack->GetTime());
	s_data->x = 0.0;
	s_data->m = 0.0;

	// Rates update
	BdmLsim::UpdateRatesLocal(a_node);

	popcnt--;

}

double RealizeMovement(EventCallBackParam node)
{
	Node *a_node = (Node *)node;
	cout << "MOVEMENT fired !" << endl;

	// TODO : The following should work : but is UNTESTED !!!
	// Ask Seb about this...
	int move_site;
	Node *n_movesite;
	BdmLsim::t_site *s_data, *ms_data;

	// Choose a site to move to
	move_site = gsl_rng_uniform_int(rng, con_deg);
	n_movesite = a_node->get_neighbors()[move_site];

	// Try moving
	s_data = BdmLsim::GetSiteData(a_node);
	ms_data = BdmLsim::GetSiteData(n_movesite);
	if (ms_data->stateStack->GetCurrentState() == BdmLsim::EMPTY)
	{
		// Copy old location properties to the new one :
		ms_data->eventStack->Copy(s_data->eventStack);
		ms_data->stateStack->Copy(s_data->stateStack);
		ms_data->x = s_data->x;
		ms_data->m = s_data->m;

		// Switch old location to EMPTY state :
		s_data->stateStack->SetCurrentState(BdmLsim::EMPTY, s_data->eventStack->GetTime());
		s_data->x = 0.0;
		s_data->m = 0.0;

		// Rates update
		BdmLsim::UpdateRatesLocal(a_node);
	}

}

double RealizeMutation(EventCallBackParam node)
{
	Node *a_node = (Node *)node;
	cout << "MUTATION fired !" << endl;

	// Mutate
	BdmLsim::t_site *s_data;
	s_data = BdmLsim::GetSiteData(a_node);
	if(s_data->stateStack->GetCurrentState() == BdmLsim::RESIDENT)
	{
		s_data->stateStack->SetCurrentState(BdmLsim::MUTANT, s_data->eventStack->GetTime());
		s_data->x = x2;
	}
	else
	{
		s_data->stateStack->SetCurrentState(BdmLsim::RESIDENT, s_data->eventStack->GetTime());
		s_data->x = x1;
	}

	// Rates update
	BdmLsim::UpdateRatesLocal(a_node);

}



/*
 * BdmLsim CLASS dedicated stuffs
 */


void BdmLsim::InitInputs()
{
	d 	= 0.01;		// Death rate
	b 	= 0.01;		// Birth rate
	mu 	= 0.01;		// Mutation rate
	x1 	= 0.01;		// Trait coeff for Resident
	x2 	= 0.02;		// Trait coeff for Mutant
	L 	= 0.01;		// Reproduction rate
}

void BdmLsim::InitPopulation()
{
	t_site *a_site;
	vector<Node*> nodes = this->GetNetwork()->get_nodes();
	vector<Node*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		a_site = (t_site *)malloc(sizeof(t_site));
		// STATES
		a_site->stateStack = new LabStateStack();
		a_site->stateStack->PushState("EMPTY", 1, -1, -1);
		a_site->stateStack->PushState("RESIDANT", 1, -1, -1);
		a_site->stateStack->PushState("MUTANT", 1, -1, -1);

		if(gsl_rng_uniform(rng) < p_init)
		{
			a_site->stateStack->SetCurrentState(BdmLsim::RESIDENT, GetTime());
			a_site->x = x1;
			a_site->m = 0;		// TODO : Try an implementation of this simulation with MOVEMENT !

			popcnt++;
		}
		else
		{
			a_site->stateStack->SetCurrentState(BdmLsim::EMPTY, GetTime());
			a_site->x = 0;
			a_site->m = 0;
		}

		// EVENTS
		a_site->eventStack = new LabEventStack();

		RateCallBack *rate_callback;
		RealizeCallBack *realize_callback;

		//		a_site->eventStack->PushEvent("NONE", 0);
		FNoneRate = &NoneRate;
		FRealizeNone = &RealizeNone;
		rate_callback = new RateCallBack(FNoneRate);
		realize_callback = new RealizeCallBack(FRealizeNone);
		a_site->eventStack->PushEvent("NONE", 0, (EventCallBackParam)(*it), rate_callback, realize_callback);

		//		a_site->eventStack->PushEvent("BIRTH", 0);
		FBirthRate = &BirthRate;
		FRealizeBirth = &RealizeBirth;
		rate_callback = new RateCallBack(FBirthRate);
		realize_callback = new RealizeCallBack(FRealizeBirth);
		a_site->eventStack->PushEvent("BIRTH", 0, (EventCallBackParam)(*it), rate_callback, realize_callback);

		//		a_site->eventStack->PushEvent("DEATH", 0);
		FDeathRate = &DeathRate;
		FRealizeDeath = &RealizeDeath;
		rate_callback = new RateCallBack(FDeathRate);
		realize_callback = new RealizeCallBack(FRealizeDeath);
		a_site->eventStack->PushEvent("DEATH", 0, (EventCallBackParam)(*it), rate_callback, realize_callback);

		//		a_site->eventStack->PushEvent("MOVEMENT", 0);
		FMovementRate = &MovementRate;
		FRealizeMovement = &RealizeMovement;
		rate_callback = new RateCallBack(FMovementRate);
		realize_callback = new RealizeCallBack(FRealizeMovement);
		a_site->eventStack->PushEvent("MOVEMENT", 0, (EventCallBackParam)(*it), rate_callback, realize_callback);

		//		a_site->eventStack->PushEvent("MUTATION", 0);
		FMutationRate = &MutationRate;
		FRealizeMutation = &RealizeMutation;
		rate_callback = new RateCallBack(FMutationRate);
		realize_callback = new RealizeCallBack(FRealizeMutation);
		a_site->eventStack->PushEvent("MUTATION", 0, (EventCallBackParam)(*it), rate_callback, realize_callback);


		(*it)->SetUserData(a_site);
	}

	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		UpdateRates(*it);
	}
	ResynchTotalRate();
}


BdmLsim::t_site *BdmLsim::GetSiteData(Node *a_node)
{
	return (t_site *)(a_node->GetUserData());
}

int BdmLsim::NbOccupiedNghs(Node *a_node)
{
	int nb = 0;
	bool occupied;
	vector<Node*> nghs = a_node->get_neighbors();
	for (vector<Node*>::iterator it = nghs.begin(); it != nghs.end(); it++)
	{
		occupied = ( BdmLsim::GetSiteData(*it)->stateStack->GetCurrentState() != BdmLsim::EMPTY );
		if (occupied) nb++;
	}
	//if (nb) cout << nb << endl;
	return nb;
}

void BdmLsim::UpdateRatesLocal(Node *a_node)
{
	vector<Node*>::iterator it;
	vector<Node*> nghs;

	BdmLsim::UpdateRates(a_node);
	nghs = a_node->get_neighbors();
	for (it = nghs.begin(); it != nghs.end(); it++)
	{
		BdmLsim::UpdateRates(*it);
	}
}

void BdmLsim::UpdateRates(Node *a_node)
{
	t_site *a_site = GetSiteData(a_node);

	totalrate -= a_site->eventStack->GetStackRate();

	if(a_site->stateStack->GetCurrentState() != BdmLsim::EMPTY)
	{
		/*a_site->eventStack->SetEventRate(BdmLsim::BIRTH, 		BirthRate	(a_node));
		a_site->eventStack->SetEventRate(BdmLsim::DEATH, 		DeathRate	(a_node));
		a_site->eventStack->SetEventRate(BdmLsim::MOVEMENT, 	MovementRate(a_node));
		a_site->eventStack->SetEventRate(BdmLsim::MUTATION, 	MutationRate(a_node));*/
		a_site->eventStack->UpdateStackRates();
		//cout << (a_site->eventStack->GetEventRate(0) + a_site->eventStack->GetEventRate(0) + a_site->eventStack->GetEventRate(1) + a_site->eventStack->GetEventRate(2) + a_site->eventStack->GetEventRate(3) + a_site->eventStack->GetEventRate(4));
		//cout << " / " << a_site->eventStack->GetStackRate() << endl;
	}
	else
		a_site->eventStack->ResetStackRate();

	totalrate += a_site->eventStack->GetStackRate();
}

void BdmLsim::ResynchTotalRate()
{
	totalrate=0.0;
	vector<Node*> nodes = this->GetNetwork()->get_nodes();
	vector<Node*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		totalrate += BdmLsim::GetSiteData(*it)->eventStack->GetStackRate();
	}
}

unsigned int BdmLsim::Census()
{
	unsigned int nx = 0, nox = 0, nxx = 0, noox = 0, nxox = 0;


	vector<Node*> nodes = this->GetNetwork()->get_nodes();
	vector<Node*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		if (GetSiteData(*it)->stateStack->GetCurrentState() != BdmLsim::EMPTY)
		{
			nx++;

			vector<Node*> nghs = (*it)->get_neighbors();
			for (vector<Node*>::iterator it2 = nghs.begin(); it2 != nghs.end(); it2++)
			{
				if (GetSiteData(*it2)->stateStack->GetCurrentState() != BdmLsim::EMPTY)
				{
					nox++;
					vector<Node*> nghs = (*it2)->get_neighbors();
					for (vector<Node*>::iterator it3 = nghs.begin(); it3 != nghs.end(); it3++)
					{
						if(GetSiteData(*it3)->stateStack->GetCurrentState() == BdmLsim::EMPTY)
						{
							noox++;
						}
						else
						{
							if ((*it3) != (*it))
								nxox++;
						}
					}
				}
				else
				{
					nxx++;
				}
			}
		}
	}

	// Write a data line
	vector<double> line;
	double tt = GetTime();

	line.push_back(tt);
	line.push_back(nx);
	line.push_back(nox);
	line.push_back(nxx);
	line.push_back(nxox);
	line.push_back(noox);
	outputer->AddDataLine(line);
	if (tt == 0) outputer2->InsertText("t nx nox nxx nxox noox\n");
	outputer2->AddDataLine(line);

	if ( ((int)tt) % tt_out_st == 0 )
	{
		line.clear();
		line.push_back(tt); outputer2->AddDataLine(line); line.clear();

		int i = 0;
		for (it = nodes.begin(); it != nodes.end(); it++)
		{
			line.push_back(GetSiteData(*it)->stateStack->GetCurrentState());
			i++;
			if (i % nb_cols == 0)
			{
				outputer2->AddDataLine(line);
				line.clear();
			}
		}
	}

	outputer->WriteData();
	outputer->ResetDataSet();

	outputer2->InsertText("\n");
	outputer2->WriteData();
	outputer2->ResetDataSet();

	flush(cout);

	return (nx);
}


void BdmLsim::RealizeEvent()
{
	double add;
	double p1;
	t_site a_site;

	vector<Node*> nodes = this->GetNetwork()->get_nodes();
	vector<Node*>::iterator it;

	// Choose a site
	add = 0.0;
	p1 = gsl_rng_uniform(rng) * totalrate;
	it = nodes.begin();
	for (;;)
	{
		add += BdmLsim::GetSiteData(*it)->eventStack->GetStackRate();
		if (p1 < add) break;
		it++;
	}
	a_site = *BdmLsim::GetSiteData(*it);

	// Choose an event and realize it
	//	add = 0.0;
	//	p1 = gsl_rng_uniform(rng) * a_site.eventStack->GetStackRate();
	//
	//	add += a_site.eventStack->GetEventRate(BdmLsim::BIRTH);
	//	if(p1 < add)
	//		RealizeBirth(*it);
	//	else
	//	{
	//		add += a_site.eventStack->GetEventRate(BdmLsim::DEATH);
	//		if(p1 < add)
	//			RealizeDeath(*it);
	//		else
	//		{
	//			add += a_site.eventStack->GetEventRate(BdmLsim::MOVEMENT);
	//			if(p1 < add)
	//				RealizeMovement(*it);
	//			else
	//				RealizeMutation(*it);
	//		}
	//	}
	p1 = gsl_rng_uniform(rng);
	a_site.eventStack->RealizeEvent(p1, GetTime());

}


void BdmLsim::DumpLattice()
{
	unsigned int i = 0;

	vector<double> a_line;

	vector<Node*> nodes = this->GetNetwork()->get_nodes();
	vector<Node*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		a_line.clear();
		a_line.push_back(i);
		a_line.push_back(GetSiteData(*it)->stateStack->GetCurrentState());
		a_line.push_back(GetSiteData(*it)->x);
		outputer3->AddDataLine(a_line);
		i++;
	}

	outputer3->WriteData();
	outputer3->ResetDataSet();
}



/*
 * Virtual stuffs implementation
 */

void BdmLsim::InitSimulation()
{
	//tt = 0.0;
	totalrate = 0.0;

	rng = LabRngUtils::CreateRandomizer();
	InitInputs();

	outputFilename = "bdmLsim.res." + run_id;
	outputFilename2 = "spat-" + outputFilename;
	stringstream ss;
	ss << "dump.state-L" << L << "-" << outputFilename;
	outputFilename3 = ss.str();

	InitPopulation();

	if (outputer == NULL) {
		outputer = new LabOutputerLine();
		outputer->SetOuputMode(LabOutputerLine::TO_BOTH);
		outputer->SetSeparator("\t");
		outputer->SetFilename(outputFilename);
	}
	if (outputer2 == NULL) {
		outputer2 = new LabOutputerLine();
		outputer2->SetOuputMode(LabOutputerLine::TO_FILE);
		outputer2->SetSeparator("\t");
		outputer2->SetFilename(outputFilename2);
	}
	if (outputer3 == NULL) {
		outputer3 = new LabOutputerLine();
		outputer3->SetOuputMode(LabOutputerLine::TO_FILE);
		outputer3->SetSeparator("\t");
		outputer3->SetFilename(outputFilename3);
	}

	UpdateEventsRate(totalrate);
	SetEndTime(tt_end);
	SetOutputTimeStep(tt_out);

}


void BdmLsim::BeforeStep() {
	UpdateEventsRate(totalrate);
	//tt = GetTime();
}

void BdmLsim::AfterStep() {
	//tt = GetTime();
}


void BdmLsim::FireEvent() {



	// TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// Choose and realize an event
	RealizeEvent();

	// Perdiodically recalculate totalrate
	if (GetNbFiredEvents() % resynch == 0)
		ResynchTotalRate();

	UpdateEventsRate(totalrate);

}

// Use preferably the 'LabOutputer' utility in this hook
void BdmLsim::OutputSimulation()
{
	// Census pop
	Census();

	// Periodically dump lattice state
	DumpLattice();
}

void BdmLsim::BeforeRun()
{
	// Get begin time
	char *str = (char *) malloc (80 * sizeof(char));
	str[0] = '\0';
	gettimeofday(&tvBegin, NULL);
	LabTimeUtils::TimevalPrint(str, &tvBegin);
	cout << str;
}

void BdmLsim::AfterRun()
{
	// Get end time
	gettimeofday(&tvEnd, NULL);
	char *str = (char *) malloc (80 * sizeof(char));
	str[0] = '\0';
	LabTimeUtils::TimevalPrint(str, &tvEnd);
	cout << str;

	// Write diff time
	LabTimeUtils::TimevalSubtract(&tvDiff, &tvEnd, &tvBegin);
	cout << "TOTAL SIMULATION TIME : ";
	cout << (tvDiff.tv_sec / 60) << " min. " << (tvDiff.tv_sec % 60) << " sec. " << (tvDiff.tv_usec) << " usec." << endl;

	// Save the generated network
	SaveNetwork("bdmLsim.res.lat." + run_id);
}


int main(int argc, char **argv) {

	BdmLsim *sim = new BdmLsim();

	sim->SetNbRepetitions(1);

	// Initialize network
	LabNetwork *net = new LabNetwork();
	net->populate(nb_sites);
	//net->square_lattice()
	if (( (int) con_deg * net->size()) % 2 == 1) {
		cout << ("The sum of all degrees must be even\nThis is not possible with the network parameters you have specified");
		return false;
	}
	vector<double> dist(con_deg+1, 0);
	dist[con_deg] = 1;
	net->rand_connect_user(dist);

	sim->SetNetwork(net);

	sim->Run();
}




