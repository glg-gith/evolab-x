/*
 * BdmLsim3.cpp
 *
 *  Created on: 18 sept. 2012
 *      Author: golgauth
 *
 *  This demo is equivalent to BdmLsim4 demo.
 *  Only the way the sites are populated change :
 *  done individually site-by-site (See InitPopulation()).
 */


/**********************************************/
/* BIRTH-DEATH-MIGRATION MODEL                */
/* Sebastien Lion                             */
/**********************************************/


#include "BdmLsim3.h"

#include <gsl/gsl_nan.h>

using namespace std;


//**  GLOBALS  **//			Lazy way : should be class attributes
string run_id	= "0";
int nb_rep		= 1;
int nb_sites 	= 100000;//100;//
int con_deg 	= 4;				// Number of connected neighbors per node
int nb_cols 	= 100;//10;//		// Number of columns for display

double p_init 	= 0.5;//0.01//		// Proba to be a RESIDENT against an EMPTY site at start
int tt_out 		= 1;   				// Output every tt_out time step
int tt_out_st	= 5;				// Output states every tt_out_st time step
int tt_end 		= 50;  				// When simulation ends

int resynch		= 500; 				// Recalculate totalrate periodically to avoid numerical drift


double totalrate;
unsigned int popcnt;           		// Total pop size (nb of non-empty sites)

string output_files_prefix;
string net_dump_filename;			// where to dump the network connections as text edges list
string xml_input_filename;			// XML file to be parsed


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
LabRng *rng;

// Timer
LabTimer *timer;

// CALCULATOR
LabSiteSetsCalculator *_C;


BdmLsim3::BdmLsim3() {
	// TODO Auto-generated constructor stub
	outputer = NULL;
	outputer2 = NULL;
	outputer3 = NULL;
	output_files_prefix = "output";
}

BdmLsim3::~BdmLsim3() {
	// TODO Auto-generated destructor stub
}


// RATES CALLBACKS :
// [Here must fit the required prototype : double CallbackName(void *)]

double NoneRate(EventCallBackParam node) { return 0.0; }

double BirthRate(EventCallBackParam node)
{
	LabNode *a_node = (LabNode *)node;
	double rate;
	BdmLsim3::t_site *a_site = BdmLsim3::GetSiteData(a_node);
	rate = b * a_site->GetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::X);
	if(a_site->GetState() == BdmLsim3::MUTANT)
		rate *= 0.9;
	return rate;
}

double DeathRate(EventCallBackParam node)
{
	LabNode *a_node = (LabNode *)node;
	BdmLsim3::t_site *a_site = BdmLsim3::GetSiteData(a_node);
	return d * a_site->GetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::X);
}

double MovementRate(EventCallBackParam node)
{
	LabNode *a_node = (LabNode *)node;
	double rate;
	rate = BdmLsim3::GetSiteData(a_node)->GetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::M);
	////rate *= 1.0 - (double)BdmLsim3::NbOccupiedNghs(a_node) / (double)con_deg;
	return rate;
}



double MutationRate(EventCallBackParam node)
{
	//LabNode *a_node = (LabNode *)node;
	return mu;
}


// REALIZES CALLBACKS :
// [Here must fit the required prototype : double CallbackName(void *)]
// [but we skip the return value, which we won't use anywhere]

double RealizeNone(EventCallBackParam node) { cout << "NONE fired !" << endl; }

double RealizeBirth(EventCallBackParam node)
{
	LabNode *a_node = (LabNode *)node;
	//cout << "BIRTH fired !" << endl;

	int birth_neighbour, birth_site;

	LabNode *n_birthsite;
	BdmLsim3::t_site *s_data, *bs_data;

	// Reproduce globally with probability L
	if(rng->GetNextDouble() < L)
	{
		do
		{
			birth_site = rng->GetNextInt(nb_sites);
			n_birthsite = a_node->GetNetwork()->GetNode(birth_site);
		}
		while (n_birthsite == a_node);
	}
	else
		// Locally with probability 1-L
	{
		birth_neighbour = rng->GetNextInt(con_deg);
		n_birthsite = a_node->GetNeighbors()[birth_neighbour];
	}

	// Reproduction only if birth_site is empty
	s_data = BdmLsim3::GetSiteData(a_node);
	bs_data = BdmLsim3::GetSiteData(n_birthsite);
	if (bs_data->GetStateStack()->GetCurrentState() == BdmLsim3::EMPTY)
	{
		// Sites update
		bs_data->SetState(s_data->GetState(), s_data->GetTimeEvent());
		bs_data->Copy(s_data, LabSiteBase::IND_DEP);

		// Rates update
		// "BdmLsim3::some_static_function" : allows to call "some_static_function"
		// without having instantiated an object of the class BdmLsim3.
		BdmLsim3::UpdateRatesLocal(a_node);
		BdmLsim3::UpdateRatesLocal(n_birthsite);

		// Population size update
		popcnt++;
	}

}

