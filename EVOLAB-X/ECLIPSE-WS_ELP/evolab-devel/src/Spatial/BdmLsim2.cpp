/*
 * BdmLsim222.cpp
 *
 *  Created on: 18 sept. 2012
 *      Author: golgauth
 */


/**********************************************/
/* BIRTH-DEATH-MIGRATION MODEL                */
/* Sebastien Lion                             */
/**********************************************/

// TODO : Try an implementation of this simulation with MOVEMENT  !!!!!!!!!!!!!!!!!!!!

#include "BdmLsim2.h"

#include <gsl/gsl_nan.h>

using namespace std;


//**  GLOBALS  **//			Lazy way : should be class attributes
string run_id	= "0";
int nb_rep		= 1;
int nb_sites 	= 100000;//100000;//100;//
int con_deg 	= 4;			// Number of connected neighbours per node
int nb_cols 	= 100;//100;//10;//			// Number of columns for display

double p_init 	= 0.5;//0.5;//0.01
int tt_out 		= 1;   			// output par pas de 1
int tt_out_st	= 5;
int tt_end 		= 50;  			// when simulation ends

int resynch		= 500; 			// recalculate totalrate periodically to avoid numerical drift

//double tt;
double totalrate;
unsigned int popcnt;           // total pop size

string output_files_prefix;
string net_dump_filename;		// where to dump the network connections as text edges list
string xml_input_filename;		// XML file to be parsed


// Timer
struct timeval tvBegin, tvEnd, tvDiff;

// Inputs
// ------
// values:
double d, b, mu, mo, x1, x2, L;
// increasing limit values:
double d2, b2, mu2, mo2, x12, x22, L2;
// step values:
double d_s, b_s, mu_s, mo_s, x1_s, x2_s, L_s;

// RNG
gsl_rng *rng;


BdmLsim2::BdmLsim2() {
	// TODO Auto-generated constructor stub
	outputer = NULL;
	outputer2 = NULL;
	outputer3 = NULL;
	output_files_prefix = "output";
}

BdmLsim2::~BdmLsim2() {
	// TODO Auto-generated destructor stub
}


// RATES CALLBACKS :

double NoneRate(EventCallBackParam node) { return 0.0; }

double BirthRate(EventCallBackParam node)
{
	Node *a_node = (Node *)node;
	double rate;
	BdmLsim2::t_site *a_site = BdmLsim2::GetSiteData(a_node);
	rate = b * a_site->GetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim2::X);
	if(a_site->GetStateStack()->GetCurrentState() == BdmLsim2::MUTANT)
		rate *= 0.9;
	return rate;
}

double DeathRate(EventCallBackParam node)
{
	Node *a_node = (Node *)node;
	BdmLsim2::t_site *a_site = BdmLsim2::GetSiteData(a_node);
	return d * a_site->GetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim2::X);
}

double MovementRate(EventCallBackParam node)
{
	Node *a_node = (Node *)node;
	double rate;
	rate = BdmLsim2::GetSiteData(a_node)->GetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim2::M);
	rate *= 1.0 - (double)BdmLsim2::NbOccupiedNghs(a_node) / (double)con_deg;
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
	//cout << "BIRTH fired !" << endl;

	int birth_neighbour, birth_site;

	Node *n_birthsite;
	BdmLsim2::t_site *s_data, *bs_data;

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
	s_data = BdmLsim2::GetSiteData(a_node);
	bs_data = BdmLsim2::GetSiteData(n_birthsite);
	if (bs_data->GetStateStack()->GetCurrentState() == BdmLsim2::EMPTY)
	{
		// Sites update
		bs_data->GetStateStack()->SetCurrentState(s_data->GetStateStack()->GetCurrentState(), s_data->GetEventStack()->GetTime());
		////bs_data->x = s_data->x;
		////bs_data->m = s_data->m;
		bs_data->GetAttributesStack()->Copy(s_data->GetAttributesStack(), LabAttributesStack::IND_DEP);
		//		graph[birth_site].occupied=OCCUPIED;
		//		graph[birth_site].state=graph[site].state;
		//		graph[birth_site].x=graph[site].x;
		//		graph[birth_site].m=graph[site].m;

		vector<Node*>::iterator it;
		vector<Node*> nghs;

		// Rates update

		//		BdmLsim2::UpdateRates(a_node);
		//		//		update_rates(site);
		//		nghs = a_node->get_neighbors();
		//		for (it = nghs.begin(); it != nghs.end(); it++)
		//		{
		//			BdmLsim2::UpdateRates(*it);
		//		}
		//		//		for(i=0; i<neighbours; i++)
		//		//			update_rates(graph[site].neigh[i]);

		BdmLsim2::UpdateRatesLocal(a_node);

		//		BdmLsim2::UpdateRates(n_birthsite);
		//		//		update_rates(birth_site);
		//		nghs = n_birthsite->get_neighbors();
		//		for (it = nghs.begin(); it != nghs.end(); it++)
		//		{
		//			BdmLsim2::UpdateRates(*it);
		//		}
		//		//		for(i=0; i<neighbours; i++)
		//		//			update_rates(graph[birth_site].neigh[i]);

		BdmLsim2::UpdateRatesLocal(n_birthsite);

		// Population size update
		popcnt++;
	}

}

