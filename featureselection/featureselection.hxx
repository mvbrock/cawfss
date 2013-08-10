#include <string>
#include <vector>
#include <list>

#include "clusteranalysis/isodata.hxx"

using namespace std;

class FeatureData;
class FeatureResults;

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
	void LoadFeatureData( char * filename );
	
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
			PYRAMIDKMEANS /** The pyramid resolution enhancement
					for the K-Means method */,
			PYRAMIDISODATA /** The pyramid resolution enhancement
					for the ISODATA method */
		} clustermethod;
protected:
		// Evaluate, using one of the cluster analysis methods, how
		// well a feature subset clusters data.
	FeatureResults Evaluate( list<int> possibleFeatureSubset );
	
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

		// The current feature subset.
	list<int> currentFeatureSubset;

		// The current feature subset's evaluation.
	double currentEvaluation;

		// The feature data.
	FeatureData * data;
};

/** \brief Interfaces with the feature data.
  *
  * FeatureData is used to store the original feature data and create feature
  * spaces from it based on an arbitrary feature subset.
  */
class FeatureData
{
public:
		/// Construct the class
	FeatureData();
		/// Construct the class, loading a file containing comma-seperated values (CSV format) into memory.
	FeatureData( char * filename);
		/// Deconstruct the class.
	~FeatureData();
		/// Return the number of features in the data.
	int GetFeatureCount();
		/// Return the list of feature names in the data.
	vector<char *> GetFeatureList();
		/// Create a new space containing the data points specified by the feature subset.
	Space * CreateSpace( list<int> featureSubset );
		/// Load a file containing comma-seperated values (CSV format) into memory.
	int LoadFeatureData( char * filename );
private:
		// The feature names.
	vector<char *> featureList;
		// The feature data.
	list<Coordinate *> data;
};



/** \brief Expresses the evaluation of a feature subset.
  *
  * FeatureResults is used to store the results from
  * the evaluation of a feature subset.  Two FeatureResults
  * objects can be compared to determine which is the more
  * optimal results.
  */
class FeatureResults
{
public:

		/** \brief Construct the class.
		  *
		  * The default constructor.
		  */
	FeatureResults();

		/** \brief Construct the class.
		  *
		  * Specify the evaluation, number of clusters, and the
		  * feature subset.
		  *
		  * \param evaluation The evaluation of the feature subset.
		  * \param clusters The number of clusters associated with the
		  * evaluation.
		  * \param featureSubset The list of features being evaluated.
		  */ 
	FeatureResults( double evaluation, int clusters, list<int> featureSubset );

		/** \brief Return the number of clusters.
		  *
		  * \return The number of clusters.
		  */
	int GetClusters();
	
		/** \brief Return the evalation of the feature subset.
		  *
		  * \return The evaluation of the feature subset.
		  */
	double GetEvaluation();

		/** \brief Return the feature subset.
		  *
		  * \return The feature subset indices.
		  */
	list<int> GetFeatureSubset();

		/** \brief Print the results. */
	void Print();
	
		/** \brief Specify the number of clusters.
		  *
		  * \param clusters The number of clusters.
		  */
	void SetClusters( int clusters );

		/** \brief Specify the evaluation of the feature subset.
		  *
		  * \param evaluation The evaluation of the feature subset.
		  */
	void SetEvaluation( double evaluation );

		/** \brief Specify the feature subset.
		  *
		  * \param featureSubset The list of feature indices.
		  */
	void SetFeatureSubset( list<int> featureSubset );
	
		/** \brief Assignment operator.
		  *
		  * \param rhs The right hand side element of the assignment.
		  * \return A reference to the object that was assigned.
		  */
	FeatureResults & operator=( FeatureResults rhs );

		/** \brief Comparison operator.
		  *
		  * \param rhs The right hand side element of the comparison.
		  * \return True if the rhs object is greater than this
		  * object, otherwise returns false.
		  */
	bool operator<( FeatureResults &rhs );
private:
		// The evaluation of the feature subset.
	double evaluation;
		// The number of clusters associated with the evaluation.
	int clusters;
		// The list of feature indices.
	list<int> featureSubset;
};
