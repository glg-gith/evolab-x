/*
 * DrugRes.cpp
 *
 *  Created on: 26 mars 2013
 *      Author: golgauth
 */


/********************************************************************/
/* S-I-S MODEL            					  						*/
/* [BIRTH, DEATH, INFECTION, CLEARANCE, SUPERINFECTION, MUTATION]	*/
/* Sylvain Gandon                             						*/
/********************************************************************/


#include "DrugRes.h"

#include <gsl/gsl_nan.h>

using namespace std;


//**  GLOBALS  **//			Lazy way : should be class static const attributes
string run_id	= "0";
int nb_rep		= 1;
int nb_sites 	= 1000; //100000;//100;//

int tt_out 		= 1000;   			// output par pas de 1
int tt_end 		= 100000;//50;  			// when simulation ends


//double tt;
////double totalrate;
int popsize;           // total pop size

string output_files_prefix;
//string xml_input_filename;		// XML file to be parsed



// Inputs

/******  Individuals  ******/
int E, S, I;												// States amounts
LabSiteBase *E_site, *S_site, *I_UT_site, *I_T_site;		// Individuals models

/*****  General rates  *****/
double theta; 				// Global birth rate
double mu;					// Natural per capita death rate
double tho;					// Probability to be treated
double sigma;				// Superinfection susceptibility
double rp; 					// Recombination probability
double mut;					// Mutation rate

/**  Loci-specific rates  **/
// State matrix
double VTCmtx_input[3][4][2];
double VTCmtx[3][4][2];


// Mean transmission, virulence and clearance
double virMOY, betaMOY, clearMOY;
double virMOY_I, virMOY_T, betaMOY_I, betaMOY_T, clearMOY_I, clearMOY_T;

// Pathogen genotype can be ab=0=>0001, Ab=1=>0010, aB=2=>0100, AB=3=>1000
int ngeno = 4;



/** Miscellaneous **/

// RNG
LabRng *rng;

// Timer
LabTimer *timer;

// CALCULATOR
LabSiteSetsCalculator *_C;


int nb_fired_evts = 0;

int killedI = 0, killedT = 0;
int clearedI = 0, clearedT = 0;


DrugRes::DrugRes() {
	outputer = NULL;
	output_files_prefix = "output";

	eventStack = new LabEventStack();
}

DrugRes::~DrugRes() {
	// TODO Auto-generated destructor stub
}


// RATES CALLBACKS :

double BirthRate(EventCallBackParam param)
{
	return theta;
}

double DeathRate(EventCallBackParam param)
{
	return (mu * _C->GS("S").Cardinality()) 					// Susceptible
		 + (mu + virMOY_I) * (_C->GS("T0").Cardinality()) 		// Infected / not treated
		 + (mu + virMOY_T) * (_C->GS("T1").Cardinality());		// Infected / treated
}

double InfectionRate(EventCallBackParam param)
{
//	return ( betaMOY_I * (_C->GS("T0").Cardinality())
//		   + betaMOY_T * (_C->GS("T1").Cardinality()) ) * _C->GS("S").Cardinality();
	return betaMOY * (_C->GS("I").Cardinality()) * _C->GS("S").Cardinality();
}

double ClearanceRate(EventCallBackParam param)
{
//	clearMOY_I*(y[1]) + clearMOY_T*(y[2]);
	return clearMOY_I * (_C->GS("T0").Cardinality())  +  clearMOY_T * (_C->GS("T1").Cardinality());
}

// TODO : Verify this formula !!!!!
double SuperinfectionRate(EventCallBackParam param)
{
//	sigma*(y[1]+y[2])+betaMOY*(y[1]+y[2]);		// Probably false...
//	sigma*betaMOY*(y[1]+y[2])*(y[1]+y[2]);		// Probably true...
//	return sigma * ( betaMOY_I * (_C->GS("T0").Cardinality())
//				   + betaMOY_T * (_C->GS("T1").Cardinality()) ) * _C->GS("I").Cardinality();
//	return sigma * ( betaMOY * (_C->GS("I").Cardinality()) ) * _C->GS("I").Cardinality();
	return sigma * _C->GS("I").Cardinality() * ( betaMOY * (_C->GS("I").Cardinality()) );
}

double MutationRate(EventCallBackParam param)
{
	//return mu;
	return mut * _C->GS("I").Cardinality();
}


//// REALIZES CALLBACKS :

double RealizeBirth(EventCallBackParam param)
{
//	cout << "FIRE BIRTH !" << endl;

	// <//DRUGRES//>
	// Here we retrieve an individual from the group "EMPTY"
	// The first one found is ok...
	LabSiteBase *site = _C->GI(_C->GS("EMPTY").GetFirst());
	if (site)
	{
		//	double p1 = rng->GetNextDouble();
		//	if (p1 < tho)
		//		site->Copy(S_T_site);
		//	else
		//		site->Copy(S_UT_site);

		site->SetState(DrugRes::SUSCEPTIBLE, 0.0);
		site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED, -1);
		site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::GENO, -1);
		// <//DRUGRES//>
		// The following copy would have been equivalent to the 3 lines above :
		//	site->Copy(S_site);

		popsize++;
	}
}

