#include <fstream>
#include <assert.h>
#include "stepwise.hxx"

Stepwise::Stepwise( string outputFilename )
	: HillClimbing( outputFilename )
{
}

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
	ofstream outFile;
	bool bOutputFileOpen = true;
	if( this->outputFilename == "" )
		bOutputFileOpen = false;
	else
		outFile.open( this->outputFilename.c_str() );

	int numFeatures = data->GetFeatureList().size();
	unsigned int level = 0;
	unsigned int backtrackCount = 0; 
	FeatureResults prevEvaluation;
	
	bestResults.insert( bestResults.begin(), numFeatures, FeatureResults() );
	do
	{		
		FeatureResults evaluation = Step(FORWARD);
		if( evaluation.IsInitialized() == true )
			bestResults[evaluation.GetFeatureSubsetSize()] = evaluation;
		if( level > 1 && prevEvaluation < evaluation && backtrackCount < backtrackMax )
		{
			Step(BACKWARD);
			Step(BACKWARD);
			level -= 2;
			backtrackCount++;
		}
		else
		{
			level++;
			backtrackCount = 0;
		}	
		prevEvaluation = evaluation;		
	}
	while( level < numFeatures && cacheExhausted == false );

	for( int i = bestResults.size()-1; i > 0 ; i-- )
	{
		
		if( bOutputFileOpen == true )
		{
			outFile << bestResults[i].GetResultsString() << endl;
		}
	}


	if( bOutputFileOpen == true )
		outFile.close();
	
	return prevEvaluation;
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

void Stepwise::SetBacktrack( unsigned int max, double threshold )
{
	this->backtrackMax = max;
	this->backtrackThreshold = threshold;
}

