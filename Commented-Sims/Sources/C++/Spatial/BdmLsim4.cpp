/*
 * BdmLsim4.cpp
 *
 *  Created on: 18 sept. 2012
 *      Author: golgauth
 *
 *  This demo is equivalent to BdmLsim3 demo.
 *  Only the way the sites are populated change :
 *  done using "Populate" facilities given a list
 *  of models (See InitPopulation()).
 *
 *  Most of the features are already commented in "BdmLsim3.cpp",
 *  so that only specific stuffs are deeply described and marked
 *  with tag <\\BDMLSIM_4\\>.
 */


/**********************************************/
/* BIRTH-DEATH-MIGRATION MODEL                */
/* Sebastien Lion                             */
/**********************************************/

#include "BdmLsim4.h"

#include <gsl/gsl_nan.h>

using namespace std;


//**  GLOBALS  **//			Lazy way : should be class static const attributes
string run_id	= "0";
int nb_rep		= 1;
int nb_sites 	= 100000;//100;//
int con_deg 	= 4;					// Number of connected neighbors per node
int nb_cols 	= 100;//10;//			// Number of columns for display

double p_init 	= 0.5;//0.5;//0.01
int tt_out 		= 1;   					// Output with steps of one time unit
int tt_out_st	= 5;
int tt_end 		= 50;  					// When does simulation ends

int resynch		= 500; 					// Recalculate totalrate periodically to avoid numerical drift
										// (every "resynch" events)...

//double tt;
double totalrate;
//unsigned int popsize;         // Total pop size // Can be obtained by (~_C->GS("EMPTY")).Cardinality()  !!! ;-)

string output_files_prefix;
string net_dump_filename;		// Where to dump the network connections as text edges list
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
LabRng *rng;

// Timer
LabTimer *timer;

// CALCULATOR
LabSiteSetsCalculator *_C;



BdmLsim4::BdmLsim4() {
	// TODO Auto-generated constructor stub
	outputer = NULL;
	outputer2 = NULL;
	outputer3 = NULL;
	output_files_prefix = "output";
}

BdmLsim4::~BdmLsim4() {
	// TODO Auto-generated destructor stub
}


// RATES CALLBACKS :

double NoneRate(EventCallBackParam node) { return 0.0; }

double BirthRate(EventCallBackParam node)
{
	LabNode *a_node = (LabNode *)node;
	double rate;
	BdmLsim4::t_site *a_site = BdmLsim4::GetSiteData(a_node);
	rate = b * a_site->GetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::X);
	if(a_site->GetState() == BdmLsim4::MUTANT)
		rate *= 0.9;
	return rate;
}

double DeathRate(EventCallBackParam node)
{
	LabNode *a_node = (LabNode *)node;
	BdmLsim4::t_site *a_site = BdmLsim4::GetSiteData(a_node);
	return d * a_site->GetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::X);
}

double MovementRate(EventCallBackParam node)
{
	LabNode *a_node = (LabNode *)node;
	double rate;
	rate = BdmLsim4::GetSiteData(a_node)->GetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::M);
	////rate *= 1.0 - (double)BdmLsim4::NbOccupiedNghs(a_node) / (double)con_deg;
	return rate;
}



double MutationRate(EventCallBackParam node)
{
	//LabNode *a_node = (LabNode *)node;
	return mu;
}


// REALIZES CALLBACKS :

double RealizeNone(EventCallBackParam node) { cout << "NONE fired !" << endl; }

double RealizeBirth(EventCallBackParam node)
{
	LabNode *a_node = (LabNode *)node;
	//cout << "BIRTH fired !" << endl;

	int birth_neighbour, birth_site;

	LabNode *n_birthsite;
	BdmLsim4::t_site *s_data, *bs_data;

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
	s_data = BdmLsim4::GetSiteData(a_node);
	bs_data = BdmLsim4::GetSiteData(n_birthsite);
	if (bs_data->GetStateStack()->GetCurrentState() == BdmLsim4::EMPTY)
	{
		// Sites update
		bs_data->Copy(s_data, LabSiteBase::IND_DEP);
		////bs_data->SetState(s_data->GetState(), s_data->GetTimeEvent()); // Already done by "Copy" !

		// Rates update
		BdmLsim4::UpdateRatesLocal(a_node);
		BdmLsim4::UpdateRatesLocal(n_birthsite);

		// Population size update
		//popsize++;
	}

}