double RealizeDeath(EventCallBackParam param)
{
//	cout << "FIRE DEATH !" << endl;

	LabSiteBase *site = NULL;
	double p1 = rng->GetNextDouble();
	double fgeno = 0.0;
	int geno = -1;

	int i;
	double max = mu + ((mu + virMOY_I) * _C->GS("T0").Cardinality()) + ((mu + virMOY_T) * _C->GS("T1").Cardinality());

	// Try to kill a S
	if (p1 < mu / max)
	{
		// <//DRUGRES//>
		// Here we retrieve an individual from the group "S"
		// We randomly choose one from the group (uniform algorithm - Mersenne Twister)...
		site = _C->GI(_C->GS("S").ChooseOne(rng));
	}
	else
	{
		fgeno += mu / max;

		// Try to kill a I_UT
		for (i = 0; i < ngeno; i++)
		{
			// Build geno group name
			stringstream ss; ss << "g" << i;

			//fgeno += (vir[i]*gI[i]) / (virMOY_I*(y[1]+y[2]));
			fgeno += ((mu + VTCmtx[DrugRes::alpha][i][0]) * _C->GS("T0").CI(_C->GS(ss.str()))) / max;
			if (p1 < (fgeno)) {
				geno = i;
				// <//DRUGRES//>
				// Here we retrieve an individual from the intersection of groups "T0" and "gi"
				// The first one found is OK because in this particular simulation, we now that
				// all the individuals belonging to this intersection have exactly the same
				// characteristics...
				// Note : GetFirst() can be a lot faster than ChooseOne() ...
				site = _C->GI((_C->GS("T0") * _C->GS(ss.str())).GetFirst());

				if (site) killedI++;

				break;
			}
		}

		if (geno == -1) 			// No match for untreated ones
		{
			// Try to kill a I_T
			for (i = 0; i < ngeno; i++)
			{
				// Build geno group name
				stringstream ss;
				ss << "g" << i;

				//fgeno += (virT[i]*gT[i]) / (virMOY_T*(y[1]+y[2]));
				fgeno += ((mu + VTCmtx[DrugRes::alpha][i][1]) * _C->GS("T1").CI(_C->GS(ss.str()))) / max;
				if (p1 < (fgeno)) {
					geno = i;
					site = _C->GI((_C->GS("T1") * _C->GS(ss.str())).GetFirst());

					if (site) killedT++;

					break;
				}
			}
		}
	}

	// Return to emptiness
	if (site) {
		site->SetState(DrugRes::EMPTY, 0.0);
		site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED, -1);
		site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::GENO, -1);
		// <//DRUGRES//>
		// The following copy would have been equivalent to the 3 lines above :
		//site->Copy(E_site);

		popsize--;
	}
}

double RealizeInfection(EventCallBackParam param)
{
//	cout << "FIRE INFECTION !" << endl;

	double p1 = rng->GetNextDouble();
	double fgeno = 0.0;
	int geno = -1;

	int i;
	double max = (betaMOY * _C->GS("I").Cardinality());

	// Choose infecting strain
	for (i = 0; i < ngeno; i++)
	{
//		fgeno += (beta[i]*gI[i] + betaT[i]*gT[i]) / (betaMOY*(y[1]+y[2]));
//		if (p1 < (fgeno)) { geno = i; break; };

		stringstream ss; ss << "g" << i;
		fgeno += (VTCmtx[DrugRes::beta][i][0] * _C->GS("T0").CI(_C->GS(ss.str())) +
				  VTCmtx[DrugRes::beta][i][1] * _C->GS("T1").CI(_C->GS(ss.str())) ) / max;
		if (p1 < (fgeno)) { geno = i; break; };
	}

	// Goto infectious state
	LabSiteBase *site = _C->GI(_C->GS("S").ChooseOne(rng));
	if (site) {
		site->SetState(DrugRes::INFECTED, 0.0);

		//		if (RND(site->GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED)) == 0) {	// Not treated
//			site->Copy(I_UT_site);
//		} else {
//			site->Copy(I_T_site);
//		}

//		if (RND(site->GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED)) == -1)		// ????
//		{
			// Choose to treat or not
			double p1 = rng->GetNextDouble();
			if (p1 < tho)
				//site->Copy(I_T_site);		// Treated
				site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED, 1);
			else
				//site->Copy(I_UT_site);		// Not treated
				site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED, 0);
//		} else {
//			if (RND(site->GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED)) == 1) // ????
//				site->Copy(I_T_site);		// Treated
//			else
//				site->Copy(I_UT_site);		// Not treated
//		}

		site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::GENO, geno);
	}
}

double RealizeClearance(EventCallBackParam param)
{
//	cout << "FIRE CLEARANCE !" << endl;

	LabSiteBase *site = NULL;

	double p1 = rng->GetNextDouble();
	double fgeno = 0.0;
	int geno = -1;

	int i;
	double max = (clearMOY_I * _C->GS("T0").Cardinality()) + (clearMOY_T * _C->GS("T1").Cardinality());

	// Try to clear a I_UT
	for (i = 0; i < ngeno; i++)
	{
		// Build geno group name
		stringstream ss; ss << "g" << i;

		//fgeno += (clear[i]*gI[i]) / (clearMOY_I*(y[1]));
		fgeno += (VTCmtx[DrugRes::gamma][i][0] * _C->GS("T0").CI(_C->GS(ss.str()))) / max;
		if (p1 < (fgeno)) {
			geno = i;
			site = _C->GI((_C->GS("T0") * _C->GS(ss.str())).GetFirst());
			if (site) clearedI++;
			break;
		}
	}

	if (geno == -1) 			// No match for untreated ones
	{
		// Try to clear a I_T
		for (i = 0; i < ngeno; i++)
		{
			// Build geno group name
			stringstream ss; ss << "g" << i;

			//fgeno += (clearT[i]*gT[i]) / (clearMOY_T*(y[2]));
			fgeno += (VTCmtx[DrugRes::gamma][i][1] * _C->GS("T1").CI(_C->GS(ss.str()))) / max;
			if (p1 < (fgeno)) {
				geno = i;
				site = _C->GI((_C->GS("T1") * _C->GS(ss.str())).GetFirst());
				if (site) clearedT++;
				break;
			}
		}
	}

	// Return to susceptible state
	if (site) {
		site->SetState(DrugRes::SUSCEPTIBLE, 0.0);
		site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED, -1);
		site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::GENO, -1);
		//site->Copy(S_site); // would have been equivalent...
	}
}

// Full mask = 1111 = 2^3+2^2+2^1+2^0 = 8+4+2+1 = 15
// Ex : g=1 => 0010 = 2^1 = 2
int g_to_mask(int g)
{
	return 1 << g;
}

