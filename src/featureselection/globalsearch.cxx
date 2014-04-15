#include <fstream>
#include "globalsearch.hxx"

GlobalSearch::GlobalSearch( string filename )
{
	this->outputFilename = filename;
	this->isFinished = false;
}

int GlobalSearch::Initialize( unsigned int featureSubsetSize )
{
	this->featureSubsetSize = featureSubsetSize;
	currentCombination = new int[ featureSubsetSize ];
	for( int i = 0; i < featureSubsetSize; i++ )
		currentCombination[ i ] = i;
}

FeatureResults GlobalSearch::Run()
{
	ofstream outFile;
	bool bOutputFileOpen = true;
	if( this->outputFilename == "" )
		bOutputFileOpen = false;
	else
		outFile.open( this->outputFilename.c_str() );

	list<FeatureResults> evaluationList;
	while( this->isFinished != true )
	{
		FeatureResults evaluation = this->Step();
		//evaluation.Print();
		evaluationList.push_back( evaluation );
		if( bOutputFileOpen == true )
			outFile << evaluation.GetResultsString() << endl;

	}
	
	if( bOutputFileOpen == true )
		outFile.close();

	evaluationList.sort();
	FeatureResults finalResult;
	if( evaluationList.size() != 0 )
		finalResult = evaluationList.front();
	return finalResult;
}

FeatureResults GlobalSearch::Step( int direction )
{
	FeatureResults result;
	switch( direction )
	{
		case FORWARD:
			result = StepForward();
			break;
		default:
			cerr<<"GlobalSearch::Step(): Invalid step direction."<<endl;
			cerr<<" - Valid directions are: FORWARD."<<endl;
			return result;
	}
	return result;
}

FeatureResults GlobalSearch::StepForward()
{
	FeatureResults result;
	currentFeatureSubset.clear();
	for( int i = 0; i < this->featureSubsetSize; i++ )
		currentFeatureSubset.push_back( currentCombination[ i ] );
	result = Evaluate( currentFeatureSubset );
	this->isFinished = NextCombination( currentCombination, this->featureSubsetSize, data->GetFeatureCount() );
	return result;
}

bool GlobalSearch::NextCombination(int * comb, int k, int n)
{
	int i = k - 1;
	++comb[i];
	while ((i >= 0) && (comb[i] >= n - k + 1 + i))
	{
		--i;
		++comb[i];
	}

	if (comb[0] > n - k) // Combination (n-k, n-k+1, ..., n) reached
		return true; // No more combinations can be generated

	// comb now looks like (..., x, n, n, n, ..., n).
	// Turn it into (..., x, x + 1, x + 2, ...)
	for (i = i + 1; i < k; ++i)
		comb[i] = comb[i - 1] + 1;

	return false;
}

