#include "hillclimbing.hxx"

int HillClimbing::Initialize( int direction )
{
	int featureListSize = data->GetFeatureList().size();
	if( featureListSize == 0 )
		cout<<"Warning: No features exist in the data."<<endl;

	facing = direction;

	switch( facing )
	{
		case BACKWARD:
			for( int i = 0; i < featureListSize; i++ )
				currentFeatureSubset.push_back( i );
			break;
		case FORWARD:
			break;
		default:
			cerr<<"HillClimbing::Initialize(): Invalid facing direction."<<endl;
			cerr<<" - Valid directions are: FORWARD and BACKWARD."<<endl;
			return -1;
	}
	return 0;
}

FeatureResults HillClimbing::Run()
{
	int numFeatures = data->GetFeatureList().size();
	list<FeatureResults> evaluationList;
	for( int i = 0; i < numFeatures; i++ )
	{
		FeatureResults evaluation = this->Step( FORWARD );
		evaluationList.push_back( evaluation );
	}
	evaluationList.sort();
	while( evaluationList.front().GetFeatureSubset().size() > subsetSizeMax ||
		evaluationList.front().GetFeatureSubset().size() < subsetSizeMin )
	{
		evaluationList.pop_front();
	}
	return evaluationList.front();
}

FeatureResults HillClimbing::Step( int direction )
{
	
	FeatureResults result;
	switch( direction )
	{
		case FORWARD:
			switch( facing )
			{
				case FORWARD:
					result = ForwardSelect();
					break;
				case BACKWARD:
					result = BackwardEliminate();
					break;
				default:
					cerr<<"HillClimbing::Step(): Invalid facing direction."<<endl;
					cerr<<" - Valid directions are: FORWARD and BACKWARD."<<endl;
			}
			break;
		default:
			cerr<<"HillClimbing::Step(): Invalid step direction."<<endl;
			cerr<<" - Valid directions are: FORWARD."<<endl;
			return result;
	}
	return result;
}

FeatureResults HillClimbing::ForwardSelect()
{
	list<int> possibleFeatureSubset = currentFeatureSubset;
	int subsetSize = possibleFeatureSubset.size();

		// Create a list of feature subset candidates, aka
		// features that aren't already in the current
		// feature subset.
	list<int> featureSubsetCandidateList;
	list<int>::iterator iter = possibleFeatureSubset.begin();
	cout<<"Feature Subset Candidate list: ";
	for(int i = 0; i < data->GetFeatureList().size(); i++)
	{
		if( i == (*iter) )
			iter++;
		else
		{
			cout<<i<<", ";
			featureSubsetCandidateList.push_back( i );
		}
	}
	cout<<endl;

	FeatureResults bestEvaluation;
	FeatureResults currentEvaluation;
	
	if( subsetSize == data->GetFeatureList().size() - 1 )
	{
		bestEvaluation.SetClusters( 0 );
		bestEvaluation.SetEvaluation( 0 );
		bestEvaluation.SetFeatureSubset( currentFeatureSubset );
		return bestEvaluation;
	}
	
		// The feature that was added to the subset to see if the
		// subset achieved a better evaluation.
	int addedFeatureIndex;
	int bestFeatureIndex = featureSubsetCandidateList.front();
	
	for(int i = 0; i < subsetSize; i++)
	{
		addedFeatureIndex = featureSubsetCandidateList.front();
		featureSubsetCandidateList.pop_front();

		possibleFeatureSubset.push_front( addedFeatureIndex );
		
		FeatureResults evaluatedResults = Evaluate( possibleFeatureSubset );
		
		currentEvaluation = evaluatedResults;

		if( currentEvaluation < bestEvaluation )
		{
			bestEvaluation = currentEvaluation;
			bestFeatureIndex = addedFeatureIndex;
		}
		possibleFeatureSubset.pop_front();
	}
	possibleFeatureSubset.push_front( addedFeatureIndex );
	return bestEvaluation;
}

FeatureResults HillClimbing::BackwardEliminate()
{
	list<int> possibleFeatureSubset = currentFeatureSubset;
	int subsetSize = possibleFeatureSubset.size();

	FeatureResults bestEvaluation;
	FeatureResults currentEvaluation;
	
		// If our feature subset size is 1, there is no need
		// to evaluate a 0 sized subset, so just zero out
		// our results and return them.
	if( subsetSize == 1 )
	{
		bestEvaluation.SetClusters( 0 );
		bestEvaluation.SetEvaluation( 0 );
		bestEvaluation.SetFeatureSubset( currentFeatureSubset );
		return bestEvaluation;
	}
	
	int poppedFeatureIndex;
	int worstFeatureIndex = possibleFeatureSubset.front();

	for(int i = 0; i < subsetSize; i++)
	{
		poppedFeatureIndex = possibleFeatureSubset.front();
		possibleFeatureSubset.pop_front();

		FeatureResults evaluatedResults = Evaluate( possibleFeatureSubset );
		currentEvaluation = evaluatedResults;

		if( currentEvaluation < bestEvaluation )
		{
			bestEvaluation = currentEvaluation;
			worstFeatureIndex = poppedFeatureIndex;
		}
		possibleFeatureSubset.push_back( poppedFeatureIndex );
	}
	currentFeatureSubset.remove( worstFeatureIndex );
	return bestEvaluation;
}