double RealizeSuperinfection(EventCallBackParam param)
{
//	cout << "FIRE SUPERINFECTION !" << endl;

	LabSiteBase *site = NULL;

	double p1 = rng->GetNextDouble();
	double fgeno = 0.0;
	int i;

	int g1, g2;
	// Choose the resident strain
	site = _C->GI(_C->GS("I").ChooseOne(rng));
	if (site) 									// TODO : else this is EXTINCTION and should be checked in the main loop
	{

		////g1 = RND(site->GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::GENO));
		g1 = site->GetAttrInt(LabSiteBase::IND_DEP, DrugRes::GENO);

		// Choose the superinfecting strain (superinfection is beta-dependent)
		// TODO : ici il faudrait faire -1 sur le group (gI ou gT) quand c'est le group de l'individu choisi ci-dessus
		//        car un individu ne peut pas se superinfecter lui-meme... ???
		for (i = 0; i < ngeno; i++)
		{
	//		fgeno += (beta[i]*gI[i] + betaT[i]*gT[i]) / (betaMOY*(y[1]+y[2]));
	//		if (p1 < (fgeno)) { geno = i; break; };

			stringstream ss; ss << "g" << i;
			fgeno += (VTCmtx[DrugRes::beta][i][0] * _C->GS("T0").CI(_C->GS(ss.str())) +
					  VTCmtx[DrugRes::beta][i][1] * _C->GS("T1").CI(_C->GS(ss.str())) ) / (betaMOY * _C->GS("I").Cardinality());
			if (p1 < (fgeno)) { g2 = i; break; };
		}


		if (g1 != g2)
		{
			// Combine g1 and g2
			// Ex : g1=1=0010 and g2=3=1000 => c=1010=2^3+2^1=10
			int c = g_to_mask(g1) | g_to_mask(g2);
			int geno = 0;

			// Choose the winner strain
			double p2 = rng->GetNextDouble();
			double rp2;
			if (c == 3 || c == 5 || c == 10 || c == 12)				// c=0011=3 or c=0101=5 or c=1010=10 or c=1100=12
			{
				if (p2 < 0.5) geno = g2;
			}
			else
			{
				if (c == 9 || c == 6)								// c=1001=9 or c=0110=6
				{
					if (c == 6) rp2 = rp; else rp2 = (1-rp);		// Switch rp for the extremes (c=1001)
					if (p2 < rp2/2.0) 						geno = 0;
					else if (p2 < rp2/2.0 + (1-rp2)/2.0) 	geno = 1;
					else if (p2 < rp2/2.0 + (1-rp2)) 		geno = 2;
					else 						 			geno = 3;
				}
			}

			// Ancient and new strains are different : do replacement
			if (g1 != geno && site)
			{
				site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::GENO, geno);
			}
		}

	}
}

double RealizeMutation(EventCallBackParam param)
{
//	cout << "FIRE MUTATION !" << endl;

	LabSiteBase *site = NULL;

	double p1 = rng->GetNextDouble();

	int g1 = 0, g2 = 0;
	// Choose the resident strain
	site = _C->GI(_C->GS("I").ChooseOne(rng));
	if (site) 									// TODO : else this is EXTINCTION and should be checked in the main loop
	{

		g1 = site->GetAttrInt(LabSiteBase::IND_DEP, DrugRes::GENO);

		if (g1 == 0 || g1 == 3)			// Extremes : choose one among the middles
		{
			if (p1 < 0.5) g2 = 1;
			else g2 = 2;
		}
		else if (g1 == 1 || g1 == 2)	// Middles : choose one among the extremes
		{
			if (p1 < 0.5) g2 = 0;
			else g2 = 3;
		}

		// Ancient and new strains are different : do replacement
		if (g1 != g2)
		{
			site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::GENO, g2);
		}

	}
}



/*
 * BdmLsim3 CLASS dedicated stuffs
 */


void set_state_matrix(double mtx[3][4][2])
{

	// Virulence				[ex : treatment lowers virulence]
	// Not-Treated / vir[Iab, IAb, IaB, IAB]
	VTCmtx[0][0][0] = mtx[0][0][0];
	VTCmtx[0][1][0] = mtx[0][0][0] + mtx[0][1][0];
	VTCmtx[0][2][0] = mtx[0][0][0] + mtx[0][2][0];
	VTCmtx[0][3][0] = mtx[0][0][0] + mtx[0][1][0] + mtx[0][2][0] + mtx[0][3][0];
	// Treated / virT[Tab, TAb, TaB, TAB]
	VTCmtx[0][0][1] = mtx[0][0][1];
	VTCmtx[0][1][1] = mtx[0][0][1] + mtx[0][1][1];
	VTCmtx[0][2][1] = mtx[0][0][1] + mtx[0][2][1];
	VTCmtx[0][3][1] = mtx[0][0][1] + mtx[0][1][1] + mtx[0][2][1] + mtx[0][3][1];

	// Transmission				[ex : treatment lowers transmission]
	// Not-Treated / beta[Iab, IAb, IaB, IAB]
	VTCmtx[1][0][0] = mtx[1][0][0];
	VTCmtx[1][1][0] = mtx[1][0][0] + mtx[1][1][0];
	VTCmtx[1][2][0] = mtx[1][0][0] + mtx[1][2][0];
	VTCmtx[1][3][0] = mtx[1][0][0] + mtx[1][1][0] + mtx[1][2][0] + mtx[1][3][0];
	// Treated / betaT[Tab, TAb, TaB, TAB]
	VTCmtx[1][0][1] = mtx[1][0][1];
	VTCmtx[1][1][1] = mtx[1][0][1] + mtx[1][1][1];
	VTCmtx[1][2][1] = mtx[1][0][1] + mtx[1][2][1];
	VTCmtx[1][3][1] = mtx[1][0][1] + mtx[1][1][1] + mtx[1][2][1] + mtx[1][3][1];

	// Clearance				[ex : treatment uppers clearance]
	// Not-Treated / clear[Iab, IAb, IaB, IAB]
	VTCmtx[2][0][0] = mtx[2][0][0];
	VTCmtx[2][1][0] = mtx[2][0][0] + mtx[2][1][0];
	VTCmtx[2][2][0] = mtx[2][0][0] + mtx[2][2][0];
	VTCmtx[2][3][0] = mtx[2][0][0] + mtx[2][1][0] + mtx[2][2][0] + mtx[2][3][0];
	// Treated / clearT[Tab, TAb, TaB, TAB]
	VTCmtx[2][0][1] = mtx[2][0][1];
	VTCmtx[2][1][1] = mtx[2][0][1] + mtx[2][1][1];
	VTCmtx[2][2][1] = mtx[2][0][1] + mtx[2][2][1];
	VTCmtx[2][3][1] = mtx[2][0][1] + mtx[2][1][1] + mtx[2][2][1] + mtx[2][3][1];

	//for(int i=0;i<ngeno;i++) vir[i]=(virmin+(virmax-virmin)*i/(ngeno-1));
	//for(int i=0;i<ngeno;i++) {beta[i]=((b0*(pow((vir[i]),coef)))/N);};	//// TRADEOFF SHAPE

}