double RealizeDeath(EventCallBackParam node)
{
	Node *a_node = (Node *)node;
	//cout << "DEATH fired !" << endl;

	// Switch to EMPTY state
	BdmLsim2::t_site *s_data;
	s_data = BdmLsim2::GetSiteData(a_node);
	s_data->GetStateStack()->SetCurrentState(BdmLsim2::EMPTY, s_data->GetEventStack()->GetTime());
	s_data->SetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim2::X, 0.0);
	s_data->SetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim2::M, 0.0);
	//	BdmLsim2::SetAttrValue(s_data, BdmLsim2::X, 0.0);
	//	BdmLsim2::SetAttrValue(s_data, BdmLsim2::M, 0.0);
	//	s_data->x = 0.0;
	//	s_data->m = 0.0;

	// Rates update
	BdmLsim2::UpdateRatesLocal(a_node);

	popcnt--;

}

double RealizeMovement(EventCallBackParam node)
{
	Node *a_node = (Node *)node;
	//cout << "MOVEMENT fired !" << endl;

	// TODO : The following should work : but is UNTESTED !!!
	// Ask Seb about this...
	//	int move_site;
	//	Node *n_movesite;
	//	BdmLsim2::t_site *s_data, *ms_data;
	//
	//	// Choose a site to move to
	//	move_site = gsl_rng_uniform_int(rng, con_deg);
	//	n_movesite = a_node->get_neighbors()[move_site];
	//
	//	// Try moving
	//	s_data = BdmLsim2::GetSiteData(a_node);
	//	ms_data = BdmLsim2::GetSiteData(n_movesite);
	//	if (ms_data->GetStateStack()->GetCurrentState() == BdmLsim2::EMPTY)
	//	{
	//		// Copy old location properties to the new one :
	//		ms_data->Copy(s_data, LabAttributesStack::IND_DEP);
	//		//		ms_data->GetEventStack()->Copy(s_data->GetEventStack());
	//		//		ms_data->GetStateStack()->Copy(s_data->GetStateStack());
	//		//		ms_data->GetAttributesStack()->Copy(s_data->GetAttributesStack(), LabAttributesStack::IND_DEP);
	//		//		ms_data->x = s_data->x;
	//		//		ms_data->m = s_data->m;
	//
	//		// Switch old location to EMPTY state :
	//		s_data->GetStateStack()->SetCurrentState(BdmLsim2::EMPTY, s_data->GetEventStack()->GetTime());
	//		s_data->SetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim2::X, 0.0);
	//		s_data->SetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim2::M, 0.0);
	//		//		BdmLsim2::SetAttrValue(s_data, BdmLsim2::X, 0.0);
	//		//		BdmLsim2::SetAttrValue(s_data, BdmLsim2::M, 0.0);
	//		//	s_data->x = 0.0;
	//		//	s_data->m = 0.0;
	//
	//		// Rates update
	//		BdmLsim2::UpdateRatesLocal(a_node);
	//	}

}

double RealizeMutation(EventCallBackParam node)
{
	Node *a_node = (Node *)node;
	//cout << "MUTATION fired !" << endl;

	// Mutate
	BdmLsim2::t_site *s_data;
	s_data = BdmLsim2::GetSiteData(a_node);
	if(s_data->GetState() == BdmLsim2::RESIDENT)
	{
		s_data->SetState(BdmLsim2::MUTANT, s_data->GetEventStack()->GetTime());
		s_data->SetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim2::X, x2);
		//s_data->x = x2;
	}
	else
	{
		s_data->SetState(BdmLsim2::RESIDENT, s_data->GetEventStack()->GetTime());
		s_data->SetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim2::X, x1);
		//s_data->x = x1;
	}

	// Rates update
	BdmLsim2::UpdateRatesLocal(a_node);

}



