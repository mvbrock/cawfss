#include <sstream>
#include <iostream>

#include "featureresults.hxx" 

using namespace std;

/////////////////////////////
// Feature Results
/////////////////////////////

FeatureResults::FeatureResults()
	: within( -1.0f ),
	  between( -1.0f ),
	  clusters( 0 ),
	  resultsInitialized( false ),
	  resultsCached( false )
{
}

FeatureResults::FeatureResults( double within, double between, int clusters, list<int> featureSubset )
{
	SetClusters( clusters );
	SetWithinBetween( within, between );
	SetFeatureSubset( featureSubset );
	this->resultsInitialized = true;
}

bool FeatureResults::GetIsCached()
{
	return resultsCached;
}

unsigned int FeatureResults::GetClusters()
{
	return clusters;
}

double FeatureResults::GetEvaluation()
{
	if( this->resultsInitialized == true )
		return this->within/this->between;
	else
		return -1.0f;
}

list<int> FeatureResults::GetFeatureSubset()
{
	return this->featureSubset;
}

unsigned int FeatureResults::GetFeatureSubsetSize()
{
	return this->featureSubset.size();
}

string FeatureResults::GetResultsString()
{
	ostringstream resultStream;
	resultStream << this->within/this->between << ",";
	resultStream << this->within << ",";
	resultStream << this->between << ",";
	resultStream << this->clusters << ",";
	resultStream << this->featureSubset.size() << ",";
	featureSubset.sort();
	list<int>::iterator iter = featureSubset.begin();
	while( iter != featureSubset.end() )
	{
		resultStream << (*iter);
		iter++;
		if( iter != featureSubset.end() )
			resultStream << ":";
	}
	string resultString = resultStream.str();
	return resultString;
}

void FeatureResults::Print()
{
	list<int>::iterator iter;
	cout<<"Clusters: "<<this->clusters<<endl;
	cout<<"Evaluation: "<<this->within/this->between<<endl;
	cout<<"Features: ";
	
	iter = featureSubset.begin();
	while( iter != featureSubset.end() )
	{
		cout<<(*iter)<<", ";
		iter++;
	}
	cout<<endl;
}

void FeatureResults::SetIsCached( bool isCached )
{
	this->resultsCached = isCached;
}

void FeatureResults::SetClusters( int clusters )
{
	this->clusters = clusters;
}

void FeatureResults::SetWithinBetween( double within, double between )
{
	this->between = between;
	this->within = within;
	this->resultsInitialized = true;
}

void FeatureResults::SetFeatureSubset( list<int> featureSubset )
{
	this->featureSubset = featureSubset;
	featureSubset.sort();
}

bool FeatureResults::IsInitialized()
{
	return this->resultsInitialized;
}

FeatureResults & FeatureResults::operator=( FeatureResults rhs )
{
	this->within = rhs.within;
	this->between = rhs.between;
	this->clusters = rhs.clusters;
	this->featureSubset = rhs.featureSubset;
	this->resultsInitialized = rhs.resultsInitialized;
	return *this;
}

bool FeatureResults::operator<( FeatureResults & rhs )
{
	if( this->resultsInitialized == true && rhs.resultsInitialized == true )
		return this->within/this->between < rhs.within/rhs.between;
	else if( this->resultsInitialized == true && rhs.resultsInitialized == false )
		return true;
	else if( this->resultsInitialized == false && rhs.resultsInitialized == true )
		return false;
	else
		return false;
}