double RealizeDeath(EventCallBackParam node)
{
	LabNode *a_node = (LabNode *)node;
	//cout << "DEATH fired !" << endl;

	// Switch to EMPTY state
	BdmLsim4::t_site *s_data;
	s_data = BdmLsim4::GetSiteData(a_node);
	s_data->SetState(BdmLsim4::EMPTY, s_data->GetTimeEvent());
	s_data->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::X, 0.0);
	s_data->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::M, 0.0);

	// Rates update
	BdmLsim4::UpdateRatesLocal(a_node);

	//popsize--;

}

double RealizeMovement(EventCallBackParam node)
{
	LabNode *a_node = (LabNode *)node;
	//cout << "MOVEMENT fired !" << endl;

	// TODO : The following should work : but is UNTESTED !!!
	// Ask Seb about this...
	int move_site;
	LabNode *n_movesite;
	BdmLsim4::t_site *s_data, *ms_data;

	// Choose a site to move to
	move_site = rng->GetNextInt(con_deg);
	n_movesite = a_node->GetNeighbors()[move_site];

	// Try moving
	s_data = BdmLsim4::GetSiteData(a_node);
	ms_data = BdmLsim4::GetSiteData(n_movesite);
	if (ms_data->GetStateStack()->GetCurrentState() == BdmLsim4::EMPTY)
	{
		// Copy old location properties to the new one :
		ms_data->Copy(s_data, LabSiteBase::IND_DEP);

		// <\\BDMLSIM_4\\>
		// Switch old location to EMPTY state :
		// Note : could also have been done by copy of the "EMPTY site" model
		//        (See "model_sites[0]" in BdmLsim4.InitPopulation() for a deeper
		//		   description of the "models based" population approach).
		s_data->SetState(BdmLsim4::EMPTY, s_data->GetTimeEvent());
		s_data->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::X, 0.0);
		s_data->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::M, 0.0);

		// Rates update
		BdmLsim4::UpdateRatesLocal(a_node);
		BdmLsim4::UpdateRatesLocal(n_movesite);
	}

}

double RealizeMutation(EventCallBackParam node)
{
	LabNode *a_node = (LabNode *)node;
	//cout << "MUTATION fired !" << endl;

	// Mutate
	BdmLsim4::t_site *s_data;
	s_data = BdmLsim4::GetSiteData(a_node);
	if(s_data->GetState() == BdmLsim4::RESIDENT)
	{
		s_data->SetState(BdmLsim4::MUTANT, s_data->GetTimeEvent());
		s_data->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::X, x2);
	}
	else
	{
		s_data->SetState(BdmLsim4::RESIDENT, s_data->GetTimeEvent());
		s_data->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::X, x1);
	}

	// Rates update
	BdmLsim4::UpdateRatesLocal(a_node);

}



/*
 * BdmLsim4 CLASS dedicated stuffs
 */


void BdmLsim4::DefaultInputs()
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

string BdmLsim4::ParseCL(int argc, char **argv)
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

