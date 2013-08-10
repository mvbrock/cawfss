#include "featureselection.hxx"

/** \brief The evolutionary selection method
  *
  * This algorithm uses a genetic algorithm to initialize a randomly generated
  * population, evaluate each member of the population, and use the best
  * members of that population to spawn a new population.  This cycle repeats
  * until the evaluation criteria is met.
  *
  */

class Evolutionary : public FeatureSelection
{
public:
	Evolutionary();
	~Evolutionary();
	
		/** \brief Initialize a random population of size
		  *  populationSize
		  *
		  * The genetic algorithm needs a random population to begin
		  * refining in order to find the most optimal population
		  * member.
		  *
		  * \param selectionPercentage The best percentage of the
		  *		population to use for spawning the next
		  *		generation.
		  * \param populationSize The size of the population to
		  *		initialize
		  */
	void Initialize(	double mutationCoefficient,
				double selectionPercentage,
				double terminationCondition,
				int populationSize );

		/** \brief Iterate through the population generations
		  *
		  * Search for the best feature subset by iterating
		  * through the population generations, refining each
		  * population by evaluating and selecting the best
		  * members, and then using those best members to form
		  * a new population.
		  *
		  * \return The results from the best feature subset.
		  */
	FeatureResults Run();

		/** \brief Take one iteration throug the population generations.
		  * 
		  * \param direction The direction in which to step through
		  * the feature space.
		  */
	FeatureResults Step( int direction = FORWARD );


//protected:
		// Combines two members into one and randomly eliminate
		// features from the member until it is of a randomly
		// chosen size between subsetSizeMin and subsetSizeMax.
	list<int> Procreate( list<int> memberOne, list<int> memberTwo );

		// Generates a random list of integers within
		// FeatureSelection::subsetMin and FeatureSelection:subsetMax.

	list<int> GenerateRandomMember();

		// Randomly remove or add features to the member based on our
		// mutation parameter.
	void MutateMember( list<int> & member );

		// The current list of feature subsets, represented as
		// members with the population.
	vector< list<int> > currentPopulation;
	
	double mutationCoefficient;
	double selectionPercentage;
	double terminationCondition;
	int populationSize;

	RNG r;
};
