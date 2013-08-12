/////////////////////////////
// K-Means Library
// Author: Matt Brock
// Date: 08-28-2007
// Last Updated: 09-25-2007
/////////////////////////////

#include <math.h>

#include "kmeans.hxx"

//////////////////////////////////////////////////////////////
// KMeans Class
//////////////////////////////////////////////////////////////

KMeans::KMeans() {
	terminationThreshold = 0;
	maxDistance = 0;
}

KMeans::KMeans(int k, double terminationThreshold, Space * space) {
	this->terminationThreshold = terminationThreshold;
	SetSpace(space);
	AddClusterList(k);
}

KMeans::~KMeans() {
}

int KMeans::AddClusterList(int numClusters) {
	// Create a number of randomly placed clusters within the
	// space and push them onto the cluster list.
	for (int i = 0; i < numClusters; i++) {
		Cluster * cluster = new Cluster(*space,
				Coordinate::Random(space->GetDimension(), rng));
		clusterList.push_back(cluster);
	}
}

int KMeans::DeleteClusters() {
	// Clear the cluster list while deleting each cluster object.
	for (int i = clusterList.size(); i > 0; i--) {
		Cluster * cluster = clusterList.front();
		clusterList.pop_front();
		delete cluster;
	}
}

int KMeans::ClusterSize() {
	// Return how many clusters exist.
	return clusterList.size();
}

list<Cluster *> KMeans::GetClusters() {
	// Return the list of clusters.
	return clusterList;
}

int KMeans::SetSpace(Space * space) {
	// Set the space for the K-Means algorithm to operate in.
	this->space = space;
	Coordinate p1 = Coordinate::Singular(0, space->GetDimension());
	Coordinate p2 = Coordinate::Singular(1, space->GetDimension());
	this->maxDistance = Coordinate::Distance(p1, p2);
}

int KMeans::Run() {
	// If we don't have any dimensions, then we don't have any points!
	if (space->GetDimension() == 0)
		return 0;
	Coordinate terminationVector = Coordinate::Singular(terminationThreshold,
			space->GetDimension());
	Coordinate distortionDelta = Coordinate::Singular(terminationThreshold + 1,
			space->GetDimension());
	while (terminationVector < distortionDelta) {
		Coordinate stdDev1 = Group();
		Shift();
		Coordinate stdDev2 = Group();
		Shift();
		distortionDelta = (stdDev2 - stdDev1);		///stdDev1;

		//distortionDelta = Coordinate::Abs( distortionDelta );
	}
}

double KMeans::Evaluate() {
	list<Cluster *>::iterator clusterIter = clusterList.begin();
	int clusterListSize = clusterList.size();
	double totalAverageDistance = 0;
	double averageDistance;
	while (clusterIter != clusterList.end()) {
		double clusterAverageDistance = 0;
		list<Coordinate *> coordList = (*clusterIter)->GetPoints();
		if (coordList.size() != 0) {
			list<Coordinate *>::iterator coordIter = coordList.begin();
			while (coordIter != coordList.end()) {
				clusterAverageDistance += Coordinate::Distance(
						*(*clusterIter)->GetClusterCenter(), *(*coordIter));
				coordIter++;
			}
			clusterAverageDistance /= coordList.size();
		} else {
			clusterListSize--;
			clusterAverageDistance = 0;
		}
		totalAverageDistance += clusterAverageDistance;
		clusterIter++;
	}
	averageDistance = totalAverageDistance / clusterListSize;

	return averageDistance / maxDistance;
}

Coordinate KMeans::Group() {
	Coordinate standardDevAverage = Coordinate::Singular(0,
			this->space->GetDimension());

	list<Cluster *>::iterator clusterIter = this->clusterList.begin();
	while (clusterIter != this->clusterList.end()) {
		(*clusterIter)->ClearPoints();
		clusterIter++;
	}

	list<Coordinate *> pointList = this->space->GetPoints();
	list<Coordinate *>::iterator coordIter = pointList.begin();
	// Iterate through the list of points in space.
	while (coordIter != pointList.end()) {
		clusterIter = clusterList.begin();
		Cluster * closestCluster = (*clusterIter);

		// Set our best distance to be the maximum possible
		// distance between two points in the space.
		double bestDistance = this->maxDistance;
		double curDistance;
		// Iterate through the list of clusters, looking
		// for the cluster center closest to the point.
		while (clusterIter != clusterList.end()) {
			curDistance = Coordinate::Distance(*(*coordIter),
					*(*clusterIter)->GetClusterCenter());
			if (curDistance < bestDistance) {
				closestCluster = (*clusterIter);
				bestDistance = curDistance;
			}
			clusterIter++;
		}
		// Add the point to the cluster with the closest
		// center.
		closestCluster->AddPoint((*coordIter));
		coordIter++;
	}

	clusterIter = this->clusterList.begin();
	while (clusterIter != this->clusterList.end()) {
		list<Coordinate *> clusterList = (*clusterIter)->GetPoints();

		standardDevAverage = standardDevAverage
				+ Coordinate::StandardDeviation(clusterList,
						*(*clusterIter)->GetClusterCenter());

		clusterIter++;
	}
	standardDevAverage = standardDevAverage
			/ Coordinate::Singular(this->clusterList.size(),
					this->space->GetDimension());
	return standardDevAverage;
}

double KMeans::Shift() {
	if (clusterList.size() == 0) {
		return 0;
	}
	list<Cluster *>::iterator clusterIter = clusterList.begin();
	double totalShift = 0;
	while (clusterIter != clusterList.end()) {
		totalShift += (*clusterIter)->Shift();
		clusterIter++;
	}
	return totalShift / clusterList.size();
}

