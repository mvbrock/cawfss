#ifndef GLOBALSEARCH_HXX
#define GLOBALSEARCH_HXX

#include "featureselection.hxx"

class GlobalSearch : public FeatureSelection
{
public:
	GlobalSearch( string filename );
	
	int Initialize( unsigned int featureSubsetSize );

	FeatureResults Run();

	FeatureResults Step( int direction = FORWARD );

protected:
	FeatureResults StepForward();

	bool NextCombination(int * comb, int k, int n);

	string outputFilename;
	
	unsigned int featureSubsetSize;
	
	int * currentCombination;
	
	bool isFinished;

};
#endif

