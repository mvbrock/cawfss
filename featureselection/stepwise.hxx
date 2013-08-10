#include "hillclimbing.hxx"

/** \brief The stepwise feature selection method.
  *
  * 
  */
class Stepwise : public HillClimbing
{
public:
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
	void SetBacktrack( int max, double threshold );
protected:
		// The direction being faced when running through the feature space.
	int facing;
		// The maximum number of steps the algorithm can move backwards
		// through the feature space.
	int backtrackMax;
		// The evaluation delta required to move backwards.
	double backtrackThreshold;
		// Moves forward while facing forward.
//	FeatureResults ForwardSelect();
		// Moves forward while facing backward.
//	FeatureResults BackwardEliminate();
};