void BdmLsim4::ParseXML(string filename)
{
	xmlDocPtr doc;
	xmlNodePtr root, consts;
	vector<xmlNodePtr> cst_vec;
	std::ostringstream ss;
	std::istringstream iss, iss2;
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

void BdmLsim4::InitPopulation()
{
	//popsize = 0;
	t_site *a_site;


	cout << "Initializing population..." << endl;

	this->SetMaxPopulationSize(nb_sites);


	// EVENTS (Common refs - callback functions)

	/** Create 2 models : 1 for Residents and 1 for Empties **/

	//		a_site->eventStack->PushEvent("NONE", 0);
	FNoneRate = &NoneRate;
	FRealizeNone = &RealizeNone;
	//		a_site->eventStack->PushEvent("BIRTH", 0);
	FBirthRate = &BirthRate;
	FRealizeBirth = &RealizeBirth;
	//		a_site->eventStack->PushEvent("DEATH", 0);
	FDeathRate = &DeathRate;
	FRealizeDeath = &RealizeDeath;
	//		a_site->eventStack->PushEvent("MOVEMENT", 0);
	FMovementRate = &MovementRate;
	FRealizeMovement = &RealizeMovement;
	//		a_site->eventStack->PushEvent("MUTATION", 0);
	FMutationRate = &MutationRate;
	FRealizeMutation = &RealizeMutation;

	// <\\BDMLSIM_4\\>
	// Here, contrary to the "BdmLsim3 demo", we define only 2 models.
	// One for EMPTY sites (model_sites[0]) and one for RESIDENT sites (model_sites[1])
	vector<LabSiteBase *> model_sites;
	for (int i=0; i<2; i++) {
		a_site = new LabSiteEvent(this);

		// ATTRIBUTES
		a_site->PushAttribute("pop", LabSiteBase::IND_DEP, 0, true, 3);
		a_site->PushAttribute("x", LabSiteBase::IND_DEP);
		a_site->PushAttribute("m", LabSiteBase::IND_DEP);

		// STATES
		a_site->PushState("EMPTY", /*true,*/ 1, -1, -1);
		a_site->PushState("RESIDENT", /*true,*/ 1, -1, -1);
		a_site->PushState("MUTANT", /*true,*/ 1, -1, -1);

		if(i == 0)
		{
			a_site->SetState(BdmLsim4::EMPTY, GetTime());
			a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::POP, 2);
			a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::X, 0.0);
			a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::M, 0.0);
		}
		else
		{
			a_site->SetState(BdmLsim4::RESIDENT, GetTime());
			a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::POP, 1);
			a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::X, x1);
			//a_site->SetAttrDouble(LabAttributesStack::IND_DEP, BdmLsim4::M, mo);
			a_site->SetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::M, 0.0);

			//popsize++;
		}


		RateCallBack *rate_callback;
		RealizeCallBack *realize_callback;

		//		a_site->eventStack->PushEvent("NONE", 0);
		rate_callback = new RateCallBack(FNoneRate);
		realize_callback = new RealizeCallBack(FRealizeNone);
		a_site->PushEvent("NONE", 0, NULL, rate_callback, realize_callback);

		//		a_site->eventStack->PushEvent("BIRTH", 0);
		rate_callback = new RateCallBack(FBirthRate);
		realize_callback = new RealizeCallBack(FRealizeBirth);
		a_site->PushEvent("BIRTH", 0, NULL, rate_callback, realize_callback);

		//		a_site->eventStack->PushEvent("DEATH", 0);
		rate_callback = new RateCallBack(FDeathRate);
		realize_callback = new RealizeCallBack(FRealizeDeath);
		a_site->PushEvent("DEATH", 0, NULL, rate_callback, realize_callback);

		//		a_site->eventStack->PushEvent("MOVEMENT", 0);
		rate_callback = new RateCallBack(FMovementRate);
		realize_callback = new RealizeCallBack(FRealizeMovement);
		a_site->PushEvent("MOVEMENT", 0, NULL, rate_callback, realize_callback);

		//		a_site->eventStack->PushEvent("MUTATION", 0);
		rate_callback = new RateCallBack(FMutationRate);
		realize_callback = new RealizeCallBack(FRealizeMutation);
		a_site->PushEvent("MUTATION", 0, NULL, rate_callback, realize_callback);

		model_sites.push_back(a_site);
	}



	// Start initialization

	vector<LabNode*> nodes = this->GetNetwork()->GetNodes();
	vector<LabNode*>::iterator it;


	// <\\BDMLSIM_4\\>
	/** Two ways of populating : you can switch between them **/
	/**      Simply uncomment one and comment the other      **/

	// Easy populate the network from a distribution and a list of models
	// This is stochastic !
	// Cumsum of nomes must be 1.0 !!
	double d[] = {0.5, 0.5};
	vector<double> dist(d, d + sizeof(d) / sizeof(double) );
	this->Populate(dist, model_sites, true);
//	// Easy populate the network from fixed quantities and a list of models
//	// Cumsum of nomes must be equal to network's size !!
//	int d[] = {52000, 48000};
//	vector<int> dist(d, d + sizeof(d) / sizeof(int) );
//	this->Populate(dist, model_sites, true);

	//cout << endl << "Initializing population... Done !" << endl;


	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		UpdateRates(*it);
	}
	ResynchTotalRate();


	int empty = 0, occupied = 0;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		if (  ( (LabSiteEvent *)(*it)->GetUserData() )->GetState() == BdmLsim4::EMPTY  ) empty++;
		else occupied++;
	}
	cout << "OCCUPATION START >> " << empty << " | " << occupied << endl;

	int universe = _C->CNT(_C->GS("U"));
	empty = _C->CNT(_C->GS("EMPTY"));
	int unset = _C->CNT(_C->GS("UNSET"));				// Predefined set/group (not usefull here...)
	occupied = _C->CNT(_C->GS("U") - _C->GS("EMPTY"));

	cout << "OCCUPATION >> " << universe << " | " << empty << " | " << occupied << " | " << unset << endl;

}


BdmLsim4::t_site *BdmLsim4::GetSiteData(LabNode *a_node)
{
	return (BdmLsim4::t_site *)(a_node->GetUserData());
}


