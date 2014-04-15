#include <fstream>
#include <algorithm>

#include "hillclimbing.hxx"

HillClimbing::HillClimbing( string outputFilename )
{
	this->outputFilename = outputFilename;
}

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
	ofstream outFile;
	bool bOutputFileOpen = true;
	if( this->outputFilename == "" )
		bOutputFileOpen = false;
	else
		outFile.open( this->outputFilename.c_str() );

	int numFeatures = data->GetFeatureList().size();
	list<FeatureResults> evaluationList;
	for( int i = 0; i < numFeatures; i++ )
	{
		FeatureResults evaluation = this->Step( FORWARD );
		evaluationList.push_back( evaluation );
		if( bOutputFileOpen == true )
			outFile << evaluation.GetResultsString() << endl;
	}
	evaluationList.sort();
	while( evaluationList.front().GetFeatureSubset().size() > subsetSizeMax ||
		evaluationList.front().GetFeatureSubset().size() < subsetSizeMin )
	{
		evaluationList.pop_front();
	}
	
	if( bOutputFileOpen == true )
		outFile.close();
	
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
	for(int i = 0; i < data->GetFeatureList().size(); i++)
	{
		if( find(possibleFeatureSubset.begin(),possibleFeatureSubset.end(), i) ==
				possibleFeatureSubset.end() )
			featureSubsetCandidateList.push_back( i );			
	}

	FeatureResults bestEvaluation;
	FeatureResults currentEvaluation;
	
	if( subsetSize == data->GetFeatureList().size() - 1 )
	{
		bestEvaluation = Evaluate( possibleFeatureSubset );
		return bestEvaluation;
	}
	
		// The feature that was added to the subset to see if the
		// subset achieved a better evaluation.
	int addedFeatureIndex;
	int bestFeatureIndex = featureSubsetCandidateList.front();
	
	for(int i = 0; i < featureSubsetCandidateList.size(); i++)
	{
		addedFeatureIndex = featureSubsetCandidateList.front();
		featureSubsetCandidateList.pop_front();

		possibleFeatureSubset.push_front( addedFeatureIndex );
		
		FeatureResults evaluatedResults = Evaluate( possibleFeatureSubset );
		
		currentEvaluation = evaluatedResults;

		if( currentEvaluation < bestEvaluation && currentEvaluation.GetIsCached() == false )
		{
			bestEvaluation = currentEvaluation;
			bestFeatureIndex = addedFeatureIndex;
		}
		possibleFeatureSubset.pop_front();
	}
	currentFeatureSubset.push_front( bestFeatureIndex );
	return bestEvaluation;
}

FeatureResults HillClimbing::BackwardEliminate()
{
	list<int> possibleFeatureSubset = currentFeatureSubset;
	int subsetSize = possibleFeatureSubset.size();
	
		// If our feature subset size is 1, there is no need
		// to evaluate a 0 sized subset, so just zero out
		// our results and return them.
	if( subsetSize == 1 )
	{
		return Evaluate( possibleFeatureSubset );
	}
	
	int poppedFeatureIndex;
	int worstFeatureIndex = possibleFeatureSubset.front();
	
	FeatureResults bestEval;
	
	for(int i = 0; i < subsetSize; i++)
	{
		poppedFeatureIndex = possibleFeatureSubset.front();
		possibleFeatureSubset.pop_front();

		FeatureResults currentEval = Evaluate( possibleFeatureSubset );
		
		if( currentEval < bestEval  && currentEval.GetIsCached() == false )
		{
			bestEval = currentEval;
			worstFeatureIndex = poppedFeatureIndex;
		}
		possibleFeatureSubset.push_back( poppedFeatureIndex );
	}
	currentFeatureSubset.remove( worstFeatureIndex );
	return bestEval;
}