void DrugRes::DefaultInputs()
{
	// Individuals
	S = 270;			// TODO : Replace it by endemic equilibrium law
	I = 100;			/// 80
	E = nb_sites - (S + I);

	////T = 20;				/// 20


	// General rates
	theta = 10.0;
	mu = .01;///.01;
	tho = 0.8;///0.8;
	sigma = 1;
	rp  = 0.2;
	mut = 0.01;///0.001;

	// Loci-specific rates
	// Default state matrix
	/*
	 * 							  Default     A effect     B effect     Epistasis
	 * 						 	 ___________________________________________________
	 * 	Virulence 		[I,T]	| [a, aT]     [daA, daAT]  [daB, daBT]  [dae, daeT]
	 * 	Transmission	[I,T]	| [b, bT]     [dbA, dbAT]  [dbB, dbBT]  [dbe, dbeT]
	 * 	Clearance		[I,T]	| [c, cT]     [dcA, dcAT]  [dcB, dcBT]  [dce, dceT]
	 *
	 */
	 //-b [[.00007,.00003],[-.000001,.000001],[-.000001,.000001],[0.0,0.0]]
	double dmatrix[3][4][2] = {
			{ {0.005, 	0.005}, 		{0.00, 		0.00}, 		{0.00, 		0.00}, 		{0.0,	0.0} },
			{ {.0007, 	.0003}, 		{.0001, 	.0002}, 	{.0001, 	.0002}, 	{0.0,	0.0} },
			////{ {.00007, 	.00003}, 		{-.00001, 	.00002}, 	{-.00001, 	.00002}, 	{0.0,	0.0} },
			//{ {.000003, 	.000002}, 		{-.0000001, 	.0000001}, 	{-.0000001, 	.0000001}, 	{0.0,	0.0} },
			///{ {mu + .01, 	mu + .01}, 	{0, 		0.00}, 		{0, 		0.00}, 		{0.0,	0.0} }
			{ {0.005, 	0.005}, 	{0, 		0.00}, 		{0, 		0.00}, 		{0.0,	0.0} }
	};
//	double dmatrix[3][4][2] = {
//			{ {0.005, 	0.005}, 		{0.00, 		0.00}, 		{0.00, 		0.00}, 		{0.0,	0.0} },
//			{ {.0007, 	.0003}, 		{-.0001, 	.0002}, 	{-.0001, 	.0002}, 	{0.001,	0.0} },
//			//{ {.000003, 	.000002}, 		{-.0000001, 	.0000001}, 	{-.0000001, 	.0000001}, 	{0.0,	0.0} },
//			///{ {mu + .01, 	mu + .01}, 	{0, 		0.00}, 		{0, 		0.00}, 		{0.0,	0.0} }
//			{ {0.005, 	0.005}, 	{0, 		0.00}, 		{0, 		0.00}, 		{0.0,	0.0} }
//	};
	memcpy(VTCmtx_input, dmatrix, sizeof(dmatrix));

	///////
	set_state_matrix(VTCmtx_input);
	///////

//	net_dump_filename = "bdmLsim.res.lat." + run_id;
	SetNbRepetitions(nb_rep);

    SetTimeDumpStep(1);
    SetDoDump(false);
    SetDoStartFromDump(false);
}