int BdmLsim4::NbOccupiedNghs(LabNode *a_node)
{
	int nb = 0;
	bool occupied;
	vector<LabNode*> nghs = a_node->GetNeighbors();
	for (vector<LabNode*>::iterator it = nghs.begin(); it != nghs.end(); it++)
	{
		occupied = ( BdmLsim4::GetSiteData(*it)->GetState() != BdmLsim4::EMPTY );
		if (occupied) nb++;
	}
	//if (nb) cout << nb << endl;
	return nb;
}

void BdmLsim4::UpdateRatesLocal(LabNode *a_node)
{
	vector<LabNode*>::iterator it;
	vector<LabNode*> nghs;

	BdmLsim4::UpdateRates(a_node);
	nghs = a_node->GetNeighbors();
	for (it = nghs.begin(); it != nghs.end(); it++)
	{
		BdmLsim4::UpdateRates(*it);
	}
}

void BdmLsim4::UpdateRates(LabNode *a_node)
{
	t_site *a_site = GetSiteData(a_node);

	totalrate -= a_site->GetEventStackRate();

	if(a_site->GetState() != BdmLsim4::EMPTY)
		a_site->UpdateEventStackRates();
	else
		a_site->ResetEventStackRate();

	totalrate += a_site->GetEventStackRate();
}

void BdmLsim4::ResynchTotalRate()
{
	totalrate=0.0;
	vector<LabNode*> nodes = this->GetNetwork()->GetNodes();
	vector<LabNode*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		totalrate += BdmLsim4::GetSiteData(*it)->GetEventStackRate();
	}
}


void BdmLsim4::CheckInputs()
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

void BdmLsim4::PrintInputs()
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

void BdmLsim4::UpdateInputs()
{
	if (d2  != GSL_NAN) d  += d_s;
	if (b2  != GSL_NAN) b  += b_s;
	if (mu2 != GSL_NAN) mu += mu_s;
	if (mo2 != GSL_NAN) mo += mo_s;
	if (x12 != GSL_NAN) x1 += x1_s;
	if (x22 != GSL_NAN) x2 += x2_s;
	if (L2  != GSL_NAN) L  += L_s;
}


