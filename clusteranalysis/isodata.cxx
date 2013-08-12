/////////////////////////////
// K-Means Library
// Author: Matt Brock
// Date: 08-28-2007
// Last Updated: 09-25-2007
/////////////////////////////

#include <math.h>

#include "isodata.hxx"

IsoData::IsoData(int kMin, int kMax, int minPoints, float terminationThreshold,
		float mergeThreshold, float splitThreshold, Space * space) {
	this->clusterCount = (kMin + kMax) / 2;
	this->kMin = kMin;
	this->kMax = kMax;
	this->terminationThreshold = terminationThreshold;
	this->minPoints = minPoints;
	this->mergeThreshold = mergeThreshold;
	this->splitThreshold = splitThreshold;
	SetSpace(space);
	AddClusterList(this->clusterCount);
}

int IsoData::Run() {
	// If we don't have any dimensions, then we don't have any points!
	if (space->GetDimension() == 0)
		return 0;
	Coordinate terminationVector = Coordinate::Singular(terminationThreshold,
			space->GetDimension());
	Coordinate distortionDelta = Coordinate::Singular(terminationThreshold + 1,
			space->GetDimension());
	// Since we're working in normalized space, the greatest shift
	// that can take place is the greatest distance between two
	// points in this space.
	float shift = maxDistance;
	// While the average shift of the cluster centers is less than
	// the shift threshold, continue to regroup and shift the
	// clusters and cluster centers.

	Coordinate stdDevCur = Coordinate::Singular(maxDistance,
			space->GetDimension());
	Coordinate stdDevPrev = Coordinate::Singular(maxDistance,
			space->GetDimension());
	do {
		stdDevCur = Group();
		Eliminate();
		Coordinate stdDev2 = Group();
		if (clusterList.size() > kMax)
			Merge();
		if (clusterList.size() < kMin)
			Split();
		Shift();
		if (stdDevCur != Coordinate::Singular(0, space->GetDimension())) {
			distortionDelta = (stdDevPrev - stdDevCur) / stdDevCur;
			distortionDelta = Coordinate::Abs(distortionDelta);
		}
		stdDevPrev = stdDevCur;
	} while (distortionDelta > terminationVector);

	return 0;
}

int IsoData::GetClusterCount() {
	return this->clusterCount;
}

int IsoData::Eliminate() {
	list<Cluster *>::iterator clusterIter = clusterList.begin();
	while (clusterIter != clusterList.end()) {
		if ((*clusterIter)->Size() < minPoints) {
			clusterList.erase(clusterIter);
			// For some reason an if-else causes a glibc
			// crash here, so we can just decrement the
			// iterator when we erase, causing the iterator
			// to stay where it originally was when we
			// increment it below.
			clusterIter--;
		}
		clusterIter++;
	}
}

int IsoData::Merge() {
	list<Cluster *>::iterator firstIter = clusterList.begin();

	while (firstIter != clusterList.end()) {
		list<Cluster *>::iterator secondIter = clusterList.begin();
		while (secondIter != clusterList.end()) {
			Coordinate firstCoord = *(*firstIter)->GetClusterCenter();
			Coordinate secondCoord = *(*secondIter)->GetClusterCenter();
			if (Coordinate::Distance(firstCoord, secondCoord)
					< mergeThreshold) {
				// Create our new cluster containing all of the points
				// from the two clusters.
				int dimension = (*firstIter)->GetDimension();
				Cluster * averageCoord = new Cluster();
				averageCoord->SetDimension(dimension);
				averageCoord->AddPointList((*firstIter)->GetPoints());
				averageCoord->AddPointList((*secondIter)->GetPoints());
				this->clusterList.push_back(averageCoord);
				this->clusterList.erase(firstIter);
				this->clusterList.erase(secondIter);
			}
			secondIter++;
		}
		firstIter++;
	}
}

int IsoData::Split() {
	Coordinate thresholdVector = Coordinate::Singular(splitThreshold,
			space->GetDimension());
	list<Cluster *>::iterator clusterIter = clusterList.begin();
	while (clusterIter != clusterList.end()) {
		float averageDistance = (*clusterIter)->AverageDistance();
		Coordinate stdDev = (*clusterIter)->StandardDeviation();
		if (thresholdVector < stdDev) {
			Coordinate * clusterCenter = (*clusterIter)->GetClusterCenter();
			stdDev = stdDev / Coordinate::Singular(2, space->GetDimension());
			Coordinate clusterCenterOne = *clusterCenter - stdDev;
			Coordinate clusterCenterTwo = *clusterCenter + stdDev;
			clusterCenterOne.Boundarize();
			clusterCenterTwo.Boundarize();
			Cluster * clusterOne = new Cluster();
			Cluster * clusterTwo = new Cluster();
			clusterOne->SetClusterCenter(clusterCenterOne);
			clusterTwo->SetClusterCenter(clusterCenterTwo);
			clusterOne->SetDimension(space->GetDimension());
			clusterTwo->SetDimension(space->GetDimension());
			clusterList.push_front(clusterOne);
			clusterList.push_front(clusterTwo);
			this->clusterList.erase(clusterIter);
			clusterIter--;
			Group();
		}
		clusterIter++;
	}
}

