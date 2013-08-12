/////////////////////////////
// K-Means Library
// Author: Matt Brock
// Date: 08-28-2007
// Last Updated: 09-25-2007
/////////////////////////////

#include <vector>
#include <list>
#include <iostream>
#include <string>
#include "rng.h"

using namespace std;

class Space;
class Cluster;
class Coordinate;

/// The Space class.
/// This class contains the data points, cluster centers, dimension, and functionality
/// for grouping data points and shifting the cluster centers to the center of these newly
/// formed groups of data points.

/// A container for points
class Space {
public:
	Space();
	Space(int dimension);
	Space(int dimension, list<Coordinate *> pointList);
	~Space();

	/// Add a coordinate specifying a data point
	int AddPoint(Coordinate * point);
	/// Add a list of coordinates specifying multiple data points.
	int AddPointList(list<Coordinate *> pointList);
	/// Add a list of coordinates, from a CSV file, specifying multiple data points.
	int AddPointList(char * filename);
	/// Clear the space of all data points
	int ClearPoints();
	/// Clear the space of all data points and delete the
	/// data points from memory.
	int DeletePoints();
	/// Retrieve the list of points.
	list<Coordinate *> GetPoints();

	/// Normalize the data to [0,1]
	int Normalize();
	/// Place any point exceeding the [0,1] boundary on the boundary.
	int Boundarize();

	/// Get the dimension of the space
	int GetDimension();
	/// Set the dimension of the space
	int SetDimension(int dimension);

	/// Print out the coordinates in the space;
	void Print();

	/// How many points are in this space.
	int Size();

	/// Does each point have the right number of dimensions?
	bool HasValidPoints();
protected:
	int dimension;
	list<Coordinate *> pointList;
	RNG r;

};

/// The Cluster class, like the Space class, is another container for points.  It, however,
/// maintains an extra point that represents the center of the point space.  It also has an
/// extra method which will shift the cluster center to the calculated center of the
/// point space.

/// A clustering container for points.
class Cluster: public Space {
public:
	Cluster();
	Cluster(Space & space);
	Cluster(Space & space, Coordinate clusterCenter);
	Cluster(Space & space, Coordinate clusterCenter,
			list<Coordinate *> pointList);
	~Cluster();

	int SetClusterCenter(Coordinate & clusterCenter);
	// TODO make this return a copy of cluster center instead of a pointer.
	Coordinate * GetClusterCenter();

	float AverageDistance();
	Coordinate StandardDeviation();
	float Shift();
private:
	Coordinate * clusterCenter;
};

/** \brief A point that can occupy a space.
 *
 * Coordinate contains a vector that represents coordinates in a space.  Coordinate
 * also has several methods that can transform a point or list of points inside of
 * a space.
 */

class Coordinate {
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
	Coordinate(vector<double> coordinateData);
	~Coordinate();

	vector<double> GetCoordinateData();
	int GetDimension();
	void SetCoordinateData(vector<double> coordinateData);
	void Print();
	string ToString();

	int Invert();
	int Boundarize();

	Coordinate Reduced(list<int> dimensions);

	Coordinate & operator=(const Coordinate & rhs);
	Coordinate operator+(const Coordinate & rhs);
	Coordinate operator-(const Coordinate & rhs);
	Coordinate operator*(const Coordinate & rhs);
	Coordinate operator/(const Coordinate & rhs);
	bool operator<(const Coordinate & rhs);
	bool operator>(const Coordinate &rhs);
	bool operator==(const Coordinate & rhs);
	bool operator!=(const Coordinate & rhs);

	static double Distance(Coordinate & coordinateOne,
			Coordinate & coordinateTwo);

	static Coordinate Abs(Coordinate & coordinate);
	static Coordinate Average(list<Coordinate *> coordList, int dimension);
	static Coordinate Random(int dimension, RNG & r);
	static Coordinate Singular(double value, int dimension);
	static Coordinate SquareRoot(Coordinate & coordinate);
	static Coordinate StandardDeviation(list<Coordinate *> & coordList,
			Coordinate & coordinate);

protected:
	vector<double> * coordinateData;
};

