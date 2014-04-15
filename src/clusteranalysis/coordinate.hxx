/////////////////////////////
// K-Means Library
// Author: Matt Brock
// Date: 08-28-2007
// Last Updated: 09-25-2007
/////////////////////////////

#ifndef COORDINATE_HXX
#define COORDINATE_HXX

#include <vector>
#include <string>
#include <list>
#include "rng.h"

using namespace std;

/** \brief A point that can occupy a space.
  *
  * Coordinate contains a vector that represents coordinates in a space.  Coordinate
  * also has several methods that can transform a point or list of points inside of
  * a space.
  */



class Coordinate
{
public:
		/** \brief Constructs the coordinate class.
		  *
		  */
	Coordinate();
		/** \brief Constructs the coordinate class.
		  *
		  * Sets the coordinate vector for the class.
		  * \param coordinateData The coordinate vector.
		  */
	Coordinate( vector<double> & coordinateData, unsigned int id );
	~Coordinate();

	vector<double> & GetCoordinateData();
	int GetDimension();
	void SetCoordinateData( vector<double> & coordinateData );
	void Print();
	string ToString();

	int Invert();
	int Boundarize();

	Coordinate Reduced( list<int> & dimensions );

	Coordinate & operator=( const Coordinate & rhs );
	Coordinate operator+( const Coordinate & rhs );
	Coordinate operator-( const Coordinate & rhs );	
	Coordinate operator*( const Coordinate & rhs );
	Coordinate operator/( const Coordinate & rhs );
	bool operator<( const Coordinate & rhs );
	bool operator>( const Coordinate &rhs );
	bool operator==( const Coordinate & rhs );
	bool operator!=( const Coordinate & rhs );

	static double Distance( Coordinate & coordinateOne, Coordinate & coordinateTwo );
	
	static Coordinate Abs( Coordinate & coordinate );
	static Coordinate Average( list<Coordinate *> coordList, int dimension );
	static Coordinate Random( int dimension, RNG & r );
	static Coordinate Singular( double value, int dimension );
	static Coordinate SquareRoot( Coordinate & coordinate );
	static double StandardDeviation( list<Coordinate *> & coordList, Coordinate & coordinate );

protected:
	vector<double> coordinateData;
	int coordinateId;
};

#endif