/*
 * BdmLsim2 CLASS dedicated stuffs
 */


void BdmLsim2::DefaultInputs()
{
	// TODO : Add those params to 'DefaultInputs', 'parseCL' and 'parseXML'
	//	int nb_sites 	= 100000;
	//	int con_deg 	= 4;
	//	int nb_cols 	= 100;		// Automate that one (Should not be a parameter)
	//
	//	double p_init 	= 0.01
	//	int tt_out 		= 1;
	//	int tt_out_st	= 5;
	//	int tt_end 		= 50;
	//
	//	int resynch		= 500;


	d 	= 0.01;	 d2 	= GSL_NAN;		// Death rate
	b 	= 0.01;	 b2 	= GSL_NAN;		// Birth rate
	mu 	= 0.01;	 mu2 	= GSL_NAN;		// Mutation rate
	mo 	= 0.01;	 mo2 	= GSL_NAN;		// Movement rate
	x1 	= 0.01;	 x12 	= GSL_NAN;		// Trait coeff for Resident
	x2 	= 0.02;	 x22 	= GSL_NAN;		// Trait coeff for Mutant
	L 	= 0.01;	 L2 	= GSL_NAN;		// Reproduction rate

	net_dump_filename = "bdmLsim.res.lat." + run_id;
	SetNbRepetitions(nb_rep);
}

string BdmLsim2::ParseCL(int argc, char **argv)
{
	// If XML input was set : skip CL parsing
	for (int i=0; i<argc; i++)
		if (argc-1 > i && strcmp(argv[i], "-i") == 0) return argv[i+1];

	// Wrap everything in a try block.  Do this every time,
	// because exceptions will be thrown for problems.
	try {

		// Define the command line object.
		CmdLine cmd("bdmLsim simulation (using EVOLAB-X1 project)", ' ', "0.4");

		// Define a value argument and add it to the command line.

		// TODO : implement inf and sup limits for all the parameters
		MultiArg<double> dr_arg("d","death-rate", "Constant individual death factor.", true, "double");
		//ValueArg<double> dr_arg("d","death-rate", "Constant individual death factor.", true, 0.01, "double");
		cmd.add( dr_arg );

		ValueArg<double> br_arg("b","birth-rate", "Constant individual birth factor.", true, 0.01, "double");
		cmd.add( br_arg );

		ValueArg<double> mu_arg("m","mut-rate", "Constant individual mutation factor.", true, 0.01, "double");
		cmd.add( mu_arg );

		ValueArg<double> mo_arg("M","mov-rate", "Constant individual movement factor.", true, 0.01, "double");
		cmd.add( mo_arg );

		ValueArg<double> x1_arg("x","x1-trait", "Constant value for resident's trait.", true, 0.01, "double");
		cmd.add( x1_arg );

		ValueArg<double> x2_arg("X","x2-trait", "Constant value for mutant's trait.", true, 0.02, "double");
		cmd.add( x2_arg );

		ValueArg<double> Lr_arg("c","child-rate", "Constant individual reproduction factor.", true, 0.01, "double");
		cmd.add( Lr_arg );

		ValueArg<string> ri_arg("r","run-id", "String identifier for this whole simulation.", true, "run-0", "string");
		cmd.add( ri_arg );

		ValueArg<int> nr_arg("n","nb-rep", "Number of repetitions.", true, 1, "int");
		cmd.add( nr_arg );


		ValueArg<double> ds_arg("s","dump-step", "Dump simulation state every 'dump-step' seconds. \n\
     Requires '-D' option to be enabled.", false, 0.01, "double");
		cmd.add( ds_arg );

		ValueArg<string> xi_arg("i","xml-input", "XML model input file.", false, "model.xml", "string");
		cmd.add( xi_arg );

		ValueArg<string> o_arg("o","out-prefix", "Text for prefixing the output filenames.", false, "output", "string");
		cmd.add( o_arg );

		// Flags
		SwitchArg dd_switch("D", "do-dump", "Dump / backup point every 'dump-step' seconds.", false);
		cmd.add( dd_switch );
		SwitchArg r_switch("R", "recovery", "Starts the simulation from the last backup point.", false);
		cmd.add( r_switch );

		// Parse the args.
		cmd.parse( argc, argv );

		vector<double> vec;

		nb_rep = nr_arg.getValue();

		vec = dr_arg.getValue();
		// TODO : do the same for all other parameters
		if (vec.size() == 1) { d = vec[0]; }
		else { d = min(vec[0], vec[1]); d2 = max(vec[0], vec[1]); d_s = (d2-d)/(double)(nb_rep-1); }

		b = br_arg.getValue();
		mu = mu_arg.getValue();
		mo = mo_arg.getValue();
		x1 = x1_arg.getValue();
		x2 = x2_arg.getValue();
		L = Lr_arg.getValue();

		run_id = ri_arg.getValue();

		this->t_dump_step = ds_arg.getValue();
		this->do_dump = dd_switch.getValue();
		this->do_start_from_dump = r_switch.getValue();

		output_files_prefix = o_arg.getValue();
		xml_input_filename = xi_arg.getValue();

		// Set net dump filename
		stringstream ss;
		ss << output_files_prefix << ".lat." << run_id; // << "." << GetCurrentRepetition();
		net_dump_filename = ss.str();
		SetNbRepetitions(nb_rep);


	}
	catch (ArgException &e)  // catch any exceptions
	{
		cerr << "Error: " << e.error() << " for arg " << e.argId() << endl;
	}

	return "";
}