double RealizeDeath(EventCallBackParam node)
{
	LabNode *a_node = (LabNode *)node;
	//cout << "DEATH fired !" << endl;

	// Switch to EMPTY state
	BdmLsim3::t_site *s_data;
	s_data = BdmLsim3::GetSiteData(a_node);
	s_data->SetState(BdmLsim3::EMPTY, s_data->GetTimeEvent());
	s_data->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::X, 0.0);
	s_data->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::M, 0.0);

	// Rates update
	BdmLsim3::UpdateRatesLocal(a_node);

	popcnt--;

}

double RealizeMovement(EventCallBackParam node)
{
	LabNode *a_node = (LabNode *)node;
	//cout << "MOVEMENT fired !" << endl;

	int move_site;
	LabNode *n_movesite;
	BdmLsim3::t_site *s_data, *ms_data;

	// Choose a site to move to
	move_site = rng->GetNextInt(con_deg);
	n_movesite = a_node->GetNeighbors()[move_site];

	// Try moving
	s_data = BdmLsim3::GetSiteData(a_node);
	ms_data = BdmLsim3::GetSiteData(n_movesite);
	if (ms_data->GetStateStack()->GetCurrentState() == BdmLsim3::EMPTY)
	{
		// Copy old location properties to the new one :
		ms_data->Copy(s_data, LabSiteBase::IND_DEP);

		// Switch old location to EMPTY state :
		s_data->SetState(BdmLsim3::EMPTY, s_data->GetTimeEvent());
		s_data->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::X, 0.0);
		s_data->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::M, 0.0);

		// Rates update
		BdmLsim3::UpdateRatesLocal(a_node);
		BdmLsim3::UpdateRatesLocal(n_movesite);
	}

}

double RealizeMutation(EventCallBackParam node)
{
	LabNode *a_node = (LabNode *)node;
	//cout << "MUTATION fired !" << endl;

	// Mutate
	BdmLsim3::t_site *s_data;
	s_data = BdmLsim3::GetSiteData(a_node);
	if(s_data->GetState() == BdmLsim3::RESIDENT)
	{
		s_data->SetState(BdmLsim3::MUTANT, s_data->GetTimeEvent());
		s_data->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::X, x2);
	}
	else
	{
		s_data->SetState(BdmLsim3::RESIDENT, s_data->GetTimeEvent());
		s_data->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::X, x1);
	}

	// Rates update
	BdmLsim3::UpdateRatesLocal(a_node);

}



/*
 * BdmLsim3 CLASS dedicated stuffs
 */


/**
 * Default inputs which can be overwritten later on by ParseCL() or ParseXML()
 */
void BdmLsim3::DefaultInputs()
{

	d 	= 0.01;	 d2 	= GSL_NAN;		// Death rate
	b 	= 0.01;	 b2 	= GSL_NAN;		// Birth rate
	mu 	= 0.01;	 mu2 	= GSL_NAN;		// Mutation rate
	mo 	= 0.01;	 mo2 	= GSL_NAN;		// Movement rate
	x1 	= 0.01;	 x12 	= GSL_NAN;		// Trait coeff for Resident
	x2 	= 0.02;	 x22 	= GSL_NAN;		// Trait coeff for Mutant
	L 	= 0.01;	 L2 	= GSL_NAN;		// Reproduction rate

	net_dump_filename = "bdmLsim.res.lat." + run_id;
	SetNbRepetitions(nb_rep);

    SetTimeDumpStep(1);
    SetDoDump(false);
    SetDoStartFromDump(false);
}

