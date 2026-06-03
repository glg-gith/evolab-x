/*
 * LabSet.h
 *
 *  Created on: Feb 20, 2013
 *      Author: golgauth
 */

#ifndef LABSETINT64_H_
#define LABSETINT64_H_

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <stdint.h>
#include <limits>
#include <algorithm>


#include <LabConsts.h>

//!
/* Enables _mm_popcnt_u32(), _mm_popcnt_u64() */
// use compiler options "-m64 -msse4.2" with this !
//#if (defined(_WIN32) || defined(_WIN64))
//  #include <nmmintrin.h>
//#else
//  #include <smmintrin.h>
//#endif


#include <LabTimeUtils.h>

#include <LabRngUtils.h>

using namespace std;


namespace elps {


// !! Compile option !!
/**
 * If the set to 1, will be optimized for counting (cardinality updated on the fly).
 * WARNING : Optimizing for counting will slow down a little the following features :
 * 				- Binary operation : (~, +, *, -, ^)
 */
#define CNT_OPT					1


#define DEFAULT_SIZE_IN_BITS 	1000000

#define ONE						(uint64_t)1

//This is better when most bits in x are 0
inline unsigned int popcount(uint64_t x) {
	//		// It uses 3 arithmetic operations and one comparison/branch per "1" bit in x.
	//		int count;
	//		for (count=0; x; count++)
	//			x &= x-1;
	//		return count;

	//!
	//return _mm_popcnt_u64(x); //__builtin_popcountl(x); //
	return __builtin_popcountll(x); //__builtin_popcountl(x);
}


#define DEBRUIJN 0x22fdd63cc95386dULL
static uint32_t table[64] ;
inline void inittable () {
    uint64_t db = DEBRUIJN ;
    int i ;
    for ( i=0 ; i < 64; ++i ) {
        table [db >> 58] = i ;
        db = db << 1;
    }
}

inline uint32_t ntz ( uint64_t n ) {
	//inittable ();
    n &= -n ;
    n *= DEBRUIJN ;
    n >>= 58;
    return table[n] ;
}

/**
 * Counts trailing zeros starting from a given point
 * @param v Input to count trailing zero bits
 * @return c will count v's trailing zero bits,
			 so if v is 1101000 (base 2), then c will be 3
 */
inline int nb_trailing_zeros(uint64_t v) {
	if (v & ONE) return 0;			// Fast check if no trailing zeros
									// Avoids a useless call to __builtin_ctzl()
	else return __builtin_ctzll(v); //__builtin_ctzl(v);
}

/**
 * Class implementing a <b>Set of Big Integers</b>.
 *
 * <b>Note :</b> The set is sorted in <b>ascendant</b> order.
 *
 * USAGE :
 * =======
 *
 * \code
 * 	LabSet a_set = LabSet(100);          // Get yourself a set which can contain numbers
 * 	...                                  // in range [0..100] (100 excluded).
 * 	a_set.Set(11)                        // Add number '11' to the set
 * 	a_set.Set(84)                        // Add number '84' to the set
 * 	a_set.Print()                        // Display it
 * 	...                                  // Shows : "{ 11 84 }"
 * \endcode
 *
 * <b>Note :</b> You can iterate through a Set by using a LabSet::iter.
 *
 */
class LIB_CLASS LabSet
{
public:
	/**
	 * Default constructor.
	 */
	LabSet();
	/**
	 * Instantiates a bitset (Set) with a given maximum size.
	 * @param sizeinbits Maximum size of the population.
	 */
	LabSet(unsigned int sizeinbits);
	/**
	 * Copy constructor.
	 * @param The Set to be cloned.
	 */
	LabSet(const LabSet &);

	virtual ~LabSet();

	/**
	 * Reset the Set to empty (full zeros).
	 */
	void Clear();
	/**
	 * Resize the Set.<br/>
	 * WARNING : If the new size is lower than the original size, the Set is truncated.
	 * @param sizeinbits The new size.
	 */
	void Resize(unsigned int sizeinbits);
	/**
	 * Add a number to the Set.
	 * WARNING : No check is perform whether i is in the correct range :
	 * 			 i MUST be in [0, size_in_bits_ - 1]
	 * @param i The number to be added.
	 */
	void Set(int i);
	/**
	 * Remove a number from the Set.
	 * WARNING : No check is perform whether i is in the correct range :
	 * 			 i MUST be in [0, size_in_bits_ - 1]
	 * @param i The number to be removed.
	 */
	void UnSet(int i);
	/**
	 * Get the number of elements currently in the Set.
	 * @return The number of elements.
	 */
	int  Cardinality();

	/**
	 * Find an element in the set
	 * @param i The number to look for.
	 * @return True if the number was found.
	 */
	bool Find(int i);
	/**
	 * Print a string representation of the Set.
	 */
	void Print();
	/**
	 * Print a string representation of the Set. Numbers are displayed in their binary form.
	 */
	void PrintBin();

