/*
 * LabRngUtils.h
 *
 *  Created on: 14 sept. 2012
 *      Author: golgauth
 */

#ifndef LABRNGUTILS_H_
#define LABRNGUTILS_H_

#include <iostream>

#include <math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <sys/time.h>
#include <time.h>

#include <vector>


#include <LabConsts.h>

using namespace std;

namespace elps {

/**
 * This class is a simplified access to Random Number Generation.<br/>
 * Mainly (but not only) a subset of features already available in GSL :<br/>
 * http://www.gnu.org/software/gsl/manual/gsl-ref.html
 */
class LIB_CLASS LabRngUtils {
public:
	LabRngUtils();
	virtual ~LabRngUtils();

	/**
	 * Initialize a RNG specifying an algorithm (default is 0 : Mersenne Twister).
	 * @param ralgo Can be 0, 1 or 2 (respectively : mt19937 - the default, taus, gfsr4)
	 * @param rseed Seed used to generate the numbers (default is -1 : clock based)
	 *              (Aka : series of random numbers are different each time the program is run)
	 * @return A pointer to a GSL random number generator
	 */
	static gsl_rng *CreateRandomizer(int ralgo = 0, int rseed = -1);
};

/**
 * This class is a simplified access to Random Number Generation.<br/>
 * Mainly (but not only) a subset of features already available in GSL :<br/>
 * http://www.gnu.org/software/gsl/manual/gsl-ref.html
 */
class LIB_CLASS LabRng {
public:
	LabRng();
	/**
	 * Initialize a RNG specifying an algorithm (default is 0 : Mersenne Twister).
	 * @param ralgo Can be 0, 1 or 2 (respectively : mt19937 - the default, taus, gfsr4)
	 * @param rseed Seed used to generate the numbers (default is -1 : clock based).
	 *              Series of random numbers are different each time the program is run,
	 *              based on the computer's time clock.
	 */
	LabRng(int ralgo, int rseed);
	virtual ~LabRng();

	/**
	 * Initialization method.
	 * @param ralgo
	 * @param rseed
	 */
	void Init(int ralgo = 0, int rseed = -1);

	// Rng
	/**
	 * GSL : Get a random double in range [0.0, 1.0].
	 * @return A random double between 0.0 and 1.0 included.
	 */
	double GetNextDouble();
	/**
	 * GSL : Get a random in in range [0, max-1].
	 * @param max The maximum returned plus 1.
	 * @return A random integer between 0 and max-1 included.
	 */
	unsigned int GetNextInt(int max);
	/**
	 * CUSTOM : Assuming max_cumsum is the cumulative sum of all the nomes,
	 * returns the index of a nome in the nomes list according to its probability
	 * to be chosen.
	 * @param max_cumsum The cumulative sum of all the elements of vector "p_nomes".
	 * @param p_nomes The vector containing the probabilities [0.0, 1.0] for each nome.
	 * @return The index of the chosen nome in the vector "p_nomes".
	 */
	unsigned int GetNextIntMultinomial(double max_cumsum, vector<double>& p_nomes);

	// RanDist
	//unsigned int GetNextIntMultinomial2(size_t K, unsigned int N, const double p[], unsigned int n[]);
	/**
	 * GSL : This function returns a random integer from the Poisson distribution with mean mu.
	 * @param mu Mean of the distribution
	 * @return A random integer
	 */
	unsigned int GetNextIntPoisson(double mu);

private:
	gsl_rng *rng;
};

}  // namespace elps

#endif /* LABRNGUTILS_H_ */
