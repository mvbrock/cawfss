#include <fstream>

#include <assert.h>
#include "rng.h"
#include "evolutionary.hxx"

using namespace std;

Evolutionary::Evolutionary( string outputFilename )
{
	this->outputFilename = outputFilename;
}

Evolutionary::~Evolutionary()
{
}


void Evolutionary::Initialize( 	double mutationCoefficient,
				double selectionPercentage,
				double terminationCondition,
				int maxIterations,
				int populationSize )
{
	this->mutationCoefficient = mutationCoefficient;
	this->selectionPercentage = selectionPercentage;
	this->terminationCondition = terminationCondition;
	this->maxIterations = maxIterations;
	this->populationSize = populationSize;

	for(int i = 0; i < populationSize; i++)
	{
		list<int> member = GenerateRandomMember();
		currentPopulation.push_back( member );
	}
}


FeatureResults Evolutionary::Run()
{
	FeatureResults result;
	
	ofstream outFile;
	bool bOutputFileOpen = true;
	if( this->outputFilename == "" )
		bOutputFileOpen = false;
	else
		outFile.open( this->outputFilename.c_str() );
	
	int iterations = 0;	
	if( this->maxIterations == 0 )
		return result;
	do
	{
		result = this->Step();
		if( bOutputFileOpen == true )
			outFile << result.GetResultsString() << endl;
		++iterations;
	}
	while( result.GetEvaluation() > this->terminationCondition && iterations < this->maxIterations);

	if( bOutputFileOpen == true )
		outFile.close();

	return result;
}


FeatureResults Evolutionary::Step( int direction )
{
	int survivingPopulationSize = (int)(currentPopulation.size() * selectionPercentage);
	list<FeatureResults> populationEvaluation;
	vector< list<int> >::iterator memberIter = currentPopulation.begin();
	
	// Evaluate each member of the population
	while( memberIter != currentPopulation.end() )
	{
		FeatureResults result;
		result = this->Evaluate( (*memberIter) );
		populationEvaluation.push_back( result );
		memberIter++;
	}
	
	// Sort the evaluated populations and remove the worst-evaluated members.
	populationEvaluation.sort();
	for(int i = 0; i < currentPopulation.size() - survivingPopulationSize; i++)
	{
		populationEvaluation.pop_back();
	}

	currentPopulation.clear();

		// Convert the list to a vector so we can grab random members
		// and procreate.
		// TODO: Find a better way to do this so we don't have to
		// create copies of the surviving population.
	vector< list<int> > survivingMembers;
	list<FeatureResults>::iterator resultIter = populationEvaluation.begin();
	while( resultIter != populationEvaluation.end() )
	{
		survivingMembers.push_back( (*resultIter).GetFeatureSubset() );
		resultIter++;
	}
	
	RNG r;
	for(int i = 0; i < this->populationSize; i++)
	{
		int randIndexOne = r.rand_int() % survivingPopulationSize;
		int randIndexTwo;
		do
		{
			randIndexTwo = r.rand_int() % survivingPopulationSize;
		}
		while( randIndexTwo == randIndexOne );
		
		list<int> newMember = this->Procreate(
					survivingMembers[ randIndexOne ],
					survivingMembers[ randIndexTwo ] );
		this->MutateMember( newMember );
		currentPopulation.push_back( newMember );
	}
	
	return populationEvaluation.front();
}

list<int> Evolutionary::GenerateRandomMember()
{
	list<int> member;
	map<int,bool> selectedFeatures;

	if( data->GetFeatureCount() != 0 )
	{
			// The size of our member will be between the
			// subsetSizeMax and subsetSizeMin.
		int subsetSize = ( r.rand_int() % ( subsetSizeMax -
					subsetSizeMin + 1 ) ) + subsetSizeMin;
		for(int i = 0; i < subsetSize; i++)
		{
			int memberValue;
				// Loop until we find a member value that
				// hasn't been selected for the member yet.
			do
			{
				memberValue = r.rand_int() %
						data->GetFeatureCount();
			} while( selectedFeatures.find( memberValue ) != selectedFeatures.end() );
			
				// Push back the member value and indicate
				// that it has been used.
			member.push_back( memberValue );
			selectedFeatures[ memberValue ] = true;
		}
	}
	assert(member.size() >= subsetSizeMin && member.size() <= subsetSizeMax);
	return member;
}