/**
 * Parsing the command line arguments
 * @param argc
 * @param argv
 * @return
 */
string BdmLsim3::ParseCL(int argc, char **argv)
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

		this->SetTimeDumpStep(ds_arg.getValue());
		this->SetDoDump(dd_switch.getValue());
		this->SetDoStartFromDump(r_switch.getValue());

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

/**
 * Parsing inputs from input XML file
 * (The file can be given by passing "-i filename" as argument to the
 * program executable).
 * TODO : In the this file could also be used to describe the simulation
 *        (ex : the algorithms to be used in the above callbacks...)
 * @param filename
 */
void BdmLsim3::ParseXML(string filename)
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
		else if (str == "dump-step") {
			double t_dump_step;
			iss >> t_dump_step;
			this->SetTimeDumpStep(t_dump_step);
		}
		else if (str == "do-dump")
			this->SetDoDump((str == "True"));
		else if (str == "recovery")
			this->SetDoStartFromDump((str == "True"));
	}


	// Set net dump filename
	ss.str("");
	ss << output_files_prefix << ".lat." << run_id; // << "." << GetCurrentRepetition();
	net_dump_filename = ss.str();

	SetNbRepetitions(nb_rep);

	xmlFreeDoc(doc);

}

/**
 * Setup the initial state of the population.
 * A site (empty or not) is assigned to every node in the network.
 */