//string DrugRes::ParseCL(int argc, char **argv)
//{
//	// If XML input was set : skip CL parsing
//	for (int i=0; i<argc; i++)
//		if (argc-1 > i && strcmp(argv[i], "-i") == 0) return argv[i+1];
//
//	// Wrap everything in a try block.  Do this every time,
//	// because exceptions will be thrown for problems.
//	try {
//
//		// Define the command line object.
//		CmdLine cmd("bdmLsim simulation (using EVOLAB-X1 project)", ' ', "0.4");
//
//		// Define a value argument and add it to the command line.
//
//		// TODO : implement inf and sup limits for all the parameters
//		MultiArg<double> dr_arg("d","death-rate", "Constant individual death factor.", true, "double");
//		//ValueArg<double> dr_arg("d","death-rate", "Constant individual death factor.", true, 0.01, "double");
//		cmd.add( dr_arg );
//
//		ValueArg<double> br_arg("b","birth-rate", "Constant individual birth factor.", true, 0.01, "double");
//		cmd.add( br_arg );
//
//		ValueArg<double> mu_arg("m","mut-rate", "Constant individual mutation factor.", true, 0.01, "double");
//		cmd.add( mu_arg );
//
//		ValueArg<double> mo_arg("M","mov-rate", "Constant individual movement factor.", true, 0.01, "double");
//		cmd.add( mo_arg );
//
//		ValueArg<double> x1_arg("x","x1-trait", "Constant value for resident's trait.", true, 0.01, "double");
//		cmd.add( x1_arg );
//
//		ValueArg<double> x2_arg("X","x2-trait", "Constant value for mutant's trait.", true, 0.02, "double");
//		cmd.add( x2_arg );
//
//		ValueArg<double> Lr_arg("c","child-rate", "Constant individual reproduction factor.", true, 0.01, "double");
//		cmd.add( Lr_arg );
//
//		ValueArg<string> ri_arg("r","run-id", "String identifier for this whole simulation.", true, "run-0", "string");
//		cmd.add( ri_arg );
//
//		ValueArg<int> nr_arg("n","nb-rep", "Number of repetitions.", true, 1, "int");
//		cmd.add( nr_arg );
//
//
//		ValueArg<double> ds_arg("s","dump-step", "Dump simulation state every 'dump-step' seconds. \n\
//     Requires '-D' option to be enabled.", false, 0.01, "double");
//		cmd.add( ds_arg );
//
//		ValueArg<string> xi_arg("i","xml-input", "XML model input file.", false, "model.xml", "string");
//		cmd.add( xi_arg );
//
//		ValueArg<string> o_arg("o","out-prefix", "Text for prefixing the output filenames.", false, "output", "string");
//		cmd.add( o_arg );
//
//		// Flags
//		SwitchArg dd_switch("D", "do-dump", "Dump / backup point every 'dump-step' seconds.", false);
//		cmd.add( dd_switch );
//		SwitchArg r_switch("R", "recovery", "Starts the simulation from the last backup point.", false);
//		cmd.add( r_switch );
//
//		// Parse the args.
//		cmd.parse( argc, argv );
//
//		vector<double> vec;
//
//		nb_rep = nr_arg.getValue();
//
//		vec = dr_arg.getValue();
//		// TODO : do the same for all other parameters
//		if (vec.size() == 1) { d = vec[0]; }
//		else { d = min(vec[0], vec[1]); d2 = max(vec[0], vec[1]); d_s = (d2-d)/(double)(nb_rep-1); }
//
//		b = br_arg.getValue();
//		mu = mu_arg.getValue();
//		mo = mo_arg.getValue();
//		x1 = x1_arg.getValue();
//		x2 = x2_arg.getValue();
//		L = Lr_arg.getValue();
//
//		run_id = ri_arg.getValue();
//
//		this->SetTimeDumpStep(ds_arg.getValue());
//		this->SetDoDump(dd_switch.getValue());
//		this->SetDoStartFromDump(r_switch.getValue());
//
//		output_files_prefix = o_arg.getValue();
//		xml_input_filename = xi_arg.getValue();
//
//		// Set net dump filename
//		stringstream ss;
//		ss << output_files_prefix << ".lat." << run_id; // << "." << GetCurrentRepetition();
//		net_dump_filename = ss.str();
//		SetNbRepetitions(nb_rep);
//
//
//	}
//	catch (ArgException &e)  // catch any exceptions
//	{
//		cerr << "Error: " << e.error() << " for arg " << e.argId() << endl;
//	}
//
//	return "";
//}
//
//void DrugRes::ParseXML(string filename)
//{
//	xmlDocPtr doc;
//	xmlNodePtr root, consts;
//	vector<xmlNodePtr> cst_vec;
//	ostringstream ss;
//	istringstream iss, iss2;
//	string str;
//	xmlChar *str2;
//	bool has_value2;
//
//	// Parse the args.
//	doc = xmlParseFile(filename.c_str());
//	if (doc == NULL) {
//		cerr << "Invalide XML input document !" << endl;
//		return exit(-1);
//	}
//	root = xmlDocGetRootElement(doc);
//	if (root == NULL) {
//		cerr << "Empty XML document !" << endl;
//		xmlFreeDoc(doc);
//		return exit(-1);
//	}
//
//	// Read constants
//	consts = LabXMLUtils::GetChildByName(root, "Constants");
//	if (consts == NULL)
//	{
//		cerr << "Missing 'Constants' node !" << endl;
//		exit(-1);
//	}
//
//	cst_vec = LabXMLUtils::GetChildrenByName(consts, "Const");
//
//	for (unsigned int i=0; i< cst_vec.size(); i++)
//	{
//		ss.str("");
//		iss.str("");
//		iss2.str("");
//		has_value2 = false;
//		ss << xmlGetProp(cst_vec[i], (const xmlChar *)"name");
//		str = ss.str();
//		iss.str((char *)xmlGetProp(cst_vec[i], (const xmlChar *)"value"));
//		LabXMLUtils::StrToLower(str);
//
//		// Test if range value
//		str2 = xmlGetProp(cst_vec[i], (const xmlChar *)"value2");
//		if (str2 != NULL)
//		{
//			iss2.str(""); iss2.str((char *)str2);
//			has_value2 = true;
//		}
//
//		if 		(str == "death-rate")
//		{
//			iss >> d;
//			// TODO : do this additional stuff for all the other params.
//			if (has_value2) { iss2 >> d2; d_s = (d2-d)/(double)(nb_rep-1); }
//		}
//		else if (str == "birth-rate")
//			iss >> b;
//		else if (str == "mut-rate")
//			iss >> mu;
//		else if (str == "mov-rate")
//			iss >> mo;
//		else if (str == "x1-rate")
//			iss >> x1;
//		else if (str == "x2-rate")
//			iss >> x2;
//		else if (str == "l-rate")
//			iss >> L;
//		else if (str == "run-id")
//			run_id = iss.str();
//		else if (str == "nb-rep")
//			iss >> nb_rep;
//		else if (str == "out-prefix")
//			output_files_prefix = iss.str();
//		else if (str == "dump-step") {
//			double t_dump_step;
//			iss >> t_dump_step;
//			this->SetTimeDumpStep(t_dump_step);
//		}
//		else if (str == "do-dump")
//			this->SetDoDump((str == "True"));
//		else if (str == "recovery")
//			this->SetDoStartFromDump((str == "True"));
//	}
//
//
//	// Set net dump filename
//	ss.str("");
//	ss << output_files_prefix << ".lat." << run_id; // << "." << GetCurrentRepetition();
//	net_dump_filename = ss.str();
//
//	SetNbRepetitions(nb_rep);
//
//	xmlFreeDoc(doc);
//
//}




