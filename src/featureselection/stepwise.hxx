#include <vector>
#include <map>
#include "hillclimbing.hxx"

/** \brief The stepwise feature selection method.
  *
  * 
  */
class Stepwise : public HillClimbing
{
public:

	Stepwise( string outputFilename );
		/** \brief 
		  *
		  */
	int Initialize( int direction );

		/** \brief
		  *
		  */
	FeatureResults Run();

		/** \brief
		  *
		  */
	FeatureResults Step( int direction );
	
		/** \brief
		  *
		  */
	void SetBacktrack( unsigned int max, double threshold );
protected:
		// The direction being faced when running through the feature space.
	int facing;
		// The maximum number of steps the algorithm can move backwards
		// through the feature space.
	unsigned int backtrackMax;
		// The evaluation delta required to move backwards.
	double backtrackThreshold;
		// The cache for already traversed nodes.
	vector< map<string, FeatureResults> > resultsCache;
		// The vector of best results for each level.
	vector< FeatureResults > bestResults;

};

