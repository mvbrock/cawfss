/////////////////////////////
// K-Means Library
// Author: Matt Brock
// Date: 08-28-2007
// Last Updated: 09-25-2007
/////////////////////////////

#ifndef CLUSTER_HXX
#define CLUSTER_HXX

#include <list>
#include "space.hxx"
#include "coordinate.hxx"

using namespace std;

/// The Cluster class, like the Space class, is another container for points.  It, however,
/// maintains an extra point that represents the center of the point space.  It also has an
/// extra method which will shift the cluster center to the calculated center of the
/// point space.

/// A clustering container for points.
class Cluster : public Space
{
public:
	Cluster();
	Cluster( Space & space );
	Cluster( Space & space, Coordinate clusterCenter );
	Cluster( Space & space, Coordinate clusterCenter, list<Coordinate *> & pointList );
	virtual ~Cluster();

	int SetClusterCenter( Coordinate & clusterCenter );
	Coordinate & GetClusterCenter();

	float AverageDistance();
	double StandardDeviation();
	float Shift();
private:
	Coordinate clusterCenter;
};

#endif