/**
 * Describes the default models and store it for later Populate() and Copy() calls...
 */

// <//DRUGRES//>
void DrugRes::DefaultModels()
{
	/** Create 1 model site for E : empty site **/
	E_site = new LabSiteBase(this);

	// ATTRIBUTES
	// Those attributes are INDEXED ones : they generate the "sets" (T0, T1) and (g0, g1, g2, g3)
	// Those sets (groups) can then be accessed using "_C->GS("a_group_name")" ("GS" is short for "GetSet")
	//
	E_site->PushAttribute("T", LabSiteBase::IND_DEP, 0, true, 2);	// Treated or not [0 : false, 1 : true]
	E_site->PushAttribute("g", LabSiteBase::IND_DEP, 0, true, 4);	// 4 possible pathogen genotypes
	// Those attributes are regular ones : they have nothing to do with "sets"
//	E_site->PushAttribute("a", LabSiteBase::IND_DEP);				// Virulence
//	E_site->PushAttribute("b", LabSiteBase::IND_DEP);				// Transmission
//	E_site->PushAttribute("c", LabSiteBase::IND_DEP);				// Clearance

	// STATES
	// Those states generate the following "sets" (EMPTY, S, I)
	//
	E_site->PushState("EMPTY", 1, -1, -1);
	E_site->PushState("S", 	   1, -1, -1);
	E_site->PushState("I", 	   1, -1, -1);

	E_site->SetState(DrugRes::EMPTY, GetTime());
	E_site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED, -1);
//	E_site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::a, 0.0);
//	E_site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::b, 0.0);
//	E_site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::c, 0.0);
	E_site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::GENO, -1);


	/** Create 1 models sites for S : susceptible sites - non-treated by default **/
	S_site = new LabSiteBase(this);
	S_site->Copy(E_site, LabSiteBase::IND_DEP);
	S_site->SetState(DrugRes::SUSCEPTIBLE, GetTime());


	/** Create 2 models sites for I : 1 for Untreated and 1 for Treated **/
	// I-Untreated
	I_UT_site = new LabSiteBase(this);
	I_UT_site->Copy(E_site, LabSiteBase::IND_DEP);
	I_UT_site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED, 0);
	I_UT_site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::GENO, 0);
//	I_UT_site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::a, VTCmtx[DrugRes::alpha]
//	                                                                 [RND(I_UT_site->GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::GENO))]
//	                                                                 [RND(I_UT_site->GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED))]
//	                                                              	 );
//	I_UT_site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::b, VTCmtx[DrugRes::beta]
//	                                                                 [RND(I_UT_site->GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::GENO))]
//	                                                                 [RND(I_UT_site->GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED))]
//																  	 );
//	I_UT_site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::c, VTCmtx[DrugRes::gamma]
//	                                                                 [RND(I_UT_site->GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::GENO))]
//	                                                                 [RND(I_UT_site->GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED))]
//																  	 );
	I_UT_site->SetState(DrugRes::INFECTED, GetTime());

	// I-Treated
	I_T_site = new LabSiteBase(this);
	I_T_site->Copy(I_UT_site, LabSiteBase::IND_DEP);
	I_T_site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED, 1);
//	I_T_site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::a, VTCmtx[DrugRes::alpha]
//	                                                                 [RND(I_T_site->GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::GENO))]
//	                                                                 [RND(I_T_site->GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED))]
//	                                                              	 );
//	I_T_site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::b, VTCmtx[DrugRes::beta]
//	                                                                 [RND(I_T_site->GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::GENO))]
//	                                                                 [RND(I_T_site->GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED))]
//																  	 );
//	I_T_site->SetAttrDouble(LabSiteBase::IND_DEP, DrugRes::c, VTCmtx[DrugRes::gamma]
//	                                                                 [RND(I_T_site->GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::GENO))]
//	                                                                 [RND(I_T_site->GetAttrDouble(LabSiteBase::IND_DEP, DrugRes::TREATED))]
//																  	 );

}

void DrugRes::InitPopulation()
{
	cout << "Initializing population..." << endl;

	/** Create 5 stating models **/
	vector<LabSiteBase *> model_sites;
	model_sites.push_back(E_site);
	model_sites.push_back(S_site);
	model_sites.push_back(I_UT_site);
	model_sites.push_back(I_T_site);


	// Populate with our 4 starting models
	int I_T = round(I * tho);
	int I_UT = I - I_T;

	popsize = S + I;

	// <\\DRUGRES\\>
	/** Two ways of populating : you can switch between them **/
	/**      Simply uncomment one and comment the other      **/

	// Easy populate from a distribution and a list of models
	// This is stochastic !
	//	// Cumsum of nomes must be 1.0 !!
	//	double d[] = {0.5, 0.5};
	//	vector<double> dist(d, d + sizeof(d) / sizeof(double) );
	//	this->Populate(dist, model_sites, true);

	// Easy populate the network from fixed quantities and a list of models
	// Cumsum of nomes must be equal to the maximum pop size !!
	int d[] = {E, S, I_UT, I_T};
	vector<int> dist(d, d + sizeof(d) / sizeof(int) );
	this->Populate(nb_sites, dist, model_sites, true);


	//cout << endl << "Initializing population... Done !" << endl;


	int universe = _C->CNT(_C->GS("U"));
	int empty = _C->CNT(_C->GS("EMPTY"));
	int full1 = _C->CNT(_C->GS("I") + _C->GS("S"));
	int full2 = _C->CNT(_C->GS("T0") + _C->GS("T1"));
	int occupied = _C->CNT(_C->GS("U") - _C->GS("EMPTY"));
	int treated = _C->CNT(_C->GS("T1"));

	cout << "OCCUPATION AT START >> " << universe << " | " << empty << " | " << occupied << " | " << treated << endl;
	cout << "OCCUPATION AT START >> " << full1 << " | " << full2 << endl;


	Update_F_Averages();
}