void BdmLsim3::InitPopulation()
{
	// Critical maximum size the simulation can handle. In spatial context,
	// this corresponds to the size of the network itself.
	this->SetMaxPopulationSize(nb_sites);


	// Opens the NetBinding for editing sites and maintain the "sets tracking" features
	// (like "_C->GS("RESIDENT").Cardinality()"). By default all sites will be a member
	// of the "U" set (Univers).
	// This call is mandatory to get SetOneSiteAt() to work.
	this->BeginSetSites();

	popcnt = 0;
	t_site *a_site;
	vector<LabNode*> nodes = this->GetNetwork()->GetNodes();
	vector<LabNode*>::iterator it;

	// Iterating through ALL nodes : Here we MUST set every single site to a
	// value (!=NULL) before leaving (EndSetSites()).
	int cnt = -1;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		// Uncomment to test what happens if the last site is not set...
		cnt++;
		if (cnt == nodes.size()-1) break;

		// Setup a new site.
		// Here this will be a "LabSiteEvent", events occur at the site level in this
		// simulation.
		a_site = new LabSiteEvent(this, *it);
		// ATTRIBUTES
		// Indexed attribute : here, 3 sets/groups are created (pop0, pop1, pop2)
		// This is only for demonstration because we don't use it in this simulation.
		// For a real use case of "indexed attribute", see the "DrugRes" demo.
		a_site->PushAttribute("pop", LabSiteBase::IND_DEP, true, 3);
		// Classical attributes
		a_site->PushAttribute("x", LabSiteBase::IND_DEP);
		a_site->PushAttribute("m", LabSiteBase::IND_DEP);

		// STATES
		// 1, -1, -1 => next_id = 1, timeout_id = -1, timeout = -1.0 : we don't use those
		// features (but they're described in "LabStateStack" class documentation).
		// This will create 3 additional sets (one per possible state)
		a_site->PushState("EMPTY",    1, -1, -1);
		a_site->PushState("RESIDENT", 1, -1, -1);
		a_site->PushState("MUTANT",   1, -1, -1);

		// Choose whether the current site is in EMPTY or RESIDENT state
		if(rng->GetNextDouble() < p_init)
		{
			// This site will be a member of the "RESIDENT" set
			a_site->SetState(BdmLsim3::RESIDENT, GetTime());

			a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::POP, 1);
			a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::X, x1);
			//a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::M, mo);
			a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::M, 0.0);

			popcnt++;
		}
		else
		{
			// This site will be a member of the "EMPTY" set
			a_site->SetState(BdmLsim3::EMPTY, GetTime());

			a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::POP, 2);
			a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::X, 0.0);
			a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::M, 0.0);
		}

		// EVENTS
		// Adds "rate" and "event" callback of each type to the current site
		// Aka : here, all sites have the same potentialities

		RateCallBack *rate_callback;
		RealizeCallBack *realize_callback;

		FNoneRate = &NoneRate;
		FRealizeNone = &RealizeNone;
		rate_callback = new RateCallBack(FNoneRate);
		realize_callback = new RealizeCallBack(FRealizeNone);
		a_site->PushEvent("NONE", 0, (EventCallBackParam)(*it), rate_callback, realize_callback);

		FBirthRate = &BirthRate;
		FRealizeBirth = &RealizeBirth;
		rate_callback = new RateCallBack(FBirthRate);
		realize_callback = new RealizeCallBack(FRealizeBirth);
		a_site->PushEvent("BIRTH", 0, (EventCallBackParam)(*it), rate_callback, realize_callback);

		FDeathRate = &DeathRate;
		FRealizeDeath = &RealizeDeath;
		rate_callback = new RateCallBack(FDeathRate);
		realize_callback = new RealizeCallBack(FRealizeDeath);
		a_site->PushEvent("DEATH", 0, (EventCallBackParam)(*it), rate_callback, realize_callback);

		FMovementRate = &MovementRate;
		FRealizeMovement = &RealizeMovement;
		rate_callback = new RateCallBack(FMovementRate);
		realize_callback = new RealizeCallBack(FRealizeMovement);
		a_site->PushEvent("MOVEMENT", 0, (EventCallBackParam)(*it), rate_callback, realize_callback);

		FMutationRate = &MutationRate;
		FRealizeMutation = &RealizeMutation;
		rate_callback = new RateCallBack(FMutationRate);
		realize_callback = new RealizeCallBack(FRealizeMutation);
		a_site->PushEvent("MUTATION", 0, (EventCallBackParam)(*it), rate_callback, realize_callback);


		// Would have been used if we had wanted to populate the nodes without sets features
		// (like "_C->GS("RESIDENT").Cardinality()"). In this case evrything would have been let
		// to the user's responsibility, and BeginSetSites(), SetOneSiteAt() and EndSetSites()
		// wouldn't have been necessary.
		//
		//(*it)->SetUserData(a_site);
		//
		// But we chose "sets tracking"
		// which, also, takes in charge to "SetUserData()", so we can retrieve a node's site by
		// simply calling "node->GetUserData()"
		this->SetOneSiteAt(a_site, (*it)->get_id());
	}

	// Closes the NetBinding for editing
	this->EndSetSites();


	// Update global and local rates for each node
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		UpdateRates(*it);
	}
	// Synchronize once the totalrate
	ResynchTotalRate();

	// The traditional way of counting : we have to loop and count by hand
	int empty = 0, occupied = 0;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		if (  ( (LabSiteEvent *)(*it)->GetUserData() )->GetState() == BdmLsim3::EMPTY  ) empty++;
		else occupied++;
	}
	cout << "OCCUPATION START >> " << empty << " | " << occupied << endl;

	// The new way, using sets features : quantities are instantaneously accessible (Cardinality())
	int U_cnt = _C->CNT(_C->GS("U"));
	empty = _C->CNT(_C->GS("EMPTY"));
	occupied = _C->GS("RESIDENT").Cardinality();//.CU(_C->GS("MUTANT"));

	cout << "OCCUPATION >> " << U_cnt << " | " << empty << " | " << occupied << endl;

}

/**
 * Shortcut to extract the site from a given node (automates the cast to "t_site" = "LabSiteEvent")
 * @param a_node
 * @return
 */
BdmLsim3::t_site *BdmLsim3::GetSiteData(LabNode *a_node)
{
	return (BdmLsim3::t_site *)(a_node->GetUserData());
}

/**
 * Counts the number of neighbors that are nit in EMPTY state
 * @param a_node
 * @return
 */
int BdmLsim3::NbOccupiedNghs(LabNode *a_node)
{
	int nb = 0;
	bool occupied;
	vector<LabNode*> nghs = a_node->GetNeighbors();
	for (vector<LabNode*>::iterator it = nghs.begin(); it != nghs.end(); it++)
	{
		occupied = ( BdmLsim3::GetSiteData(*it)->GetState() != BdmLsim3::EMPTY );
		if (occupied) nb++;
	}
	//if (nb) cout << nb << endl;
	return nb;
}

/**
 * Updates the neighbors rates. Must be called each time an event occur on a given site.
 * @param a_node
 */