	/**
	 * Get the first (smallest) element of the Set.
	 * @return The smallest integer found, or -2 if the Set was empty.
	 */
	int GetFirst();
	/**
	 * Choose an element using uniform integer random number generator.
	 * @param rng The Random Number Generator.
	 * @return Return The chosen number, or -2 if the Set was empty.
	 */
	int ChooseOne(LabRng *rng);
	/**
	 * Get an element at the specified position in the Set.
	 * @param n The position to look at.
	 * @return The requested number, or -2 if the Set was empty.
	 */
	int GetAt(int n);

	/**
	 * The maximum number the set can store (= size of the Set).
	 * @return The biggest number that can be handled by Set.
	 */
	int Capacity();

	/** Assignment operator. */
	const LabSet & operator = ( const LabSet & );
	/** Same as "operator =", but necessary as Python does not support "=" overloading. */
	const LabSet & operator << ( const LabSet & );

	/*
	 * All the following operators assume that left and right operands
	 * have the same size "size_in_bits" (enough for our needs,
	 * since memory usage is not really a problem for us).
	 */
	/** Inversion.      **/
	LabSet         operator ~  () const;
	/** Union          **/
	LabSet         operator +  ( const LabSet & );
	/** Intersection.   **/
	LabSet         operator *  ( const LabSet & );
	/** Difference.     **/
	LabSet         operator -  ( const LabSet & );
	/** Symmetrical D. **/
	LabSet         operator ^  ( const LabSet & );

	/*
	 * Comparison.
	 */
	/** Equality. */
	bool         	operator == ( const LabSet & ) const;
	/** Inequality. */
	bool         	operator != ( const LabSet & ) const;

	/**
	 * Convenient, moreover :
	 * (A += B) is actually a lot faster than (A = A + B)...
	 * [ No intermediary storage ]
	 */
	const LabSet & operator +=  ( const LabSet & );
	/** (A *= B) is faster than (A = A * B)... */
	const LabSet & operator *=  ( const LabSet & );
	/** (A -= B) is faster than (A = A - B)... */
	const LabSet & operator -=  ( const LabSet & );
	/** (A ^= B) is faster than (A = A ^ B)... */
	const LabSet & operator ^=  ( const LabSet & );

	/** Inversion */
	const LabSet & INV();

	/**
	 * Gets the population count of the union.<br/>
	 * Faster than realizing the union, then calling Cardinality().<br/>
	 * [ No intermediary storage ]
	 */
	int CU  ( const LabSet & );
	/** Faster counting of the intersection. */
	int CI  ( const LabSet & );
	/** Faster counting of the difference. */
	int CD  ( const LabSet & );
	/** Faster counting of the symmetrical difference. */
	int CSD ( const LabSet & );

	/**
	 * Basic LabSet iterator utility.
	 *
	 * USAGE :
	 * =======
	 *
	 * \code
	 * 	LabSet ls;
	 * 	int val;
	 * 	LabSet::iter it(ls);
	 * 	while ((val = it.next()) != it.end())
	 * 	{
	 * 			cout << (val) << " "; 			// Do something...
	 * 	}
	 * \endcode
	 *
	 * Or:
	 *
	 * \code
	 * 	LabSet ls;
	 * 	LabSet::iter it(ls);
	 * 	for (int val=it.next(); val != it.end(); val=it.next())
	 * 	{
	 * 			cout << (val) << " "; 			// Do something...
	 * 	}
	 * \endcode
	 *
	 */
	class iter {
	public:

		/** End marker */
		static const short END = -2;

		/**
		 * Instantiate an iterator for the given Set.
		 * @param bv Set to be browsed.
		 */
		iter(LabSet& bv) { pos_ = -1; bv_ = &bv; }
		virtual ~iter()  {  }

		/**
		 * Reset the iterator.
		 */
		void reset() { pos_ = -1; }
		/**
		 * Get the end marker.
		 * @return END (= -2)
		 */
		int  end()   { return END; }
		/**
		 * Get the next number from the current position of the iterator.
		 * @return The number immediately following the current one.
		 */
		int  next()  { pos_ = bv_->next_set_bit(pos_); return pos_; }

	private:
		/** Current position */
		int  	 pos_;
		/** Reference to the iterable Set */
		LabSet * bv_;
	};

private:

	/** Data storage raw array. */
	uint64_t *   data_;
	/** Data array size. */
	unsigned int data_len_;
	/** The maximum number the set can store (= size of the Set). */
	unsigned int size_in_bits_;
	/** The number of elements currently in the Set. */
	unsigned int cardinality_;

	/**
	 * Initializer - common to all constructors.
	 * @param sizeinbits The initial Set's capacity.
	 */
	void init_bitset(unsigned int sizeinbits);
	/**
	 * Calculates the Set's cardinality.
	 * @return
	 */
	int  calc_cardinality();
	/**
	 * Get the number immediately following the given one.
	 * @param i The number from which to start searching.
	 * @return The next number, -2 if the Set was empty.
	 */
	int  next_set_bit(int i);

//	LabRng *rng;
};


} /* namespace elps */
#endif /* LABSETINT64_H_ */
