#include "featureselection.hxx"

/** \brief The hill climbing feature selection method.
  *
  * The hill climbing method searches for the feature subset that most
  * optimally clusters the feature data.  This can be achieved by
  * starting with an empty subset, and then adding features, one
  * at a time, and evaluating the subset to see which added features
  * result in the optimal feature subset.  Conversely, the search can
  * be performed by starting out with the complete set of features,
  * removing features, one at a time, and evaluating the subset to
  * see which removed features result in the optimal feature subset.
  */
class HillClimbing : public FeatureSelection
{
public:
		/** \brief Initialize the feature selection algorithm.
		  *
		  * The algorithm can face in a particular direction as it
		  * runs.  In other words, the initial feature subset can
		  * either start out as empty or as the complete of features.
		  * If the direction is set to FORWARD, we start out with
		  * an empty subset and add features one at a time.  If the
		  * direction is BACKWARD, we start out with a complete
		  * subset and remove features one at a time.
		  *
		  * \param direction The algorithm can face either FORWARD
		  * or BACKWARD.
		  * \return -1 if the direction specified is invalid.
		  */
	int Initialize( int direction );

		/** \brief Run through the feature space.
		  *
		  * Use Step() to search for the best feature subset.
		  *
		  * \return The resulting optimal feature subset.
		  */
	FeatureResults Run();

		/** \brief Take one step in a direction through the feature space.
		  *
		  * For the hill climbin algorithm, the only valid direction
		  * is FORWARD.
		  *
		  * \param direction The algorithm can only move FORWARD.
		  * \return The resulting optimal feature subset for the
		  * particular iteration.
		  */
	FeatureResults Step( int direction );
protected:
		// The direction being faced when running through the feature space.
	int facing;
		// Moves forward while facing forward.
	FeatureResults ForwardSelect();
		// Moves forward while facing backward.
	FeatureResults BackwardEliminate();
};