void BdmLsim3::UpdateRatesLocal(LabNode *a_node)
{
	vector<LabNode*>::iterator it;
	vector<LabNode*> nghs;

	BdmLsim3::UpdateRates(a_node);
	nghs = a_node->GetNeighbors();
	for (it = nghs.begin(); it != nghs.end(); it++)
	{
		BdmLsim3::UpdateRates(*it);
	}
}

/**
 * Updates the event rates for the given site.
 * @param a_node
 */
void BdmLsim3::UpdateRates(LabNode *a_node)
{
	t_site *a_site = GetSiteData(a_node);

	totalrate -= a_site->GetEventStackRate();

	if(a_site->GetState() != BdmLsim3::EMPTY)
		a_site->UpdateEventStackRates();
	else
		a_site->ResetEventStackRate();

	totalrate += a_site->GetEventStackRate();
}

/**
 * Resynchronizes the totalrate
 */
void BdmLsim3::ResynchTotalRate()
{
	totalrate=0.0;
	vector<LabNode*> nodes = this->GetNetwork()->GetNodes();
	vector<LabNode*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		totalrate += BdmLsim3::GetSiteData(*it)->GetEventStackRate();
	}
}

/**
 * Checks the validity of the inputs values
 */
void BdmLsim3::CheckInputs()
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

/**
 * Prints the inputs used for the current repetition
 */
void BdmLsim3::PrintInputs()
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

/**
 * Not used : originally used to change the inputs incrementally at each new repetition
 */
void BdmLsim3::UpdateInputs()
{
	if (d2  != GSL_NAN) d  += d_s;
	if (b2  != GSL_NAN) b  += b_s;
	if (mu2 != GSL_NAN) mu += mu_s;
	if (mo2 != GSL_NAN) mo += mo_s;
	if (x12 != GSL_NAN) x1 += x1_s;
	if (x22 != GSL_NAN) x2 += x2_s;
	if (L2  != GSL_NAN) L  += L_s;
}

/**
 * Gets some stats
 * @return
 */
