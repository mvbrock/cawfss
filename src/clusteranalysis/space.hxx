/////////////////////////////
// K-Means Library
// Author: Matt Brock
// Date: 08-28-2007
// Last Updated: 09-25-2007
/////////////////////////////

#ifndef SPACE_HXX
#define SPACE_HXX

#include <list>
#include "rng.h"
#include "coordinate.hxx"

using namespace std;

/// The Space class.
/// This class contains the data points, cluster centers, dimension, and functionality
/// for grouping data points and shifting the cluster centers to the center of these newly
/// formed groups of data points.

/// A container for points
class Space
{
public:
	Space();
	Space( int dimension );
	Space( int dimension, list<Coordinate *> & pointList );
	virtual ~Space();

	/// Add a coordinate specifying a data point
	bool AddPoint( Coordinate * point );
	/// Add a list of coordinates specifying multiple data points.
	bool AddPointList( list<Coordinate *> & pointList );
	/// Add a list of coordinates, from a CSV file, specifying multiple data points.
	int AddPointList( char * filename );
	/// Clear the space of all data points
	int ClearPoints();
	/// Clear the space of all data points and delete the
	/// data points from memory.
	int DeletePoints();
	/// Retrieve the list of points.
	list<Coordinate *> & GetPoints();

	/// Normalize the data to [0,1]
	int Normalize();
	/// Place any point exceeding the [0,1] boundary on the boundary.
	int Boundarize();

	/// Get the dimension of the space
	int GetDimension();
	/// Set the dimension of the space
	int SetDimension( int dimension );

	
	/// Print out the coordinates in the space;
	void Print();

	/// How many points are in this space.
	int Size();

	/// Does each point have the right number of dimensions?
	bool HasValidPoints();
protected:
	bool isCluster;
	int dimension;
	list<Coordinate *> pointList;	
	RNG r;

};

#endif