/*
 * // <//DRUGRES//>
 * Update the propensity averages.
 * In this demo, we are only interested in quantities (MASS ACTION).
 */
void DrugRes::Update_F_Averages()
{
	double nb_infect = (double)(_C->GS("I").Cardinality());
	double nb_I = (double)(_C->GS("T0").Cardinality());
	double nb_T = (double)(_C->GS("T1").Cardinality());

	virMOY = 0.0;
	virMOY_I = 0.0;
	virMOY_T = 0.0;
	betaMOY = 0.0;
	betaMOY_I = 0.0;
	betaMOY_T = 0.0;
	clearMOY = 0.0;
	clearMOY_I = 0.0;
	clearMOY_T = 0.0;

	for(int i = 0; i < ngeno; i++)
	{
		stringstream ss; ss << "g" << i;

		int I_UT = _C->GS("T0").CI(_C->GS(ss.str()));
		int I_T  = _C->GS("T1").CI(_C->GS(ss.str()));

		if (nb_infect != 0)
		{
//			virMOY += (vir[i]*gI[i] + virT[i]*gT[i]) / nb_infect;
//			betaMOY += (beta[i]*gI[i] + betaT[i]*gT[i]) / nb_infect;
//			clearMOY += (clear[i]*gI[i] + clearT[i]*gT[i]) / nb_infect;
			virMOY   += ( VTCmtx[DrugRes::alpha][i][0] * I_UT + VTCmtx[DrugRes::alpha][i][1] * I_T )  /  nb_infect;
			betaMOY  += ( VTCmtx[DrugRes::beta][i][0]  * I_UT + VTCmtx[DrugRes::beta][i][1] * I_T )   /  nb_infect;
			clearMOY += ( VTCmtx[DrugRes::gamma][i][0] * I_UT + VTCmtx[DrugRes::gamma][i][1] * I_T )  /  nb_infect;
		}
		if (nb_I != 0)
		{
//			virMOY_I += (vir[i]*gI[i]) / nb_I;
//			clearMOY_I += (clear[i]*gI[i]) / nb_I;
			virMOY_I   += (VTCmtx[DrugRes::alpha][i][0] * I_UT) / nb_I;
			betaMOY_I  += (VTCmtx[DrugRes::beta][i][0]  * I_UT) / nb_I;
			clearMOY_I += (VTCmtx[DrugRes::gamma][i][0] * I_UT) / nb_I;
		}
		if (nb_T != 0)
		{
//			virMOY_T += (virT[i]*gT[i]) / nb_T;
//			clearMOY_T += (clearT[i]*gT[i]) / nb_T;
			virMOY_T   += (VTCmtx[DrugRes::alpha][i][1] * I_T) / nb_T;
			betaMOY_T  += (VTCmtx[DrugRes::beta][i][1]  * I_T) / nb_T;
			clearMOY_T += (VTCmtx[DrugRes::gamma][i][1] * I_T) / nb_T;
		}
	}
}


//void DrugRes::CheckInputs()
//{
//	bool ok = true;
//
//	if (d  < 0.0 || d  > 1.0) ok = false;
//	if (b  < 0.0 || b  > 1.0) ok = false;
//	if (mu < 0.0 || mu > 1.0) ok = false;
//	if (mo < 0.0 || mo > 1.0) ok = false;
//	if (x1 < 0.0 || x1 > 1.0) ok = false;
//	if (x2 < 0.0 || x2 > 1.0) ok = false;
//	if (L  < 0.0 || L  > 1.0) ok = false;
//
//	if (!ok)
//	{
//		cerr << "The given d, b, mu, mo, x1, x2 or L value is not in range [0.0, 1.0]" << endl;
//		exit(-1);
//	}
//}

void DrugRes::PrintInputs()
{
	cout << "-------------------------------------" << endl;
	cout << "Input params for repetition '" << GetCurrentRepetition() << "' :" << endl;
	cout << "-------------------------------------" << endl;
	cout << "S  = " << S  << endl;			// TODO : Replace it by endemic equilibrium law
	cout << "I  = " << I  << endl;			/// 80 + 20

	// General rates
	cout << "theta  = " << theta  << endl;
	cout << "mu     = " << mu  << endl;
	cout << "tho    = " << tho  << endl;
	cout << "sigma  = " << sigma  << endl;
	cout << "rp     = " << rp  << endl;
	cout << "mut    = " << mut  << endl;
	cout << "-------------------------------------" << endl;
}

//void DrugRes::UpdateInputs()
//{
//	if (d2  != GSL_NAN) d  += d_s;
//	if (b2  != GSL_NAN) b  += b_s;
//	if (mu2 != GSL_NAN) mu += mu_s;
//	if (mo2 != GSL_NAN) mo += mo_s;
//	if (x12 != GSL_NAN) x1 += x1_s;
//	if (x22 != GSL_NAN) x2 += x2_s;
//	if (L2  != GSL_NAN) L  += L_s;
//}
//

void DrugRes::Census()
{
	// Write a data line
	vector<double> line;
	double tt = GetTime();

	if (tt == 0) outputer->InsertText("t    S    I_UT    I_T    I     (g0, g1, g2, g3)\n");

	line.push_back(tt);
	line.push_back(nb_fired_evts);

	line.push_back(_C->GS("S").Cardinality());
	line.push_back(_C->GS("T0").Cardinality());
	line.push_back(_C->GS("T1").Cardinality());
	line.push_back(_C->GS("I").Cardinality());
	outputer->AddDataLine(line);
	outputer->WriteData();
	outputer->ResetDataSet();

	stringstream ss;
	ss << "( " 	<< _C->GS("g0").Cardinality() << ", "
				<< _C->GS("g1").Cardinality() << ", "
				<< _C->GS("g2").Cardinality() << ", "
				<< _C->GS("g3").Cardinality() << " )\n";
	ss << "K_UT/C_UT[" << killedI << ", " << clearedI << "]" << "    K_T/C_T[" << killedT << ", " << clearedT << "]\n";
	outputer->InsertText(ss.str());
}


