#ifndef FEATURESELECTION_HXX
#define FEATURESELECTION_HXX

#include <iostream>
#include <list>
#include <string>
#include <map>

#include "isodata.hxx"
#include "featuredata.hxx"
#include "featureresults.hxx"

using namespace std;

/** \brief Non-functioning base class for feature selection.
  *
  * This is the base class for the feature selection methods to be
  * implemented.  Currently on the list to be implemented are
  * the hillclimbing method, stepwise selection, evoluationary,
  * and dynamic programming.
  */
class FeatureSelection
{
public: 
		/** \brief Construct the class.
		  *
		  */
	FeatureSelection();

		/** \brief Deconstruct the class.
		  *
		  */
	~FeatureSelection();
 
		/** \brief Load the feature data.
		  *
		  * The feature data is loaded from a comma-separated file
		  * containing the feature names and their values.
		  *
		  * \param filename The filename specifying the CSV file.
		  */
	void LoadFeatureData( const string filename );
	
		/** \brief Load the feature data.
		  *
		  * The feature data is loaded from a pointer to another
		  * feature data object.
		  *
		  * \param data The point to another feature data object.
		  */
	void LoadFeatureData( FeatureData * data );

	
		/** \brief Specify the clustering method.
		  *
		  * \param method Can be KMEANS, ISODATA, PYRAMIDKMEANS,
		  * or PYRAMIDISODATA.
		  */
	void SetClusteringMethod( int clusterMethod );

		/** \brief Set an acceptable feature subset size range.
		  *
		  * \param subsetSizeMin The lower bound on the number of clusters.
		  * \param subsetSizeMax The upper bound on the number of clusters.
		  */
	void SetFeatureSubsetRange( int subsetSizeMin, int subsetSizeMax );

		/** \brief Set an acceptable K range.
		  *
		  *  The K range specifies the range of the number of clusters.
		  *
		  * \param kRangeMin The lower bound on the number of clusters.
		  * \param kRangeMax The upper bound on the number of clusters. 
		  */
	void SetKRange( int kRangeMin, int kRangeMax );
	
		/** \brief Set the termination condition for the clustering algorithm.
		  *
		  *  The clustering termination condition specifies when the clustering will end.
		  *
		  * \param terminationThreshold The termination threshold.
		  */
	void SetTerminationThreshold( double terminationThreshold );

		/** \brief Set the maximum iterations of the clustering algorithm.
		  *
		  * The maximum iterations of the clustering algorithm sets a hard stop for the algorithm in addition
		  * to the termination threshold.
		  *
		  * \param maxIterations The maximum iterations
		  */
	void SetMaxIterations( unsigned int maxIterations );
	
		/** \brief Set the parameters for the ISODATA clustering algorithm.
		  *
		  *  Sets some additional parameters for the ISODATA clustering algorithm.
		  *
		  * \param mergeThreshold The merge threshold.
		  * \param splitThreshold The split threshold.
		  */

	void SetIsoDataParams( int minPointsInCluster, double mergeThreshold, double splitThreshold );

		/** \brief Run through the feature space.
		  *
		  * Search for the best feature subset by running through the
		  * feature space.
		  *
		  * \return The results from the best feature subset.
		  */
	virtual FeatureResults Run() = 0;

		/** \brief Take one step in a direction through the feature
		  * space.
		  * 
		  * \param direction The direction in which to step through
		  * the feature space.
		  */
	virtual FeatureResults Step( int direction = FORWARD ) = 0;

		/** \brief Specifies in which direction to move through the
		  * feature space or which direction face while moving.
		  */
	static enum direction { 
			FORWARD /** Face or move forward. */,
			BACKWARD /** Face or move backward. */
		} direction;

		/** \brief The cluster analysis methods that can be used
		  * to determine the best feature subsets.
		  */
	static enum clustermethod {
			KMEANS /** The basic K-Means method */,
			ISODATA /** The ISODATA method */,
			PYRAMIDKMEANS /** The pyramid resolution enhancement for the K-Means method */,
			PYRAMIDISODATA /** The pyramid resolution enhancement for the ISODATA method */
		} clustermethod;
protected:
		// Evaluate, using one of the cluster analysis methods, how
		// well a feature subset clusters data.
	FeatureResults Evaluate( list<int> possibleFeatureSubset );
	
		// Convert a feature list to a string value for caching purposes.
	string FeatureListToString( list<int> & featureList );
	
		// The clustering method to use for evaluating the feature subset.
	int clusteringMethod;

		// The lower bound to an acceptable feature subset size.
	int subsetSizeMin;
		// The upper bound to an acceptable feature subset size.
	int subsetSizeMax;
	
		// The lower bound to how many clusters we can have.
	int kRangeMin;
		// The upper bound to how many clusters we can have.
	int kRangeMax;
	
	
		// The minimum allowable number of points associated with a cluster center.
	int minPointsInCluster;
		// The termination threshold for the clustering algorithm.
	double terminationThreshold;
		// The maximum number of iterations for the clustering algorithm.
	int maxIterations;
		// The merge threshold for the ISODATA clustering algorithm.
	double mergeThreshold;
		// The split threshold for the ISODATA clustering algorithm.
	double splitThreshold;

		// The current feature subset.
	list<int> currentFeatureSubset;

		// The current feature subset's evaluation.
	double currentEvaluation;

		// The feature data.
	FeatureData * data;
	
		// Maps a list of features to a result, so that the algorithm doesn't
		// have to calculate the results repeatedly for that unique list of features, in addition
		// to not running the cluster analysis algorithm more than once on a set of features, which
		// can create bias towards the lists of features that are clustered more than once (as
		// the cluster analysis algorithms are non-deterministic and have a probability of generating
		// better results, the more times they are run).
	map< string, FeatureResults > resultsCache;
		// Counts the number of times the cache has been accessed without adding any new items to the cache.
	unsigned int subsequentCacheAccessCounter;
		// Indicates whether or not every cache item has been accessed without adding any new items to the cache.
	bool cacheExhausted;
};

#endif