void BdmLsim2::ParseXML(string filename)
{
	xmlDocPtr doc;
	xmlNodePtr root, consts;
	vector<xmlNodePtr> cst_vec;
	ostringstream ss;
	istringstream iss, iss2;
	string str;
	xmlChar *str2;
	bool has_value2;

	// Parse the args.
	doc = xmlParseFile(filename.c_str());
	if (doc == NULL) {
		cerr << "Invalide XML input document !" << endl;
		return exit(-1);
	}
	root = xmlDocGetRootElement(doc);
	if (root == NULL) {
		cerr << "Empty XML document !" << endl;
		xmlFreeDoc(doc);
		return exit(-1);
	}

	// Read constants
	consts = LabXMLUtils::GetChildByName(root, "Constants");
	if (consts == NULL)
	{
		cerr << "Missing 'Constants' node !" << endl;
		exit(-1);
	}

	cst_vec = LabXMLUtils::GetChildrenByName(consts, "Const");

	for (unsigned int i=0; i< cst_vec.size(); i++)
	{
		ss.str("");
		iss.str("");
		iss2.str("");
		has_value2 = false;
		ss << xmlGetProp(cst_vec[i], (const xmlChar *)"name");
		str = ss.str();
		iss.str((char *)xmlGetProp(cst_vec[i], (const xmlChar *)"value"));
		LabXMLUtils::StrToLower(str);

		// Test if range value
		str2 = xmlGetProp(cst_vec[i], (const xmlChar *)"value2");
		if (str2 != NULL)
		{
			iss2.str(""); iss2.str((char *)str2);
			has_value2 = true;
		}

		if 		(str == "death-rate")
		{
			iss >> d;
			// TODO : do this additional stuff for all the other params.
			if (has_value2) { iss2 >> d2; d_s = (d2-d)/(double)(nb_rep-1); }
		}
		else if (str == "birth-rate")
			iss >> b;
		else if (str == "mut-rate")
			iss >> mu;
		else if (str == "mov-rate")
			iss >> mo;
		else if (str == "x1-rate")
			iss >> x1;
		else if (str == "x2-rate")
			iss >> x2;
		else if (str == "l-rate")
			iss >> L;
		else if (str == "run-id")
			run_id = iss.str();
		else if (str == "nb-rep")
			iss >> nb_rep;
		else if (str == "out-prefix")
			output_files_prefix = iss.str();
		else if (str == "dump-step")
			iss >> this->t_dump_step;
		else if (str == "do-dump")
			this->do_dump = (str == "True");
		else if (str == "recovery")
			this->do_start_from_dump = (str == "True");
	}


	// Set net dump filename
	ss.str("");
	ss << output_files_prefix << ".lat." << run_id; // << "." << GetCurrentRepetition();
	net_dump_filename = ss.str();

	SetNbRepetitions(nb_rep);

	xmlFreeDoc(doc);

}

