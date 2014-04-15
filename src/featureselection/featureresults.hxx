#ifndef FEATURERESULTS_HXX
#define FEATURERESULTS_HXX

#include <list>

using namespace std;

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
	FeatureResults( double within, double between, int clusters, list<int> featureSubset );

		/** \brief Return whether or not the results were cached.
		 * 
		 * \return Whether or not the results were cached.
		 */
	bool GetIsCached();

		/** \brief Return the number of clusters.
		  *
		  * \return The number of clusters.
		  */
	unsigned int GetClusters();
	
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
	
		/** \brief Return the size of the feature subset.
		 * 
		 * \return The size of the feature subset.
		 */
	unsigned int GetFeatureSubsetSize();

		/** \brief Return the results as a one-line string
		  *
		  * \return The one-line string containing the results.
		  */
	string GetResultsString();


		/** \brief Print the results. */
	void Print();
	
		/** \brief Specify whether or not the results were cached.
		 * 
		 * \param isCached Whether or not the results were cached.
		 */
	void SetIsCached( bool isCached );
	
		/** \brief Specify the number of clusters.
		  *
		  * \param clusters The number of clusters.
		  */
	void SetClusters( int clusters );

		/** \brief Specify the evaluation of the feature subset.
		  *
		  * \param evaluation The evaluation of the feature subset.
		  */
	void SetWithinBetween( double within, double between );

		/** \brief Specify the feature subset.
		  *
		  * \param featureSubset The list of feature indices.
		  */
	void SetFeatureSubset( list<int> featureSubset );
	
		/** \brief Return whether or not the results have been initialized.
		 * 
		 * \return Whether or not the results have been initialized.
		 */
	bool IsInitialized();
	
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
		// The within measure of the feature subset.
	double within;
		// The between measure of the feature subset.
	double between;
		// The number of clusters associated with the evaluation.
	unsigned int clusters;
		// The list of feature indices.
	list<int> featureSubset;
		// Whether or not the results have been initialized
	bool resultsInitialized;
		// Whether or not the results were cached
	bool resultsCached;
};

#endif

