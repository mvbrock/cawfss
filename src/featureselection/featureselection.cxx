#include <sstream>
#include "featureselection.hxx"


/////////////////////////////////////
// Feature Selection Class
/////////////////////////////////////

FeatureSelection::FeatureSelection()
{
	data = new FeatureData();
	clusteringMethod = KMEANS;
	subsetSizeMin = 0;
	subsetSizeMax = 0;
	kRangeMin = 0;
	kRangeMax = 0;
	terminationThreshold = 0.0f;
	minPointsInCluster = 0;
	mergeThreshold = 0.0f;
	splitThreshold = 0.0f;
	currentEvaluation = 1.0f;
	subsequentCacheAccessCounter = 0;
	cacheExhausted = false;
}

FeatureSelection::~FeatureSelection()
{
	delete data;
}

FeatureResults FeatureSelection::Evaluate( list<int> possibleFeatureSubset )
{
	FeatureResults results;
	
    // Sort the feature subset and convert it to a string.
	possibleFeatureSubset.sort();
	string featureListString = FeatureListToString(possibleFeatureSubset);
    // Search for the results in the cache, otherwise, evaluate the possible feature subset.
	map< string, FeatureResults >::iterator resultsCacheIter = resultsCache.find( featureListString );
	if( resultsCacheIter != resultsCache.end() )
	{
	    // If so, retrieve the results from the cache
		results = (*resultsCacheIter).second;
		subsequentCacheAccessCounter++;
	    // If the subsequent cache access counter exceeds the size of the cache, 
		if( subsequentCacheAccessCounter > resultsCache.size() )
			cacheExhausted = true;
		results.SetIsCached( true );
		return results;
	}
	else
	{
		subsequentCacheAccessCounter = 0;
		cacheExhausted = false;
	}

	Space * space = data->CreateSpace( possibleFeatureSubset );

	space->Normalize();
	
	double within = 0.0f;
	double between = 0.0f;			

	switch( clusteringMethod )
	{
		case KMEANS:
		{
			FeatureResults bestEvaluation;
			FeatureResults currentEvaluation;
			int bestK = kRangeMin;

			for(int j = kRangeMin; j <= kRangeMax; j++)
			{
				// Retrieve the evaluation (within/between)
				KMeans kmeans( j, terminationThreshold, space );
				kmeans.Run();
				kmeans.Evaluate(within,between);
				
				// Set the current evaluation.
				currentEvaluation.SetClusters( bestK );
				currentEvaluation.SetWithinBetween( within, between );
				currentEvaluation.SetFeatureSubset( possibleFeatureSubset );
				
				if( currentEvaluation < bestEvaluation )
				{
					bestEvaluation = currentEvaluation;
					bestK = j;
				}
			}
			break;
		}
		case ISODATA:
		{
			IsoData isodata(	string(""),
								this->kRangeMin,
								this->kRangeMax,
								this->minPointsInCluster,
								this->terminationThreshold,
								this->maxIterations,
								this->mergeThreshold,
								this->splitThreshold,
								space );
			isodata.Run();
			isodata.Evaluate(within,between);
			if(within/between < 0 )
			{
				cout<<"WTF"<<endl;
			}
			results.SetClusters( isodata.GetClusterCount() );
			results.SetWithinBetween( within, between );
			results.SetFeatureSubset( possibleFeatureSubset );
			break;
		}
		case PYRAMIDKMEANS:
			break;
		case PYRAMIDISODATA:
			break;
		default:
			break;
	}
	
	delete space;
	resultsCache[ featureListString ] = results;
	return results;
}

void FeatureSelection::LoadFeatureData( const string filename )
{
	data->LoadFeatureData( filename );
}

void FeatureSelection::LoadFeatureData( FeatureData * data )
{
	this->data = data;
}

void FeatureSelection::SetClusteringMethod( int clusteringMethod )
{
	this->clusteringMethod = clusteringMethod;
}

void FeatureSelection::SetFeatureSubsetRange( int subsetSizeMin, int subsetSizeMax )
{
	this->subsetSizeMin = subsetSizeMin;
	this->subsetSizeMax = subsetSizeMax;
}

void FeatureSelection::SetKRange( int kRangeMin, int kRangeMax )
{
	this->kRangeMin = kRangeMin;
	this->kRangeMax = kRangeMax;
}

void FeatureSelection::SetTerminationThreshold( double terminationThreshold )
{
	this->terminationThreshold = terminationThreshold;
}

void FeatureSelection::SetMaxIterations( unsigned int maxIterations )
{
	this->maxIterations = maxIterations;
}
	
void FeatureSelection::SetIsoDataParams( int minPointsInCluster, double mergeThreshold, double splitThreshold )
{
	this->minPointsInCluster = minPointsInCluster;
	this->mergeThreshold = mergeThreshold;
	this->splitThreshold = splitThreshold;
}

string FeatureSelection::FeatureListToString( list<int> & featureList )
{
	ostringstream featureListStream;
	list<int>::iterator featureListIter;
	for( featureListIter = featureList.begin(); featureListIter != featureList.end(); ++featureListIter )
		featureListStream << (*featureListIter) << ":";
	return featureListStream.str();
}