void BdmLsim2::InitPopulation()
{
	popcnt = 0;
	t_site *a_site;
	vector<Node*> nodes = this->GetNetwork()->get_nodes();
	vector<Node*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		//cout << (*it)->get_neighbors().size() << endl;
		a_site = new LabSiteEvent();
		//a_site = (t_site *)malloc(sizeof(t_site));

		// ATTRIBUTES
		a_site->GetAttributesStack()->PushAttribute("x", LabAttributesStack::IND_DEP);
		a_site->GetAttributesStack()->PushAttribute("m", LabAttributesStack::IND_DEP);

		// STATES
		//a_site->CreateStateStack(); // Done in constructor
		//a_site->stateStack = new LabStateStack();
		a_site->GetStateStack()->PushState("EMPTY", 1, -1, -1);
		a_site->GetStateStack()->PushState("RESIDANT", 1, -1, -1);
		a_site->GetStateStack()->PushState("MUTANT", 1, -1, -1);

		if(gsl_rng_uniform(rng) < p_init)
		{
			a_site->GetStateStack()->SetCurrentState(BdmLsim2::RESIDENT, GetTime());
			a_site->SetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim2::X, x1);
			// TODO : Try an implementation of this simulation with MOVEMENT !
			//a_site->SetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim2::M, mo);
			a_site->SetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim2::M, 0.0);
			//			a_site->x = x1;
			//			a_site->m = 0;

			popcnt++;
		}
		else
		{
			a_site->GetStateStack()->SetCurrentState(BdmLsim2::EMPTY, GetTime());
			a_site->SetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim2::X, 0.0);
			a_site->SetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim2::M, 0.0);
			//			a_site->x = 0;
			//			a_site->m = 0;
		}

		// EVENTS
		//a_site->CreateEventStack(); // Done in constructor
		//a_site->eventStack = new LabEventStack();

		RateCallBack *rate_callback;
		RealizeCallBack *realize_callback;

		//		a_site->eventStack->PushEvent("NONE", 0);
		FNoneRate = &NoneRate;
		FRealizeNone = &RealizeNone;
		rate_callback = new RateCallBack(FNoneRate);
		realize_callback = new RealizeCallBack(FRealizeNone);
		a_site->GetEventStack()->PushEvent("NONE", 0, (EventCallBackParam)(*it), rate_callback, realize_callback);

		//		a_site->eventStack->PushEvent("BIRTH", 0);
		FBirthRate = &BirthRate;
		FRealizeBirth = &RealizeBirth;
		rate_callback = new RateCallBack(FBirthRate);
		realize_callback = new RealizeCallBack(FRealizeBirth);
		a_site->GetEventStack()->PushEvent("BIRTH", 0, (EventCallBackParam)(*it), rate_callback, realize_callback);

		//		a_site->eventStack->PushEvent("DEATH", 0);
		FDeathRate = &DeathRate;
		FRealizeDeath = &RealizeDeath;
		rate_callback = new RateCallBack(FDeathRate);
		realize_callback = new RealizeCallBack(FRealizeDeath);
		a_site->GetEventStack()->PushEvent("DEATH", 0, (EventCallBackParam)(*it), rate_callback, realize_callback);

		//		a_site->eventStack->PushEvent("MOVEMENT", 0);
		FMovementRate = &MovementRate;
		FRealizeMovement = &RealizeMovement;
		rate_callback = new RateCallBack(FMovementRate);
		realize_callback = new RealizeCallBack(FRealizeMovement);
		a_site->GetEventStack()->PushEvent("MOVEMENT", 0, (EventCallBackParam)(*it), rate_callback, realize_callback);

		//		a_site->eventStack->PushEvent("MUTATION", 0);
		FMutationRate = &MutationRate;
		FRealizeMutation = &RealizeMutation;
		rate_callback = new RateCallBack(FMutationRate);
		realize_callback = new RealizeCallBack(FRealizeMutation);
		a_site->GetEventStack()->PushEvent("MUTATION", 0, (EventCallBackParam)(*it), rate_callback, realize_callback);


		(*it)->SetUserData(a_site);
	}

	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		UpdateRates(*it);
	}
	ResynchTotalRate();
}


BdmLsim2::t_site *BdmLsim2::GetSiteData(Node *a_node)
{
	return (BdmLsim2::t_site *)(a_node->GetUserData());
}
//
//double BdmLsim2::GetAttrValue(t_site *a_site, int t_site_attr)
//{
//	return a_site->GetAttributesStack()->GetAttribute(LabAttributesStack::IND_DEP, t_site_attr)->d_value;
//}
//
//void BdmLsim2::SetAttrValue(t_site *a_site, int t_site_attr, double value)
//{
//	a_site->GetAttributesStack()->GetAttribute(LabAttributesStack::IND_DEP, t_site_attr)->d_value = value;
//}


