/*
 * LabSet.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: golgauth
 */

#include "LabSet.h"

namespace elps {


LabSet::LabSet() {
	init_bitset(DEFAULT_SIZE_IN_BITS);
}

LabSet::LabSet(int sizeinbits) {
	init_bitset(sizeinbits);
}

void LabSet::init_bitset(unsigned int sizeinbits) {
	// +1 (the last higher weighted bit is 0 : allows to stop when iterating)
	// See example in "Print()" function
	size_in_bits_ = sizeinbits + 1;
	data_len_ = (size_in_bits_ + 63) / 64;
	data_ = new uint64_t[data_len_];
	if (CNT_OPT) cardinality_ = 0;
	Clear();						// Start filled with 0s. // Necessary...

//	rng = new LabRng();

//	cout << "Created LabSet : " << this << endl;
}

LabSet::LabSet(const LabSet& src) {
	size_in_bits_ = src.size_in_bits_;
	data_len_ = src.data_len_;
	data_ = new uint64_t[data_len_];
	memcpy( data_, src.data_, src.data_len_ * sizeof(uint64_t) );
	cardinality_ = src.cardinality_;

//	rng = new LabRng();

//	cout << "Created LabSet (by COPY !!) : " << this << endl;
}


//// Cython !!!
#ifndef NOPYTHON
LabSet::LabSet(PyObject *obj): LabSet() {
	//if (obj == NULL) { this->m_obj = NULL; return; }
	//if (this->m_obj) Py_XDECREF(this->m_obj);
	this->m_obj = obj;
	//Py_XINCREF(this->m_obj);
}

LabSet::LabSet(PyObject *obj, int sizeinbits): LabSet(sizeinbits) {
	//if (obj == NULL) { this->m_obj = NULL; return; }
	//if (this->m_obj) Py_XDECREF(this->m_obj);
	this->m_obj = obj;
	//Py_XINCREF(this->m_obj);
}

LabSet::LabSet(PyObject *obj, const LabSet& src): LabSet(src) {
	//if (obj == NULL) { this->m_obj = NULL; return; }
	//if (this->m_obj) Py_XDECREF(this->m_obj);
	this->m_obj = obj;
	//Py_XINCREF(this->m_obj);
}
#endif


LabSet::~LabSet() {

	if (data_ != NULL) delete[] data_;
//	if (rng != NULL) delete rng;

}

void LabSet::Clear() {
	std::fill_n(data_, data_len_, 0);

	if (CNT_OPT) cardinality_ = 0;
}


void LabSet::Resize(unsigned int sizeinbits) {

   	bool truncated;
   	unsigned int new_sizeinbits = sizeinbits + 1;

   	if (size_in_bits_ != new_sizeinbits)
   	{

   		truncated = (size_in_bits_ > new_sizeinbits);
   		size_in_bits_ = new_sizeinbits;
   		unsigned int new_size = (size_in_bits_ + 63) / 64;

   		if (new_size != data_len_) 		// Resize only if storage size changed
   		{
   			uint64_t *new_data = new uint64_t[new_size];
   			if (!truncated) 			// Clear : required only if extended
   				std::fill_n(new_data, new_size, 0);

   			memcpy( new_data, data_, std::min(data_len_, new_size) * sizeof(uint64_t) );

   			data_len_ = new_size;
   			delete [] data_;
   			data_ = new_data;
   		}

   		//--
   		if (truncated) {
   		    // Clear the bits beyond the maximum size
   		    unsigned int begin = sizeinbits;
   		    unsigned int end = data_len_ * 64;
   		    for ( unsigned int i = begin; i<end; ++i ) UnSet(i);
   		    // Update cardinality
   		    if (CNT_OPT) cardinality_ = calc_cardinality();
   		}
   	}
}


void LabSet::Set(int i) {

	int n = i / 64;
	uint64_t shift1l = ONE << (i % 64);
	if ( !(data_[n] & shift1l) ) {				// Not found
		data_[n] |= shift1l;
		if (CNT_OPT) ++cardinality_;
	}
}

void LabSet::UnSet(int i) {

	int n = i / 64;
	uint64_t shift1l = ONE << (i % 64);
	if ( data_[n] & shift1l ) {					// Found
		data_[n] &= ~shift1l;
		if (CNT_OPT) --cardinality_;
	}
}

//void LabSet::Set(int i, bool b) {
//	if(b) Set(i); else UnSet(i);
//}

bool LabSet::Find(int i) {
	return ((data_[i / 64]) & (ONE << (i % 64)));
}


int LabSet::Cardinality() {
	if (CNT_OPT) return cardinality_;
	else return calc_cardinality();
}


int LabSet::calc_cardinality() {
	unsigned int sum = 0;

	for(unsigned int i=0; i<data_len_; i++)
		sum += popcount(data_[i]);
		//sum += bitcount64_4way(data[i], data[i+1], data[i+2], data[i+3]);

	return sum;
}

int LabSet::next_set_bit(int i) {
	++i;
	unsigned int x = i / 64;
	uint64_t w = data_[x];
	w >>= (i % 64);
	if (w != 0) {
		return i + nb_trailing_zeros(w);
	}
	++x;
	for (; x < data_len_; ++x) {
		if (data_[x] != 0) {
			return x * 64 + nb_trailing_zeros(data_[x]);
		}
	}
	return -2;
}


void LabSet::Print() {

	cout << "\nSet size : " << this->Cardinality() << endl;
	cout << "{ ";
	int cnt = 0;
	iter it(*this);
	for (int val = it.next(); val != it.end(); val = it.next())
	{
		cout << (val) << " ";
		if ((++cnt) % 30 == 0) cout << endl;
		//cnt++;
	}
	cout << "}" << endl;
	//cout << "COUNT : " << cnt << endl;
}

void LabSet::PrintBin() {

	cout << "\nData size : " << this->data_len_ << endl;
	cout << "{ ";
	iter it(*this);
	for (unsigned int i=0; i<this->data_len_; i++)
	{
		for (int j=0; j<64; j++)
		{
			cout << (((this->data_[i] >> j) & ONE) ? 1 : 0);
		}
		cout << "\n ";
	}
	cout << "}" << endl;
}



int LabSet::GetFirst() {
	return this->next_set_bit(-1);
}

int LabSet::ChooseOne(LabRng *rng) {
	// Get lucky...
	// [still uniform : each one gets exactly 1/(set_size) chances to be the one]
	int ret = rng->GetNextInt(this->size_in_bits_ - 1);

	// Otherwise...
	if (!Find(ret)) {
		ret = GetAt(rng->GetNextInt(this->Cardinality()));
	}

//	cout << "choose one : " << ret << endl;
	return ret;
//	return GetAt(rng->GetNextInt(this->Cardinality()));
}

int LabSet::GetAt(int n) {

	int cnt = 0, val;
	iter it(*this);
	while ((val = it.next()) != it.end() && (n != cnt++)) {  }

	return val;
}


int LabSet::Capacity() {
	return this->size_in_bits_ - 1;
}


// *************************************************



/*----------------------------------------------------------------------------*
 **  "operator = " assigns the right hand side to this set.
 **
 **  returns: nothing
 */
const LabSet & LabSet::operator = ( const LabSet &rhs )
{
	if( &rhs != this )                              // avoid self assignment
	{
//		cout << "'=' operator called !!!" << endl;
//
//		cout << "this->data_len_ = " << this->data_len_ << endl;
//		cout << "rhs->data_len_ = " << rhs.data_len_ << endl;

		this->Resize(rhs.size_in_bits_ - 1);
		memcpy( data_, rhs.data_, rhs.data_len_ * sizeof(uint64_t) );

//		cout << "this->data_len_ = " << this->data_len_ << endl;
//		cout << "rhs->data_len_ = " << rhs.data_len_ << endl;


		if (CNT_OPT) cardinality_ = rhs.cardinality_;
	}
	return *this;                                   // enable x = y = z;
}
const LabSet & LabSet::operator << ( const LabSet &rhs )
{
	////*this = rhs;
	return (*this = rhs);									// enable x = y = z;
}

/*----------------------------------------------------------------------------*
**  "operator ~ " performs set complement operation (not).
**
**  returns: pointer to set
*/
LabSet LabSet::operator ~ () const
{
    LabSet rv(size_in_bits_-1);

    unsigned int i;
    for( i = 0; i < data_len_; i++ ) {
    	rv.data_[i] = ~data_[i];                        // bitwise complement
    	if (CNT_OPT) rv.cardinality_ += popcount(rv.data_[i]);
    }

    rv.size_in_bits_ = size_in_bits_;
    // Clear the bits beyond the maximum size
    unsigned int begin = rv.size_in_bits_ - 1;
    unsigned int end = data_len_ * 64;
    for ( i = begin; i<end; ++i ) rv.UnSet(i);

    return rv;
}


/*----------------------------------------------------------------------------*
**  "operator + " performs set union operation (or).
**
**  returns: pointer to set
*/
LabSet LabSet::operator + ( const LabSet &rhs )
{
    LabSet rv(size_in_bits_-1);

    for( unsigned int i = 0; i < data_len_; i++ ) {
    	rv.data_[i] = data_[i] | rhs.data_[i];            // bitwise OR
		if (CNT_OPT) rv.cardinality_ += popcount(rv.data_[i]);
	}

    return rv;
}


/*----------------------------------------------------------------------------*
**  "operator * " performs set intersection operation (and).
**
**  returns: pointer to set
*/
LabSet LabSet::operator * ( const LabSet &rhs )
{
	LabSet rv(size_in_bits_-1);

    for( unsigned int i = 0; i < data_len_; i++ ) {
    	rv.data_[i] = data_[i] & rhs.data_[i];        // bitwise AND
		if (CNT_OPT) rv.cardinality_ += popcount(rv.data_[i]);
	}

    return rv;
}


/*----------------------------------------------------------------------------*
**  "operator - " performs set difference operation.
**
**  returns: pointer to set
*/
LabSet LabSet::operator - ( const LabSet &rhs )
{
	LabSet rv(size_in_bits_-1);

    for( unsigned int i = 0; i < data_len_; i++ ) {
    	rv.data_[i] = data_[i] & ( ~rhs.data_[i] );       // bitwise a AND ~b
		if (CNT_OPT) rv.cardinality_ += popcount(rv.data_[i]);
	}

    return rv;
}


/*----------------------------------------------------------------------------*
**  "operator ^ " performs set symmetric difference operation (xor).
**
**  returns: pointer to set
*/
LabSet LabSet::operator ^ ( const LabSet &rhs )
{
	LabSet rv(size_in_bits_-1);

    for( unsigned int i = 0; i < data_len_; i++ ) {
    	rv.data_[i] = data_[i] ^ rhs.data_[i];            // bitwise XOR
		if (CNT_OPT) rv.cardinality_ += popcount(rv.data_[i]);
	}

    return rv;
}



bool LabSet::operator == ( const LabSet &rhs ) const {

	if ((CNT_OPT) && (cardinality_ != rhs.cardinality_)) return false;

    for( unsigned int i = 0; i < data_len_; i++ )
    {
    	if (data_[i] != rhs.data_[i]) {
    		return false;
    	}
    }
    return true;
}

bool LabSet::operator != ( const LabSet &rhs ) const {

    return !(*this == rhs);
}



const LabSet & LabSet::operator +=  ( const LabSet & rhs) {

	if (CNT_OPT) cardinality_ = 0;
    for( unsigned int i = 0; i < data_len_; i++ ) {
    	data_[i] = data_[i] | rhs.data_[i];
		if (CNT_OPT) cardinality_ += popcount(data_[i]);
	}

    return *this;
}

const LabSet & LabSet::operator *=  ( const LabSet & rhs) {

	if (CNT_OPT) cardinality_ = 0;
    for( unsigned int i = 0; i < data_len_; i++ ) {
    	data_[i] = data_[i] & rhs.data_[i];
		if (CNT_OPT) cardinality_ += popcount(data_[i]);
	}

    return *this;
}

const LabSet & LabSet::operator -=  ( const LabSet & rhs) {

	if (CNT_OPT) cardinality_ = 0;
    for( unsigned int i = 0; i < data_len_; i++ ) {
    	data_[i] = data_[i] & ( ~rhs.data_[i] );
		if (CNT_OPT) cardinality_ += popcount(data_[i]);
	}

    return *this;
}

const LabSet & LabSet::operator ^=  ( const LabSet & rhs) {

	if (CNT_OPT) cardinality_ = 0;
    for( unsigned int i = 0; i < data_len_; i++ ) {
    	data_[i] = data_[i] ^ rhs.data_[i];
		if (CNT_OPT) cardinality_ += popcount(data_[i]);
	}

    return *this;
}


// *************************************************

const LabSet & LabSet::INV() {

    unsigned int i;

	if (CNT_OPT) cardinality_ = 0;
    for( i = 0; i < data_len_; i++ ) {
    	data_[i] = ~data_[i];
    	if (CNT_OPT) cardinality_ += popcount(data_[i]);
    }

    // Clear the bits beyond the maximum size
    unsigned int begin = size_in_bits_ - 1;
    unsigned int end = data_len_ * 64;
    for ( i = begin; i<end; ++i ) UnSet(i);

    return *this;
}

// *************************************************


int LabSet::CU ( const LabSet & rhs ) {
	int sum = 0;
	for( unsigned int i=0; i < data_len_; i++ )
		sum += popcount(data_[i] | rhs.data_[i]);
	return sum;
}

int LabSet::CI  ( const LabSet & rhs ) {

//	cout << "CI : this->data_len_ = " << this->data_len_ << endl;
//	cout << "CI : rhs->data_len_ = " << rhs.data_len_ << endl;

	int sum = 0;
	for( unsigned int i=0; i < data_len_; i++ )
		sum += popcount(data_[i] & rhs.data_[i]);
//	cout << "CI sum = " << sum << endl;
	return sum;
}

int LabSet::CD  ( const LabSet & rhs ) {
	int sum = 0;
	for( unsigned int i=0; i < data_len_; i++ )
		sum += popcount(data_[i] & (~rhs.data_[i]));
	return sum;
}

int LabSet::CSD ( const LabSet & rhs ) {
	int sum = 0;
	for( unsigned int i=0; i < data_len_; i++ )
		sum += popcount(data_[i] ^ rhs.data_[i]);
	return sum;
}


} /* namespace elps */
