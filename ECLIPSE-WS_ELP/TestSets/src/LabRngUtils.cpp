/*
 * LabRngUtils.cpp
 *
 *  Created on: 14 sept. 2012
 *      Author: golgauth
 */

#include "LabRngUtils.h"


namespace elps {


LabRngUtils::LabRngUtils() {
	// TODO Auto-generated constructor stub

}

LabRngUtils::~LabRngUtils() {
	// TODO Auto-generated destructor stub
}

gsl_rng *LabRngUtils::CreateRandomizer(int ralgo, int rseed)
{
	// Initialize randomizer
	gsl_rng *r = NULL;

	// Choose randomizer
	switch (ralgo)
	{
	case 0: r = gsl_rng_alloc(gsl_rng_mt19937); break;
	case 1: r = gsl_rng_alloc(gsl_rng_taus); break;
	case 2: r = gsl_rng_alloc(gsl_rng_gfsr4); break;
	}

	// Generate a seed if necessary
	if (rseed == -1)
	{
		// Get a different seed at each program run using the system current time
		// Otherwise : use the default fixed one
		srand(time(NULL));
		rseed = rand();
	}
	// Setup randomizer
	gsl_rng_set(r, rseed);

	return r;
}



LabRng::LabRng() {
	Init(0, -1);
}
LabRng::LabRng(int ralgo, int rseed) {
	Init(ralgo, rseed);
}
void LabRng::Init(int ralgo, int rseed) {

	// Choose randomizer
	switch (ralgo)
	{
	case 0: rng = gsl_rng_alloc(gsl_rng_mt19937); break;
	case 1: rng = gsl_rng_alloc(gsl_rng_taus); break;
	case 2: rng = gsl_rng_alloc(gsl_rng_gfsr4); break;
	}

	// Generate a seed if necessary
	if (rseed == -1)
	{
		// Get a different seed at each program run using the system current time
		// Otherwise : use the default fixed one
		srand(time(NULL));
		rseed = rand();
	}
	// Setup randomizer
	gsl_rng_set(rng, rseed);

}


LabRng::~LabRng() {
//	cout << "Destroying RNG : " << rng << " on RNGRandomizer : " << this << endl;
//	if (rng) gsl_rng_free(rng);
//	//rng = NULL;
//	cout << "Destroyed." << rng << endl;
}

double LabRng::GetNextDouble() {
	return gsl_rng_uniform(rng);
}

unsigned int LabRng::GetNextInt(int max) {
	return gsl_rng_uniform_int(rng, max);
}

unsigned int LabRng::GetNextIntMultinomial(double max_cumsum, vector<double>& p_nomes) {

	int n = 0;
	double add = 0.0;
	double p = this->GetNextDouble() * max_cumsum;

	//if (max_cumsum == 0.0) return p_nomes.size();

	for (unsigned int i=0; i<p_nomes.size(); i++)
	{
		add += p_nomes[i];
		if (p < add) break;
		else n++;
	}
//	cerr << "nnn = " << n << endl;
	return n;
}



unsigned int LabRng::GetNextIntPoisson(double mu) {
	return gsl_ran_poisson(rng, mu);
}



}  // namespace elps