unsigned int BdmLsim3::Census()
{
	unsigned int nx = 0, nox = 0, nxx = 0, noox = 0, nxox = 0;


	vector<LabNode*> nodes = this->GetNetwork()->GetNodes();
	vector<LabNode*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		if (GetSiteData(*it)->GetState() != BdmLsim3::EMPTY)
		{
			nx++;

			vector<LabNode*> nghs = (*it)->GetNeighbors();
			for (vector<LabNode*>::iterator it2 = nghs.begin(); it2 != nghs.end(); it2++)
			{
				if (GetSiteData(*it2)->GetState() != BdmLsim3::EMPTY)
				{
					nox++;
					vector<LabNode*> nghs = (*it2)->GetNeighbors();
					for (vector<LabNode*>::iterator it3 = nghs.begin(); it3 != nghs.end(); it3++)
					{
						if(GetSiteData(*it3)->GetState() == BdmLsim3::EMPTY)
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

/**
 * Fires an event on a randomly chosen site
 */
void BdmLsim3::RealizeEvent()
{
	double add;
	double p1;
	t_site *a_site;

	vector<LabNode*> nodes = this->GetNetwork()->GetNodes();
	vector<LabNode*>::iterator it;

	// Choose a site
	// This could have been done with the specialized function LabRng.GetNextIntMultinomial()
	// (See the BdmLsim4 python demo to see it in action)
	add = 0.0;
	p1 = rng->GetNextDouble() * totalrate;
	it = nodes.begin();
	for (;;)
	{
		add += BdmLsim3::GetSiteData(*it)->GetEventStackRate();
		if (p1 < add) break;
		it++;
	}
	a_site = BdmLsim3::GetSiteData(*it);

	// Fires the event according to its propensity to occur
	p1 = rng->GetNextDouble();
	a_site->RealizeEvent(p1, GetTime());

}

/**
 * Saves the lattice to an output file. One  line per node.
 */
void BdmLsim3::DumpLattice()
{
	unsigned int i = 0;

	vector<double> a_line;

	vector<LabNode*> nodes = this->GetNetwork()->GetNodes();
	vector<LabNode*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		a_line.clear();
		a_line.push_back(i);
		a_line.push_back(GetSiteData(*it)->GetState());
		a_line.push_back(GetSiteData(*it)->GetAttrDouble(LabSiteBase::IND_DEP, BdmLsim3::X));
		outputer3->AddDataLine(a_line);
		i++;
	}

	outputer3->WriteData();
	outputer3->ResetDataSet();
}

/**
 * Initializes the outputers
 */
void BdmLsim3::SetOutputFilenames()
{
	stringstream ss;
	ss << output_files_prefix << "." << run_id << "." << GetCurrentRepetition();
	outputFilename = ss.str();

	outputFilename2 = "spat-" + outputFilename;

	ss.str("");
	ss << "dump.state-L" << L << "-" << outputFilename;
	outputFilename3 = ss.str();

	if (outputer  != NULL) outputer->SetFilename(outputFilename, !this->GetDoStartFromDump());
	if (outputer2 != NULL) outputer2->SetFilename(outputFilename2, !this->GetDoStartFromDump());
	if (outputer3 != NULL) outputer3->SetFilename(outputFilename3, !this->GetDoStartFromDump());

}


/*
 * Virtual stuffs implementation :
 * -----------------------------
 *
 * Here we re-implement the hooks that are specific to our simulation.
 * Each hook redefined here replaces the default behavior (from class "LabSimulatorTime"
 * BdmLsim3 inherits from).
 *
 * The default structure of RunSimulation() (the hook called at each repetition) is as
 * follows :
 *
 * <pre>
 * void LabSimulatorBase::RunSimulation()
 * {
 * 		InitSimulation();
 * 		ResetTime();
 * 		InitUnBinarize();
 * 		BeforeRun();
 * 		do
 * 		{
 * 			OutputSimulation()
 * 			BeforeStep();			// !! 'UpdateEventsRate()' must be called in there !!
 * 			StepSimulation();
 * 			AfterStep();
 * 			StepBinarize();
 * 		}
 * 		while (!EndSimulation());
 * 		OutputSimulation();
 * 		AfterRun();
 * }
 * </pre>
 *
 * Note : Also be aware that the total events rate is in constant evolution
 *        and by the way, has to be updated in the stepping process.
 *
 */


/**
 * Initializes the simulation.
 * Called once when sim->Run() is called.
 */
void BdmLsim3::InitSimulation()
{
	//tt = 0.0;
	totalrate = 0.0;

	// Setup the Random Number Generator
	rng = new LabRng();


	InitPopulation();	// Here is most the important part.

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

/**
 * Before firing an event.
 * Called at each iteration of the time loop
 */
void BdmLsim3::BeforeStep() {
	// The simulation needs to now the totalrate to properly compute
	// the continuous time (t = -log(p1) / totalrate, See LabSimulatorTime::TimeStep())
	UpdateEventsRate(totalrate);
}

/**
 * After firing an event.
 * Called at each iteration of the time loop
 */
void BdmLsim3::AfterStep() {
}


void BdmLsim3::FireEvent() {

	// Choose and realize an event
	RealizeEvent();

	// Periodically recalculate totalrate
	if (GetNbFiredEvents() % resynch == 0)
		ResynchTotalRate();

	UpdateEventsRate(totalrate);

}

// Use preferably the 'LabOutputer' utility in this hook
void BdmLsim3::OutputSimulation()
{
	/** Old stuffs for getting the right stats **/
	// Uncomment if you like

//	// Census pop
//	Census();
//
//	// Periodically dump lattice state
//	DumpLattice();

	/** Occupation : mainly for testing purpose of the "sets/groups" system **/

	LabOutputerLine::DrawProgressBar(50, this->GetTime() / tt_end, false, true);
	cout << endl;

	// Time spend to compute
	LabTimer *t = new LabTimer();
	t->Start();

	int empty = _C->CNT(_C->GS("EMPTY"));
	// Counts union of RESIDENT+MUTANT (number of occupied sites)
	int occupied = _C->GS("RESIDENT").CU(_C->GS("MUTANT"));
	cout << "OCCUPATION >> " << empty << " | " << occupied  << " ( " << _C->GS("RESIDENT").Cardinality() << " " << _C->GS("MUTANT").Cardinality() << " )" << endl;

	// Averages the common attributes/traits
	int attrs[] = { (int)X, (int)M };
	vector<int> v_attrs (attrs, attrs + sizeof(attrs) / sizeof(attrs[0]) );
	int deps[] = { (int)LabSiteBase::IND_DEP, (int)LabSiteBase::IND_DEP };
	vector<int> v_deps (deps, deps + sizeof(deps) / sizeof(deps[0]) );
	vector<double> avgs = _C->AVG( (_C->GS("RESIDENT") + _C->GS("MUTANT")), v_deps, v_attrs );
	cout << "MEAN X     >> " << avgs[0] << endl;
	cout << "MEAN M     >> " << avgs[1] << endl;


	t->Stop();
	t->Print();

}

/**
 * Called before each repetition/run (RunSimulation() hook)
 */
void BdmLsim3::BeforeRun()
{
	// Get begin time
	timer = new LabTimer();
	timer->Start();

	// Print input params
	PrintInputs();

	// Save the generated network
	if (GetDoDump() && !GetDoStartFromDump())
		SaveNetwork(net_dump_filename);
}

/**
 * Called after each repetition/run
 */
void BdmLsim3::AfterRun()
{
	// Get end time
	cout << "\n" << endl;
	timer->Stop();
    cout << "TOTAL SIMULATION TIME :" << endl;
	timer->Print();
	delete timer;
}


/**
 * This feature is under construction : not fully functional.
 * The goal is to be able to save the whole state of the simulation
 * to enable later recovery (restart at the last binarization point).
 */
void BdmLsim3::BinarizeAll()
{
	// Binarize current simulation time
	LabBinaryUtils::WriteValueToStream(GetTime(), this->os);

	// Binarize current simulation repetition
	LabBinaryUtils::WriteValueToStream(GetCurrentRepetition(), this->os);

	// Binarize all sites
	vector<LabNode*> nodes = this->GetNetwork()->GetNodes();
	vector<LabNode*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		GetSiteData(*it)->Binarize(this->os);
	}

	// TODO !!! (and report to Python)
//	// Binarize output recovery point for each output file
//	outputer->Binarize(this->os);
//	outputer2->Binarize(this->os);
//	outputer3->Binarize(this->os);
}

void BdmLsim3::UnBinarizeAll()
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
	vector<LabNode*> nodes = this->GetNetwork()->GetNodes();
	vector<LabNode*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		GetSiteData(*it)->UnBinarize(this->is);
	}

	// TODO !!! (and report to Python)
//	// UnBinarize output recovery point for each output file
//	outputer->UnBinarize(this->is);
//	outputer2->UnBinarize(this->is);
//	outputer3->UnBinarize(this->is);

}



// Override simulation default run behavior
void BdmLsim3::Run()
{
	for (int i=0; i<nb_rep; i++)
	{
		// Run a repetition
		RunSimulation();		// This is a hook defined in the "LabSimulatorTime" class
								// Here we keep the default behavior and call it as is.
		UpdateInputs();
	}
}

int main(int argc, char **argv) {

	BdmLsim3 *sim = new BdmLsim3();

	_C = new LabSiteSetsCalculator();

	// Initialize with default hardcoded values
	sim->DefaultInputs();

	// Replaces input model configuration by values from CL or XML, if any
	xml_input_filename = sim->ParseCL(argc, argv);
	if (xml_input_filename != "")
	{
		cout << "Loading XML model : " << xml_input_filename << endl;
		sim->ParseXML(xml_input_filename);
	}

	sim->CheckInputs();

	ofstream os;
	ifstream is;
	sim->SetBinaryStreams(&os, &is);


	// Initializes the network
	if (sim->GetDoStartFromDump())
	{
		// Reload from backup (feature under construction)
		sim->LoadNetwork(net_dump_filename);
	}
	else
	{
		LabNetwork *net = new LabNetwork();
		net->Populate(nb_sites);
		// Connect the nodes using one of the available algorithms
		// Here a linear distribution with "con_deg" neighbors per node
		bool ok = net->RandConnectUserLinear(con_deg);
		cout << "CONNECTION ? " << ok << endl;
		sim->SetNetwork(net);
	}
	cout << "NB NETWORK NODES = " << sim->GetNetwork()->GetSize() << endl;

	// Starts the simulation
	sim->Run();
}