void DrugRes::RealizeEvent()
{

	// <//DRUGRES//>
	// Fire an event from the eventStack here !!!
	// In this demo, firing events is managed at the simulation level (attribute "eventStack")
	// (in "BdmLsimX", events occurred at the site level).

	double p1 = rng->GetNextDouble();
	eventStack->RealizeEvent(p1, GetTime());
	eventStack->UpdateStackRates();

	// <//DRUGRES//>
	Update_F_Averages();

	nb_fired_evts++;
}


void DrugRes::SetOutputFilenames()
{
	stringstream ss;
	ss << output_files_prefix << "." << run_id << "." << GetCurrentRepetition();
	outputFilename = ss.str();

	if (outputer  != NULL) outputer->SetFilename(outputFilename, !this->GetDoStartFromDump());
}

/*
 * Virtual stuffs implementation
 */

void DrugRes::InitSimulation()
{

	rng = new LabRng();

	InitPopulation();		// IMPORTANT !!

	outputer = NULL;

	if (outputer == NULL) {
		outputer = new LabOutputerLine();
		outputer->SetOuputMode(LabOutputerLine::TO_BOTH);
		outputer->SetSeparator("\t");
//		outputer->SetFilename(outputFilename);
	}

	SetOutputFilenames();


	// <//DRUGRES//>
	// Fill the global and unique event stack with all possible events
	// Possible events [BIRTH, DEATH, INFECTION, CLEARANCE, SUPERINFECTION, MUTATION]

	RateCallBack *rate_callback;
	RealizeCallBack *realize_callback;

	FBirthRate = &BirthRate;
	FRealizeBirth = &RealizeBirth;

	FDeathRate = &DeathRate;
	FRealizeDeath = &RealizeDeath;

	FInfectionRate = &InfectionRate;
	FRealizeInfection = &RealizeInfection;

	FClearanceRate = &ClearanceRate;
	FRealizeClearance = &RealizeClearance;

	FSuperinfectionRate = &SuperinfectionRate;
	FRealizeSuperinfection = &RealizeSuperinfection;

	FMutationRate = &MutationRate;
	FRealizeMutation = &RealizeMutation;


	rate_callback = new RateCallBack(FBirthRate);
	realize_callback = new RealizeCallBack(FRealizeBirth);
	eventStack->PushEvent("BIRTH", 0, NULL, rate_callback, realize_callback);


	rate_callback = new RateCallBack(FDeathRate);
	realize_callback = new RealizeCallBack(FRealizeDeath);
	eventStack->PushEvent("DEATH", 0, NULL, rate_callback, realize_callback);


	rate_callback = new RateCallBack(FInfectionRate);
	realize_callback = new RealizeCallBack(FRealizeInfection);
	eventStack->PushEvent("INFECTION", 0, NULL, rate_callback, realize_callback);


	rate_callback = new RateCallBack(FClearanceRate);
	realize_callback = new RealizeCallBack(FRealizeClearance);
	eventStack->PushEvent("CLEARANCE", 0, NULL, rate_callback, realize_callback);


	rate_callback = new RateCallBack(FSuperinfectionRate);
	realize_callback = new RealizeCallBack(FRealizeSuperinfection);
	eventStack->PushEvent("SUPERINFECTION", 0, NULL, rate_callback, realize_callback);


	rate_callback = new RateCallBack(FMutationRate);
	realize_callback = new RealizeCallBack(FRealizeMutation);
	eventStack->PushEvent("MUTATION", 0, NULL, rate_callback, realize_callback);



	// <//DRUGRES//>
	// Update the events rates of the global and unique event stack
	eventStack->UpdateStackRates();
	// Necessary for updating the continuous time (-log(p1) / eventStack->GetStackRate())
	// See : LabSimulatorTime.StepTime()
	UpdateEventsRate(eventStack->GetStackRate());

	SetEndTime(tt_end);
	SetOutputTimeStep(tt_out);

}


void DrugRes::BeforeStep() {

	////UpdateEventsRate(totalrate);
	UpdateEventsRate(eventStack->GetStackRate());

}

void DrugRes::AfterStep() {

}


void DrugRes::FireEvent() {

	// Chooses and realize an event
	RealizeEvent();

	////UpdateEventsRate(totalrate);
	UpdateEventsRate(eventStack->GetStackRate());

}

// Use preferably the 'LabOutputer' utility in this hook
void DrugRes::OutputSimulation()
{
	// Census pop
	Census();
}

void DrugRes::BeforeRun()
{
	// Get begin time
	timer = new LabTimer();
	timer->Start();

	// Print input params
	PrintInputs();

}

void DrugRes::AfterRun()
{
	// Get end time
	cout << "\n" << endl;
	timer->Stop();
    cout << "TOTAL SIMULATION TIME :" << endl;
	timer->Print();
	delete timer;
}


//void DrugRes::BinarizeAll()
//{
//}
//
//void DrugRes::UnBinarizeAll()
//{
//}
//


// Override simulation's default run behavior
//void DrugRes::Run()
//{
//	for (int i=0; i<nb_rep; i++)
//	{
//		// Run a repetition
//		RunSimulation();
//	}
//}

int main(int argc, char **argv) {

	DrugRes *sim = new DrugRes();

	_C = new LabSiteSetsCalculator();

	sim->SetMaxPopulationSize(nb_sites);

	// Initialize with default hardcoded values
	sim->DefaultInputs();

	// Initialize individuals models
	sim->DefaultModels();

	// Replace input model configuration by values from CL or XML
//	xml_input_filename = sim->ParseCL(argc, argv);
//	if (xml_input_filename != "")
//	{
//		cout << "Loading XML model : " << xml_input_filename << endl;
//		sim->ParseXML(xml_input_filename);
//	}
//
//	sim->CheckInputs();

//	ofstream os;
//	ifstream is;
//	sim->SetBinaryStreams(&os, &is);


	sim->Run();
}




