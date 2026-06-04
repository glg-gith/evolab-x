/*----------------------------------------------------------------------------*
**  sets.cpp : class definitions for set operations using bit representations. 
**
**  Stephen R. Schmitt
*/

#include "LabSet2.h"

namespace elps {

/*----------------------------------------------------------------------------*
**  "sets()" default constructor, initializes elements of the set to zero.  
**
**  returns: nothing
*/
LabSet2::LabSet2()
{
    clear();                                        // initialize to empty
}

/*----------------------------------------------------------------------------*
**  "sets()" copy constructor, initializes elements of the set to be
**  equal to the values of the argument set.  
**
**  returns: nothing
*/
LabSet2::LabSet2( const LabSet2 &rhs )                       // the set to copy
{
    for( int i = 0; i < MAX_WORDS; i++ )            // copy into new set
	set[i] = rhs.set[i];
}

/*----------------------------------------------------------------------------*
**  "item()" checks for existence of an item in the set.  
**
**  returns: 1 if item present, 0 if not, -1 on range error
*/
int LabSet2::item( int bit )                           // index of the item
{
    if( bit <= 0 || bit > MAX_WORDS * WORD_SIZE )   // in range?
	return -1;

    int i = bit / WORD_SIZE;
    bit -= i * WORD_SIZE;
    
    return set[i] & 1 << ( bit - 1 );               // binary AND
}

/*----------------------------------------------------------------------------*
**  "insert()" puts an item into the set. Range errors are ignored.  
**
**  returns: nothing
*/
void LabSet2::insert( int bit )                        // index of the item
{
    if( bit <= 0 || bit > MAX_WORDS * WORD_SIZE )   // in range?
	return;

    int i = bit / WORD_SIZE;
    bit -= i * WORD_SIZE;

    set[i] |= 1 << ( bit - 1 );                     // binary OR
}

/*----------------------------------------------------------------------------*
**  "remove()" takes an item out of the set. Range errors are ignored.  
**
**  returns: nothing
*/
void LabSet2::remove( int bit )                        // index of the item
{
    if( bit <= 0 || bit > MAX_WORDS * WORD_SIZE )   // in range?
	return;

    int i = bit / WORD_SIZE;
    bit -= i * WORD_SIZE;

    set[i] &= ~( 1 << ( bit - 1 ) );                // binary AND
}

/*----------------------------------------------------------------------------*
**  "clear()" removes all items from the set..  
**
**  returns: nothing
*/
void LabSet2::clear()
{
    for( int i = 0; i < MAX_WORDS; i++ )            // all words in set
        set[i] = 0;
}

/*----------------------------------------------------------------------------*
**  "define()" initializes elements of the set using an enumerated input 
**  string (numbers separated by spaces).  
**
**  NOTE: Values of set elements that are too large are ignored.
**
**  returns: nothing
*/
void LabSet2::define( char *input )                    // string of numbers
{
    int length = strlen( input );                   // of input buffer
    int pos = 0;                                    // current character
    clear();                                        // initialize to empty

    while( pos < length )                           // more input
    {
	int bit = 0;                                // init bit to set

	while( isdigit( input[pos] ) )
	{
	    bit *= 10;                              // compute bit to set
	    bit += input[pos] - '0';

	    pos++;                                  // advance
        }

	if( bit )                                   // zero means no bit
	    insert( bit );

	pos++;                                      // advance
    }
}

/*----------------------------------------------------------------------------*
**  "binary()" prints binary representation of set to stdout.
**
**  returns:  nothing
*/
void LabSet2::binary()
{
    for( int j = 0; j < MAX_WORDS; j++ )            // do all words
    {
        unsigned long mask = 1;                     // start at low bit

	for( int i = 0; i < WORD_SIZE; i++ )        // every bit in word
        {
            if( i % 4 == 0 )                        // add space
		putchar( ' ' );
            
	    set[j] & mask ? putchar( '1' ) : putchar( '0' );
	    
	    mask <<= 1;                             // next higher bit 
	}
    }
}

/*----------------------------------------------------------------------------*
**  "cardinality()" counts items in the set.
**
**  returns:  total items
*/
int LabSet2::cardinality()
{
    int count = 0;                                  // init counter

    for( int j = 0; j < MAX_WORDS; j++ )            // do all words
    {
        unsigned long mask = 1;                     // start at low bit
        
	for( int i = 0; i < WORD_SIZE; i++ )        // every bit in word
	{
	    count += set[j] & mask ? 1 : 0;
	    mask <<= 1;                             // next higher bit
	}
    }

    return count;
}

/*----------------------------------------------------------------------------*
**  "print()" displays set to stdout as a list of integers.
**
**  returns:  nothing
*/
void LabSet2::print()
{
    printf( "{ " );

    for( int j = 0; j < MAX_WORDS; j++ )            // do all words
    {
        unsigned long mask = 1;                     // start at low bit
        
	for( int i = 1; i <= WORD_SIZE; i++ )
	{
	    if( set[j] & mask )                     // check bit
		printf( "%d ", ( WORD_SIZE * j + i ) );

	    mask <<= 1;                             // next higher bit
	}
    }
    
    printf( "}" );
}

/*----------------------------------------------------------------------------*
**  "operator = " assigns the right hand side to this set.
**
**  returns: nothing
*/
const LabSet2 &LabSet2::operator = ( const LabSet2 &rhs )
{
    if( &rhs != this )                              // avoid self assignment
    {
	for( int i = 0; i < MAX_WORDS; i++ )        // copy into this set
	    set[i] = rhs.set[i];
    }

    return *this;                                   // enable x = y = z;
}

/*----------------------------------------------------------------------------*
**  "operator ~ " performs set complement operation (not).
**
**  returns: pointer to set
*/
LabSet2 LabSet2::operator ~ () const
{
    LabSet2 rv;
	
    for( int i = 0; i < MAX_WORDS; i++ )
	rv.set[i] = ~set[i];                        // bitwise complement

    return rv;
}
		      
/*----------------------------------------------------------------------------*
**  "operator + " performs set union operation (or).
**
**  returns: pointer to set
*/
LabSet2 LabSet2::operator + ( const LabSet2 &rhs )
{
    LabSet2 rv;
	
    for( int i = 0; i < MAX_WORDS; i++ )
	rv.set[i] = set[i] | rhs.set[i];            // bitwise OR

    return rv;
}

/*----------------------------------------------------------------------------*
**  "operator * " performs set intersection operation (and).
**
**  returns: pointer to set
*/
LabSet2 LabSet2::operator * ( const LabSet2 &rhs )
{
    LabSet2 rv;
    
    for( int i = 0; i < MAX_WORDS; i++ )
	rv.set[i] = set[i] & rhs.set[i];            // bitwise AND
                       
    return rv;
}

/*----------------------------------------------------------------------------*
**  "operator - " performs set difference operation.
**
**  returns: pointer to set
*/
LabSet2 LabSet2::operator - ( const LabSet2 &rhs )
{
    LabSet2 rv;
    
    for( int i = 0; i < MAX_WORDS; i++ )
	rv.set[i] = set[i] & ( ~rhs.set[i] );       // bitwise a AND ~b
                       
    return rv;
}

/*----------------------------------------------------------------------------*
**  "operator ^ " performs set symmetric difference operation (xor).
**
**  returns: pointer to set
*/
LabSet2 LabSet2::operator ^ ( const LabSet2 &rhs )
{
    LabSet2 rv;
    
    for( int i = 0; i < MAX_WORDS; i++ )
	rv.set[i] = set[i] ^ rhs.set[i];            // bitwise XOR
                       
    return rv;
}

#define TEST_CLASS                                  // define to test class
#ifdef TEST_CLASS

/*--------------------------------------------------------------------------*
**  "result()" prints test result to stdout.
**
**  returns: nothing
*/
void result( LabSet2 *r, char *msg )
{
    printf( msg ); 
    r->print(); 
    printf( "\n" );
}

/*--------------------------------------------------------------------------*
**  "main()" program entry point, this test several set operation cases.
**
**  returns: 0 if no errors occur
*/
int main( int argc, char* argv[] )
{
    LabSet2 A, B, C;

    printf( "Set operations program.\n");

    A.define( "1 2 3 4 5 6" ); 
    result( &A, "A = " );
    
    printf( "A - in binary low bit to high bit-\n" );
    A.binary();
    printf( " cardinality of A = %d\n", A.cardinality() );

    B.define( "4 5 6 7 8 9" );
    result( &B, "B = " );
    
    C.define( "7 8 9 10 11 12" );
    result( &C, "C = " ); 
    
    LabSet2 D = A + B + C;                             // uses copy constructor
    result( &D, "A + B + C = "  );

    D = A + B;                                      // A or B             
    result( &D, "A + B = " );

    D = A * B;                                      // A and B     
    result( &D, "A * B = " );

    D = A + C;                                      // A or C      
    result( &D, "A + C = " );

    D = A * C;                                      // A and C     
    result( &D, "A * C = " );

    D = A - B;                                      // set difference
    result( &D, "A - B = " );

    D = A * ~B;                                     // should equal A - B     
    result( &D, "A * ~B = " );

    D = A ^ B;                                      // A xor B (symmetric difference)
    result( &D, "A ^ B = " );

    D = ( A + B ) - ( A * B );                      // should equal A xor B 
    result( &D, "( A + B ) - ( A * B ) = " );

    D = ( A - B ) + ( B - A );                      // should equal A xor B  
    result( &D, "( A - B ) + ( B - A ) = " );

    D = ~(( ~A ) * ( ~B ));                         // should equal A or B (DeMorgan Law)
    result( &D, "~(( ~A ) * ( ~B )) = " );

    D = ~(( ~A ) + ( ~B ));                         // should equal A and B (DeMorgan Law)
    result( &D, "~(( ~A ) + ( ~B )) = " );

    D.item( 2 ) ? D.remove( 2 ) : D.insert( 2 );
    result( &D, "toggle item 2; D = " );

    D.item( 4 ) ? D.remove( 4 ) : D.insert( 4 );
    result( &D, "toggle item 4; D = " );

    char buf[32];
    //gets(buf); // Can overflow, as well as 'scanf'
    fgets(buf, sizeof(buf), stdin);
    return 0;
}

#undef TEST_CLASS

}

#endif



