#include <iostream>
#include <fstream>

#include "featuredata.hxx"
#include "clusterdata.hxx"

using namespace std;

/////////////////////////////////////
// Feature Data Class
/////////////////////////////////////

FeatureData::FeatureData()
{
}

FeatureData::FeatureData( const string filename )
{
	LoadFeatureData( filename );
}

FeatureData::~FeatureData()
{
}

int FeatureData::GetFeatureCount()
{
	return featureList.size();
}

vector<char *> FeatureData::GetFeatureList()
{
	return featureList;
}

Space * FeatureData::CreateSpace( list<int> featureSubset )
{
	list<Coordinate *> pointList;
	list<Coordinate *>::iterator iter = data.begin();
	while( iter != data.end() )
	{
		Coordinate * coordinate = new Coordinate();
		*coordinate = (*iter)->Reduced( featureSubset );
		pointList.push_back( coordinate );
		iter++;
	}
	Space * space = new Space( featureSubset.size(), pointList );
	if( space->HasValidPoints() == false )
	{
		cerr<<"FeatureData::CreateSpace: Invalid points in space"<<endl;
	}
	return space;
}

int FeatureData::LoadFeatureData( const string filename )
{
	ClusterData::LoadFeatureData( filename.c_str(), data, featureList );
	return 0;
}