list<int> Evolutionary::Procreate( list<int> memberOne, list<int> memberTwo )
{
		// Merge the two lists together
	list<int> member = memberOne;
	member.merge( memberTwo );
		
		// Remove any duplicate features.
	map<int,bool> uniqueFeatures;
	list<int>::iterator memberIter = member.begin();
	while( memberIter != member.end() )
	{
		uniqueFeatures[ (*memberIter) ] = true;
		memberIter++;
	}
	member.clear();

	map<int,bool>::iterator mapIter = uniqueFeatures.begin();
	while( mapIter != uniqueFeatures.end() )
	{
		member.push_back( (*mapIter).first );
		mapIter++;
	}
		// Sort the list of unique features.
	member.sort();
	
		// Determine the size of the combined subset
	int subsetSize = ( r.rand_int() % ( subsetSizeMax -
				subsetSizeMin + 1 ) ) + subsetSizeMin;
		// The subset size determines how many features we will
		// remove from the combined members.
	int removalCount = member.size() - subsetSize;
	int numRemoved = 0;
	memberIter = member.begin();
	while( numRemoved < removalCount && removalCount > 0 && member.size() > 0 )
	{
			// Flip a coin
		if( r.uniform() < 0.50 )
		{
			numRemoved++;
			memberIter = member.erase( memberIter );
		}
		if( memberIter != member.end() )
			memberIter++;
		if( memberIter == member.end() )
			memberIter = member.begin();
	}
	assert(member.size() >= subsetSizeMin && member.size() <= subsetSizeMax);
	return member;
}

void Evolutionary::MutateMember( list<int> & member )
{
		// Decide whether or not to mutate the member.
	if( r.uniform() > mutationCoefficient )
		return;

		// For now the probability of an add or a removal
		// is statically set at 0.5
	bool addFeature = r.rand_int() % 2;
	bool removeFeature = r.rand_int() % 2;

		// Map the member's features.	
	map<int,bool> memberFeatures;
	list<int>::iterator memberIter = member.begin();
	while( memberIter != member.end() )
	{
		memberFeatures[ (*memberIter) ] = true;
		memberIter++;
	}

		// If we need to add a feature and the size of the list is less than the maximum size.
	if( addFeature == true && member.size() < this->subsetSizeMax )
	{
		int featureToAdd;
			// Generate random indices until we get a feature
			// that doesn't exist in the member.
		do
		{
			featureToAdd = r.rand_int() %
						data->GetFeatureCount();
		} while( memberFeatures[ featureToAdd ] == true );
			// Add this feature.
		member.push_back( featureToAdd );
	}
		// If we need to remove a feature and the size of the list is greater than the minimum size.
	if( removeFeature == true && member.size() > this->subsetSizeMin )
	{
		int featureToRemove;
			// Generate random indices until we get a feature
			// that exists in the member.
			// MORE EFFICIENT: RANDOMLY CHOOSE ONE OF THE
			// MEMBER FEATURES VIA AN INDEX
		do
		{
			featureToRemove = r.rand_int() %
						data->GetFeatureCount();
		} while( memberFeatures[ featureToRemove ] != true );
		
		memberIter = member.begin();
			// Search through the member features until we find
			// that feature to remove.
		while( memberIter != member.end() )
		{
			if( (*memberIter) == featureToRemove )
			{
				member.erase( memberIter );
				break;
			}
			memberIter++;
		}
	}
	assert(member.size() >= subsetSizeMin && member.size() <= subsetSizeMax);
		// Sort at the end.
	member.sort();
}
