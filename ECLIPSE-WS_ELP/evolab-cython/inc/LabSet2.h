/*----------------------------------------------------------------------------*
**  sets.h : class declaration for set operations using bit representations. 
**
**  Stephen R. Schmitt
*/

#ifndef LABSET_H
#define LABSET_H


#include <ctype.h>
#include <stdio.h>
#include <string.h>


namespace elps {

#define MAX_WORDS  2
#define WORD_SIZE  ( 8 * sizeof( unsigned long ) )

class LabSet2
{
public:
    LabSet2();
    LabSet2( const LabSet2 & );
    void binary();
    int  cardinality();
    void clear();
    void define( char * );
    void insert( int );
    int  item( int );
    void print();
    void remove( int );

    const LabSet2 &operator = ( const LabSet2 & );
    LabSet2 operator ~ () const;
    LabSet2 operator + ( const LabSet2 & );
    LabSet2 operator * ( const LabSet2 & );
    LabSet2 operator - ( const LabSet2 & );
    LabSet2 operator ^ ( const LabSet2 & );

private:
    unsigned long set[MAX_WORDS];                   // the set
};

}

#endif //LABSET_H
