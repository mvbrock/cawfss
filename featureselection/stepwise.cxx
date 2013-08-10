#include "stepwise.hxx"

int Stepwise::Initialize( int direction )
{
	int featureListSize = data->GetFeatureList().size();
	if( featureListSize == 0 )
		cout<<"Warning: No features exist in the data."<<endl;

	this->facing = direction;

	switch( facing )
	{
		case BACKWARD:
			for( int i = 0; i < featureListSize; i++ )
				currentFeatureSubset.push_back( i );
			break;
		case FORWARD:
			break;
		default:
			cerr<<"Stepwise::Initialize(): Invalid facing direction."<<endl;
			cerr<<" - Valid directions are: FORWARD and BACKWARD."<<endl;
			return -1;
	}
	return 0;
}

FeatureResults Stepwise::Run()
{
	int numFeatures = data->GetFeatureList().size();
	list<FeatureResults> evaluationList;
	
		// TODO:
		// Modify this loop to traverse forward and backward
		// through the feature space, recording an evaluation
		// based on some criteria so we can sort through them
		// in the end.
	int i = 0;
		// Initialize with an evaluation delta that doesn't satisfy
		// the backtrace criteria.
	double evalCur = 0;
	double evalPrev = this->backtrackThreshold * 2;
	do {
		FeatureResults evaluation = this->Step( FORWARD );
		evalCur = evaluation.GetEvaluation();
		double evalDelta = evalCur - evalPrev;
		if( evalDelta < 0 )
		{
			evaluationList.push_back( evaluation );
			i++;
		
		} else
		{
			evaluation = this->Step( BACKWARD );
			i--;
		}
		evaluation.Print();
		evalPrev = evalCur;
	} while( i < numFeatures );
	

	evaluationList.sort();
	while( evaluationList.front().GetFeatureSubset().size() > subsetSizeMax ||
		evaluationList.front().GetFeatureSubset().size() < subsetSizeMin )
	{
		evaluationList.pop_front();
	}
	
	return evaluationList.front();
}

FeatureResults Stepwise::Step( int direction )
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
					cerr<<"Stepwise::Step(): Invalid facing direction."<<endl;
					cerr<<" - Valid directions are: FORWARD and BACKWARD."<<endl;
			}
			break;
		case BACKWARD:
			switch( facing )
			{
				case FORWARD:
					result = BackwardEliminate();
					break;
				case BACKWARD:
					result = ForwardSelect();
					break;
				default:
					cerr<<"Stepwise::Step(): Invalid facing direction."<<endl;
					cerr<<" - Valid directions are: FORWARD and BACKWARD."<<endl;
			}
			break;
		default:
			cerr<<"Stepwise::Step(): Invalid step direction."<<endl;
			cerr<<" - Valid directions are: FORWARD and BACKWARD."<<endl;
			return result;
	}
	return result;
}

void Stepwise::SetBacktrack( int max, double threshold )
{
	this->backtrackMax = max;
	this->backtrackThreshold = threshold;
}

/*FeatureResults Stepwise::ForwardSelect()
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

FeatureResults Stepwise::BackwardEliminate()
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
	
		// The feature that was popped from the subset to see
		// if the subset achieved a better evaluation.
	int poppedFeatureIndex;
		// This will be our initial worst feature (aka the
		// feature that contributes to the worst evaluation).
	int worstFeatureIndex = possibleFeatureSubset.front();

	for(int i = 0; i < subsetSize; i++)
	{
			// Remove the front item of our subset and save it
			// so we can add it later.
		poppedFeatureIndex = possibleFeatureSubset.front();
		possibleFeatureSubset.pop_front();

			// Evaluate this subset with the front item removed and
			// record the results.
		FeatureResults evaluatedResults = Evaluate( possibleFeatureSubset );
		currentEvaluation = evaluatedResults;

			// Check to see if these results are better then our best
			// recorded evaluation.
		if( currentEvaluation < bestEvaluation )
		{
				// If so, record the evaluation and the index that was
				// popped, since it obviously hindered the subset in
				// achieving a good evaluation.
			bestEvaluation = currentEvaluation;
			worstFeatureIndex = poppedFeatureIndex;
		}
			// Put the previously popped index back into the subset.
		possibleFeatureSubset.push_back( poppedFeatureIndex );
	}
		// Remove our worst feature...
	currentFeatureSubset.remove( worstFeatureIndex );
		// ...and return our best evaluation.
	return bestEvaluation;
}
*/
