/*
 * LabSet1.h
 *
 *  Created on: Feb 13, 2013
 *      Author: golgauth
 */

#ifndef LABSET_H_
#define LABSET_H_


#include <iostream>
#include <set>
#include <algorithm>


using namespace std;

namespace elps {


template < class T >
class LabSet1 {

public:

	LabSet1<T>();
	/**
	 * Copy constructor.
	 * @param Another LabSet1
	 */
    LabSet1<T>(const LabSet1<T> &);
	virtual ~LabSet1<T>();

	inline set<T> GetSet() { return _set; }

    int  Cardinality();
    void Clear();
    void Insert(T elt);
    bool Remove(T elt);
    bool Find(T elt);
    void Print();

    int FindPos(T elt);
    T& GetAt(int pos);
    bool RemoveAt(unsigned int pos);

    //const LabSet1<T> &operator = (const LabSet1<T> &); # Op. "=" NOT YET SUPPORTED BY Cython
    const LabSet1<T> &operator << (const LabSet1<T> &);
    //LabSet1 operator ~ () const;
    LabSet1<T>& operator + (const LabSet1<T> &);
    LabSet1<T> operator * (const LabSet1<T> &);
    LabSet1<T> operator - (const LabSet1<T> &);
    LabSet1<T> operator ^ (const LabSet1<T> &);