unsigned int BdmLsim4::Census()
{
	unsigned int nx = 0, nox = 0, nxx = 0, noox = 0, nxox = 0;

	unsigned int nox2 = 0, nxx2 = 0, noox2 = 0, nxox2 = 0, toto1 = 0, toto2 = 0;


	vector<LabNode*> nodes = this->GetNetwork()->GetNodes();
	vector<LabNode*>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		if (GetSiteData(*it)->GetState() != BdmLsim4::EMPTY)
		{
			nx++;

			vector<LabNode*> nghs = (*it)->GetNeighbors();
			for (vector<LabNode*>::iterator it2 = nghs.begin(); it2 != nghs.end(); it2++)
			{
				if (GetSiteData(*it2)->GetState() != BdmLsim4::EMPTY)
				{
					nox++;
					vector<LabNode*> nghs = (*it2)->GetNeighbors();
					for (vector<LabNode*>::iterator it3 = nghs.begin(); it3 != nghs.end(); it3++)
					{
						if(GetSiteData(*it3)->GetState() == BdmLsim4::EMPTY)
						{
							noox++;
						}
						else
						{
							//if ((*it3) != (*it))
								nxox++;
								if(GetSiteData(*it3)->GetState() == BdmLsim4::RESIDENT)
									toto1++;
								if(GetSiteData(*it3)->GetState() == BdmLsim4::MUTANT)
									toto2++;
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

	//line.push_back(nx);
	line.push_back((~_C->GS("EMPTY")).Cardinality());

	line.push_back(nox);
	//line.push_back(nox2);

	line.push_back(nxx);
	//line.push_back(nxx2);

	line.push_back(nxox);
	//line.push_back(nxox2);

	line.push_back(noox);
	//line.push_back(noox2);


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


void BdmLsim4::RealizeEvent()
{
	double add;
	double p1;
	t_site *a_site;

	vector<LabNode*> nodes = this->GetNetwork()->GetNodes();
	vector<LabNode*>::iterator it;

	// Choose a site
	add = 0.0;
	p1 = rng->GetNextDouble() * totalrate;
	it = nodes.begin();
	for (;;)
	{
		add += BdmLsim4::GetSiteData(*it)->GetEventStackRate();
		if (p1 < add) break;
		it++;
	}
	a_site = BdmLsim4::GetSiteData(*it);

	p1 = rng->GetNextDouble();
	a_site->RealizeEvent(p1, GetTime());
}


void BdmLsim4::DumpLattice()
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
		a_line.push_back(GetSiteData(*it)->GetAttrDouble(LabSiteBase::IND_DEP, BdmLsim4::X));
		outputer3->AddDataLine(a_line);
		i++;
	}

	outputer3->WriteData();
	outputer3->ResetDataSet();
}

void BdmLsim4::SetOutputFilenames()
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
 * Virtual stuffs implementation
 */

void BdmLsim4::InitSimulation()
{
	//tt = 0.0;
	totalrate = 0.0;

	rng = new LabRng();


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


void BdmLsim4::BeforeStep() {
	UpdateEventsRate(totalrate);
	//tt = GetTime();
}

int cnt = 0;

void BdmLsim4::AfterStep() {
	//tt = GetTime();

	//	if (round(this->GetTime()) == cnt) {
	//		LabOutputerLine::DrawProgressBar(150, this->GetTime() / tt_end, true, true);
	//		++cnt;
	//	}


	//	int empty = _C->CNT(_C->GS("EMPTY"));
	//	int occupied = _C->GS("RESIDENT").CU(_C->GS("MUTANT"));
	//	//int occupied = _C->CNT((_C->GS("RESIDENT") + _C->GS("MUTANT")));
	//
	//	//cout << "OCCUPATION >> " << empty << " | " << occupied  << " ( " << _C->GS("RESIDENT").Cardinality() << " " << _C->GS("MUTANT").Cardinality() << " )" << endl;
	//	cout << "MEAN X     >> " << _C->AVG((_C->GS("RESIDENT") + _C->GS("MUTANT")), LabSiteBase::IND_DEP, BdmLsim4::X) << endl;

}


void BdmLsim4::FireEvent() {

	// Choose and realize an event
	RealizeEvent();

	// Periodically recalculate totalrate
	if (GetNbFiredEvents() % resynch == 0)
		ResynchTotalRate();

	UpdateEventsRate(totalrate);

}

// Use preferably the 'LabOutputer' utility in this hook
void BdmLsim4::OutputSimulation()
{
	//	// Census pop
		Census();
	//
	//	// Periodically dump lattice state
	//	DumpLattice();

	// Occupation

	cout << endl;
	LabOutputerLine::DrawProgressBar(50, this->GetTime() / tt_end, false, true);
	cout << endl;

	LabTimer *t = new LabTimer();
	t->Start();

	int empty = _C->CNT(_C->GS("EMPTY"));
	int occupied = _C->GS("RESIDENT").CU(_C->GS("MUTANT"));
	//int occupied = _C->CNT((_C->GS("RESIDENT") + _C->GS("MUTANT")));

	cout << "OCCUPATION >> " << empty << " | " << occupied  << " ( " << _C->GS("RESIDENT").Cardinality() << " " << _C->GS("MUTANT").Cardinality() << " )" << endl;
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

void BdmLsim4::BeforeRun()
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

void BdmLsim4::AfterRun()
{
	// Get end time
	cout << "\n" << endl;
	timer->Stop();
	cout << "TOTAL SIMULATION TIME :" << endl;
	timer->Print();
	delete timer;
}


void BdmLsim4::BinarizeAll()
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

void BdmLsim4::UnBinarizeAll()
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
void BdmLsim4::Run()
{
	for (int i=0; i<nb_rep; i++)
	{
		// Run a repetition
		RunSimulation();
		UpdateInputs();
	}
}

int main(int argc, char **argv) {

	BdmLsim4 *sim = new BdmLsim4();

	_C = new LabSiteSetsCalculator();

	// Initialize with default hardcoded values
	sim->DefaultInputs();

	// Replace input model configuration by values from CL or XML
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


	// Initialize network

	////sim->SetMaxPopulationSize(nb_sites); // Moved to InitPopulation...

	if (sim->GetDoStartFromDump())
	{
		sim->LoadNetwork(net_dump_filename);
	}
	else
	{
		LabNetwork *net = new LabNetwork();
		net->Populate(nb_sites);

		bool ok = net->RandConnectUserLinear(con_deg);
		cout << "CONNECTION ? " << ok << endl;
		sim->SetNetwork(net);
	}
	cout << "NB NETWORK NODES = " << sim->GetNetwork()->GetSize() << endl;

	//	// Initialize population with empty sites
	//	LabSiteEvent *default_model = new LabSiteEvent(sim);
	//	sim->Populate(default_model, true);

	sim->Run();
}