int BdmLsim2::NbOccupiedNghs(Node *a_node)
{
	int nb = 0;
	bool occupied;
	vector<Node*> nghs = a_node->get_neighbors();
	for (vector<Node*>::iterator it = nghs.begin(); it != nghs.end(); it++)
	{
		occupied = ( BdmLsim2::GetSiteData(*it)->GetState() != BdmLsim2::EMPTY );
		if (occupied) nb++;
	}
	//if (nb) cout << nb << endl;
	return nb;
}

void BdmLsim2::UpdateRatesLocal(Node *a_node)
{
	vector<Node*>::iterator it;
	vector<Node*> nghs;

	BdmLsim2::UpdateRates(a_node);
	nghs = a_node->get_neighbors();
	for (it = nghs.begin(); it != nghs.end(); it++)
	{
		BdmLsim2::UpdateRates(*it);
	}
}

void BdmLsim2::UpdateRates(Node *a_node)
{
	t_site *a_site = GetSiteData(a_node);

	totalrate -= a_site->GetEventStack()->GetStackRate();

	if(a_site->GetStateStack()->GetCurrentState() != BdmLsim2::EMPTY)
	{
		/*a_site->eventStack->SetEventRate(BdmLsim2::BIRTH, 		BirthRate	(a_node));
		a_site->eventStack->SetEventRate(BdmLsim2::DEATH, 		DeathRate	(a_node));
		a_site->eventStack->SetEventRate(BdmLsim2::MOVEMENT, 	MovementRate(a_node));
		a_site->eventStack->SetEventRate(BdmLsim2::MUTATION, 	MutationRate(a_node));*/
		a_site->GetEventStack()->UpdateStackRates();
		//cout << (a_site->eventStack->GetEventRate(0) + a_site->eventStack->GetEventRate(0) + a_site->eventStack->GetEventRate(1) + a_site->eventStack->GetEventRate(2) + a_site->eventStack->GetEventRate(3) + a_site->eventStack->GetEventRate(4));
		//cout << " / " << a_site->eventStack->GetStackRate() << endl;
	}
	else
		a_site->GetEventStack()->ResetStackRate();

	totalrate += a_site->GetEventStack()->GetStackRate();
}

void BdmLsim2::ResynchTotalRate()
{
	totalrate=0.0;
	vector<Node*> nodes = this->GetNetwork()->get_nodes();
	vector<Node*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		totalrate += BdmLsim2::GetSiteData(*it)->GetEventStack()->GetStackRate();
	}
}


void BdmLsim2::CheckInputs()
{
	bool ok = true;

	if (d  < 0.0 || d  > 1.0) ok = false;
	if (b  < 0.0 || b  > 1.0) ok = false;
	if (mu < 0.0 || mu > 1.0) ok = false;
	if (mo < 0.0 || mo > 1.0) ok = false;
	if (x1 < 0.0 || x1 > 1.0) ok = false;
	if (x2 < 0.0 || x2 > 1.0) ok = false;
	if (L  < 0.0 || L  > 1.0) ok = false;

	if (!ok)
	{
		cerr << "The given d, b, mu, mo, x1, x2 or L value is not in range [0.0, 1.0]" << endl;
		exit(-1);
	}
}

void BdmLsim2::PrintInputs()
{
	cout << "-------------------------------------" << endl;
	cout << "Input params for repetition '" << GetCurrentRepetition() << "' :" << endl;
	cout << "-------------------------------------" << endl;
	cout << "d  = " << d  << endl;
	cout << "b  = " << b  << endl;
	cout << "mu = " << mu << endl;
	cout << "mo = " << mo << endl;
	cout << "x1 = " << x1 << endl;
	cout << "x2 = " << x2 << endl;
	cout << "L  = " << L  << endl;
	cout << "-------------------------------------" << endl;
}

void BdmLsim2::UpdateInputs()
{
	if (d2  != GSL_NAN) d  += d_s;
	if (b2  != GSL_NAN) b  += b_s;
	if (mu2 != GSL_NAN) mu += mu_s;
	if (mo2 != GSL_NAN) mo += mo_s;
	if (x12 != GSL_NAN) x1 += x1_s;
	if (x22 != GSL_NAN) x2 += x2_s;
	if (L2  != GSL_NAN) L  += L_s;
}


