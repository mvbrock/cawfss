//////////////////////////////
// K-Means Library
// Author: Matt Brock
// Date: 08-28-2007
// Last Updated: 09-25-2007
//////////////////////////////

#ifndef KMEANS_HXX
#define KMEANS_HXX

#include "space.hxx"
#include "cluster.hxx"
#include "coordinate.hxx"

/// The K-Means class.
/// The programmer will use this class to run K-Means on an already initialized
/// Space object containing data points and cluster centers in a specified
/// dimension.  The K-Means class will specify some criteria for when a cluster
/// center has been appropriately relocated and also when to merge similar
/// cluster centers.

/// A basic class for the K-Means algorithm.
class KMeans
{
public:
	KMeans();
	KMeans( int k, double terminationThreshold, Space * space );
	virtual ~KMeans();

		/// Add a number of randomly placed clusters to the list.
	int AddClusterList( int numClusters );
		/// Return the number of clusters in this space.
	int ClusterSize();
		/// Clear the space of all cluster center points
	int DeleteClusters();
		/// Get the list of clusters
	list<Cluster *> & GetClusters();

		/// Set the space for the K-Means algorithm to operate in.
	int SetSpace( Space * space );

		/// Run the Group(), Shift(), and Merge() functions until the
		/// shifting threshold has been met.
	int Run();
		/// Return the average distance from the points in space to
		/// their respective clusters.
	void Evaluate( double & within, double & between );


protected:
		/// Place the points into the nearest cluster and return the
		/// standard deviation of the points using the cluster center
		/// as the mean.
	double Group();
		/// Shift the cluster center to the average of all of its
		/// points.
	double Shift();

		/// The random number generator
	RNG rng;
	Coordinate * boundaryA;
	Coordinate * boundaryB;
	Space *space;
	list<Cluster *> clusterList;
	double terminationThreshold;
	double maxDistance;

};

#endif
