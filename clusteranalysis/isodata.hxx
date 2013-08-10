/////////////////////////////
// K-Means Library
// Author: Matt Brock
// Date: 08-28-2007
// Last Updated: 09-25-2007
/////////////////////////////

#include "kmeans.hxx"

/** \brief ISODATA is an extension of the K-Means algorithm.
  *
  * The IsoData class.
  * The programmer will use this class to run the IsoData algorithm on an already initialized
  * Space object containing data points and cluster centers in a specified
  * dimension.  The IsoData class will specify some criteria for when a cluster
  * center has been appropriately relocated and also when to perform the eliminate,
  * merge, and split operations.
  */
class IsoData : public KMeans
{
public:
	IsoData( int kMin, int kMax, int minPoints, float shiftThreshold, float mergeThreshold,
			float splitThreshold, Space * space );
	
		/** \brief Run the ISODATA algorithm.
		  *
		  * Run the Group(), Shift(), Eliminate(), Merge(), and
		  * Split(), functions until the shifting threshold has
		  * been met.
		  */
	int Run();

		/** \brief Return the current cluster count.
		  *
		  * Return the current cluster count.  This value should be
		  * within kMin and kMax.
		  *
		  * \return The current cluster count.
		  */
	int GetClusterCount();
protected:
		// Eliminate clusters that have less than minPoints points
	int Eliminate();
		// Merge clusters whose distance between eachother is lower
		// than the mergeThreshold.
	int Merge();
		// Split a cluster whose standard deviation exceeds the
		// splitThreshold.
	int Split();

	int clusterCount;
	int kMin;
	int kMax;
	int minPoints;
	float mergeThreshold;
	float splitThreshold;
};