unsigned int BdmLsim2::Census()
{
	unsigned int nx = 0, nox = 0, nxx = 0, noox = 0, nxox = 0;


	vector<Node*> nodes = this->GetNetwork()->get_nodes();
	vector<Node*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		if (GetSiteData(*it)->GetState() != BdmLsim2::EMPTY)
		{
			nx++;

			vector<Node*> nghs = (*it)->get_neighbors();
			for (vector<Node*>::iterator it2 = nghs.begin(); it2 != nghs.end(); it2++)
			{
				if (GetSiteData(*it2)->GetState() != BdmLsim2::EMPTY)
				{
					nox++;
					vector<Node*> nghs = (*it2)->get_neighbors();
					for (vector<Node*>::iterator it3 = nghs.begin(); it3 != nghs.end(); it3++)
					{
						if(GetSiteData(*it3)->GetState() == BdmLsim2::EMPTY)
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
			line.push_back(GetSiteData(*it)->GetState());
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


void BdmLsim2::RealizeEvent()
{
	double add;
	double p1;
	t_site *a_site;

	vector<Node*> nodes = this->GetNetwork()->get_nodes();
	vector<Node*>::iterator it;

	// Choose a site
	add = 0.0;
	p1 = gsl_rng_uniform(rng) * totalrate;
	it = nodes.begin();
	for (;;)
	{
		add += BdmLsim2::GetSiteData(*it)->GetEventStack()->GetStackRate();
		if (p1 < add) break;
		it++;
	}
	a_site = BdmLsim2::GetSiteData(*it);

	// Choose an event and realize it
	//	add = 0.0;
	//	p1 = gsl_rng_uniform(rng) * a_site.eventStack->GetStackRate();
	//
	//	add += a_site.eventStack->GetEventRate(BdmLsim2::BIRTH);
	//	if(p1 < add)
	//		RealizeBirth(*it);
	//	else
	//	{
	//		add += a_site.eventStack->GetEventRate(BdmLsim2::DEATH);
	//		if(p1 < add)
	//			RealizeDeath(*it);
	//		else
	//		{
	//			add += a_site.eventStack->GetEventRate(BdmLsim2::MOVEMENT);
	//			if(p1 < add)
	//				RealizeMovement(*it);
	//			else
	//				RealizeMutation(*it);
	//		}
	//	}

	p1 = gsl_rng_uniform(rng);
	a_site->GetEventStack()->RealizeEvent(p1, GetTime());

}


void BdmLsim2::DumpLattice()
{
	unsigned int i = 0;

	vector<double> a_line;

	vector<Node*> nodes = this->GetNetwork()->get_nodes();
	vector<Node*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		a_line.clear();
		a_line.push_back(i);
		a_line.push_back(GetSiteData(*it)->GetState());
		a_line.push_back(GetSiteData(*it)->GetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim2::X));
		outputer3->AddDataLine(a_line);
		i++;
	}

	outputer3->WriteData();
	outputer3->ResetDataSet();
}

void BdmLsim2::SetOutputFilenames()
{
	stringstream ss;
	ss << output_files_prefix << "." << run_id << "." << GetCurrentRepetition();
	outputFilename = ss.str();

	outputFilename2 = "spat-" + outputFilename;

	ss.str("");
	ss << "dump.state-L" << L << "-" << outputFilename;
	outputFilename3 = ss.str();

	if (outputer  != NULL) outputer->SetFilename(outputFilename);
	if (outputer2 != NULL) outputer2->SetFilename(outputFilename2);
	if (outputer3 != NULL) outputer3->SetFilename(outputFilename3);

}

/*
 * Virtual stuffs implementation
 */

void BdmLsim2::InitSimulation()
{
	//tt = 0.0;
	totalrate = 0.0;

	rng = LabRngUtils::CreateRandomizer();


	InitPopulation();

	outputer = NULL;
	outputer2 = NULL;
	outputer3 = NULL;

	if (outputer == NULL) {
		outputer = new LabOutputerLine();
		outputer->SetOuputMode(LabOutputerLine::TO_BOTH);
		outputer->SetSeparator("\t");
//		outputer->SetFilename(outputFilename);
	}
	if (outputer2 == NULL) {
		outputer2 = new LabOutputerLine();
		outputer2->SetOuputMode(LabOutputerLine::TO_FILE);
		outputer2->SetSeparator("\t");
//		outputer2->SetFilename(outputFilename2);
	}
	if (outputer3 == NULL) {
		outputer3 = new LabOutputerLine();
		outputer3->SetOuputMode(LabOutputerLine::TO_FILE);
		outputer3->SetSeparator("\t");
//		outputer3->SetFilename(outputFilename3);
	}

	SetOutputFilenames();

	UpdateEventsRate(totalrate);
	SetEndTime(tt_end);
	SetOutputTimeStep(tt_out);

}


void BdmLsim2::BeforeStep() {
	UpdateEventsRate(totalrate);
	//tt = GetTime();
}

void BdmLsim2::AfterStep() {
	//tt = GetTime();
}


void BdmLsim2::FireEvent() {

	//cout << "t = " << GetTime() << "\t\t";

	// Choose and realize an event
	RealizeEvent();

	// Periodically recalculate totalrate
	if (GetNbFiredEvents() % resynch == 0)
		ResynchTotalRate();

	UpdateEventsRate(totalrate);

}

// Use preferably the 'LabOutputer' utility in this hook
void BdmLsim2::OutputSimulation()
{
//	// Census pop
//	Census();
//
//	// Periodically dump lattice state
//	DumpLattice();
}

void BdmLsim2::BeforeRun()
{
	// Get begin time
	char *str = (char *) malloc (80 * sizeof(char));
	str[0] = '\0';
	gettimeofday(&tvBegin, NULL);
	LabTimeUtils::TimevalPrint(str, &tvBegin);
	cout << str;

	// Print input params
	PrintInputs();

	// Save the generated network
	if (do_dump && !do_start_from_dump)
		SaveNetwork(net_dump_filename);
}

void BdmLsim2::AfterRun()
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
}


void BdmLsim2::BinarizeAll()
{
	// Binarize current simulation time
	LabBinaryUtils::WriteValueToStream(GetTime(), this->os);

	// Binarize current simulation repetition
	LabBinaryUtils::WriteValueToStream(GetCurrentRepetition(), this->os);

	// Binarize all sites
	vector<Node*> nodes = this->GetNetwork()->get_nodes();
	vector<Node*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		GetSiteData(*it)->Binarize(this->os);
	}
}

void BdmLsim2::UnBinarizeAll()
{
	// UnBinarize current simulation time
	double time;
	LabBinaryUtils::ReadValueFromStream(time, this->is);
	SetTime(time);

	// UnBinarize current simulation repetition
	int rep_num;
	LabBinaryUtils::ReadValueFromStream(rep_num, this->is);
	SetCurrentRepetition(rep_num);
	// Update filenames according to the rep num
	SetOutputFilenames();


	cout << "Starting at t = " << GetTime();
	cout << " (rep = " << GetCurrentRepetition() << ")" << endl;

	// UnBinarize all sites
	vector<Node*> nodes = this->GetNetwork()->get_nodes();
	vector<Node*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		GetSiteData(*it)->UnBinarize(this->is);
	}
}