    set<T> _set;

private:

};


//-----------------------------------------------------------------------------------------------


template < class T>
LabSet1<T>::LabSet1() {
	// TODO Auto-generated constructor stub

}

template < class T>
LabSet1<T>::~LabSet1() {
	// TODO Auto-generated destructor stub
}

/*----------------------------------------------------------------------------*
 **  "sets()" copy constructor, initializes elements of the set to be
 **  equal to the values of the argument set.
 **
 **  returns: nothing
 */
template < class T>
LabSet1<T>::LabSet1( const LabSet1<T> &rhs )                       // the set to copy
{
	//set<t_set_element>::iterator it;
	//	_set.clear();
	//	for (it=rhs._set.begin(); it != rhs._set.end(); ++it) {
	//		_set.insert(*it);
	//	}
	_set = set<T>(rhs._set);
}

/*----------------------------------------------------------------------------*
 **  "clear()" removes all items from the set..
 **
 **  returns: nothing
 */
template < class T>
void LabSet1<T>::Clear()
{
	_set.clear();
}

/*----------------------------------------------------------------------------*
 **  "insert()" puts an item into the set. Range errors are ignored.
 **
 **  returns: nothing
 */
template < class T>
void LabSet1<T>::Insert( T elt )                        // index of the item
{
	_set.insert(elt);
	//cout << "Inserted : " << elt << endl;
}

/*----------------------------------------------------------------------------*
 **  "remove()" takes an item out of the set. Range errors are ignored.
 **
 **  returns: nothing
 */
template < class T>
bool LabSet1<T>::Remove( T elt )                        // index of the item
{
	int nb_elt = _set.erase(elt);
	return (nb_elt);
}

/*----------------------------------------------------------------------------*
 **  "item()" checks for existence of an item in the set.
 **
 **  returns: if item present
 */
template < class T>
bool LabSet1<T>::Find( T elt )
{
	return (_set.find(elt) != _set.end());
}



/*----------------------------------------------------------------------------*
 **  "cardinality()" counts items in the set.
 **
 **  returns:  total items
 */
template < class T>
int LabSet1<T>::Cardinality()
{
	return _set.size();
}

/*----------------------------------------------------------------------------*
 **  "print()" displays set to stdout as a list of elements.
 **
 **  returns:  nothing
 */
template < class T>
void LabSet1<T>::Print()
{
	class set<T>::iterator it;

	cout << "{ ";
	for (it=_set.begin(); it != _set.end(); ++it) {
		cout << (*it) << " ";
	}
	cout << "}" << endl;
}


/*----------------------------------------------------------------------------*
 **  "item()" find position of element in the set.
 **
 **  returns: a position
 */
template < class T>
int LabSet1<T>::FindPos( T elt )                           // index of the item
{
	class set<T>::const_iterator it = _set.find(elt);
	if( it == _set.end() )
		 return -1;

	return std::distance( _set.begin(), it );
}

/*----------------------------------------------------------------------------*
 **  "GetAt()" returns the element located at 'pos' position.
 **
 **  returns:  nothing
 */
template < class T>
T& LabSet1<T>::GetAt(int pos)
{
	class set<T>::const_iterator it(_set.begin());
	// 'advance' the iterator 'pos' times
	std::advance(it, pos);
	//cout << "ELEMENT = " << (*it) << endl;
	//this->Print();
	return (it == _set.end()) ? 0 : (*it);
}

/*----------------------------------------------------------------------------*
 **  "Remove()" removes the element located at 'pos' position.
 **
 **  returns:  nothing
 */
template < class T>
bool LabSet1<T>::RemoveAt(unsigned int pos)
{
	if (pos < _set.size())
	{
		class set<T>::const_iterator it(_set.begin());
		// 'advance' the iterator 'pos' times
		std::advance(it, pos);
		//cout << "ELEMENT = " << (*it) << endl;
		//this->Print();

		_set.erase(it);
		return true;
	}
	return false;
}


/*----------------------------------------------------------------------------*
 **  "operator = " assigns the right hand side to this set.
 **
 **  returns: nothing
 */
template < class T>
const LabSet1<T> &LabSet1<T>::operator << ( const LabSet1<T> &rhs )
{
	_set = set<T>(rhs._set);
	return *this;
}

/*----------------------------------------------------------------------------*
 **  "operator ~ " performs set complement operation (not).
 **
 **  returns: pointer to set
 */
//LabSet1 LabSet1::operator ~ () const
//				{
//	LabSet1 rv;
//
//	for( int i = 0; i < MAX_WORDS; i++ )
//		rv.set[i] = ~set[i];                        // bitwise complement
//
//	return rv;
//				}

/*----------------------------------------------------------------------------*
 **  "operator + " performs set union operation (or).
 **
 **  returns: pointer to set
 */
template < class T>
LabSet1<T>& LabSet1<T>::operator + ( const LabSet1<T> &rhs )
{
	LabSet1<T> ret;
	//set<T> res;
	set_union(_set.begin(), _set.end(), rhs._set.begin(), rhs._set.end(), inserter(ret._set, ret._set.end()));
	//ret._set = set<T>(res);
	return ret;
}

/*----------------------------------------------------------------------------*
 **  "operator * " performs set intersection operation (and).
 **
 **  returns: pointer to set
 */
template < class T>
LabSet1<T> LabSet1<T>::operator * ( const LabSet1<T> &rhs )
{
	LabSet1<T> ret;
	set<T> res;
	set_intersection(_set.begin(), _set.end(), rhs._set.begin(), rhs._set.end(), inserter(res, res.end()));
	ret._set = set<T>(res);
	return ret;
}

/*----------------------------------------------------------------------------*
 **  "operator - " performs set difference operation.
 **
 **  returns: pointer to set
 */
template < class T>
LabSet1<T> LabSet1<T>::operator - ( const LabSet1<T> &rhs )
{
	LabSet1 ret;
	set<T> res;
	set_difference(_set.begin(), _set.end(), rhs._set.begin(), rhs._set.end(), inserter(res, res.end()));
	ret._set = set<T>(res);
	return ret;
}

/*----------------------------------------------------------------------------*
 **  "operator ^ " performs set symmetric difference operation (xor).
 **
 **  returns: pointer to set
 */
template < class T>
LabSet1<T> LabSet1<T>::operator ^ ( const LabSet1<T> &rhs )
{
	LabSet1 ret;
	set<T> res;
	set_symmetric_difference(_set.begin(), _set.end(), rhs._set.begin(), rhs._set.end(), inserter(res, res.end()));
	ret._set = set<T>(res);
	return ret;
}


} /* namespace elps */
#endif /* LABSET_H_ */