// Override simulation default run behaviour
void BdmLsim2::Run()
{
	for (int i=0; i<nb_rep; i++)
	{
		// Run a repetition
		RunSimulation();
		UpdateInputs();
	}
}

int main(int argc, char **argv) {

	BdmLsim2 *sim = new BdmLsim2();

	// Initialize with default hardcoded values
	sim->DefaultInputs();

	// Replace input model configuration by values from CL or XML
	xml_input_filename = sim->ParseCL(argc, argv);
	if (xml_input_filename != "")
	{
		cout << "Loading XML model : " << xml_input_filename << endl;
		sim->ParseXML(xml_input_filename);
	}
	// No more required : Done in 'parseCL' and 'parseXML' now
	//	sim->t_dump_step = 1.0;			// Expressed in seconds
	//	sim->do_dump = true;
	//	sim->do_start_from_dump = true;// Turn on/off simulation data recovery

	sim->CheckInputs();

	ofstream os;
	ifstream is;
	sim->SetBinaryStreams(&os, &is);


	// Initialize network

	if (sim->do_start_from_dump)
	{
		sim->LoadNetwork(net_dump_filename);
	}
	else
	{
		LabNetwork *net = new LabNetwork();
		net->populate(nb_sites);
		//net->square_lattice()
		if (( (int) con_deg * net->size()) % 2 == 1) {
			cout << ("The sum of all degrees must be even\nThis is not possible with the network parameters you have specified");
			return false;
		}
		vector<double> dist(con_deg+1, 0);
		dist[con_deg] = 1;
		cout << "CONNECTION ? " << net->rand_connect_user(dist) << endl;
		sim->SetNetwork(net);
	}
	cout << "NB NETWORK NODES = " << sim->GetNetwork()->get_nodes().size() << endl;

	sim->Run();
}




