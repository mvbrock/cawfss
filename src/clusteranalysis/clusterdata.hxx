#ifndef CLUSTERDATA_HXX
#define CLUSTERDATA_HXX

#include <list>

#include "coordinate.hxx"

using namespace std;

class ClusterData
{
public:
	ClusterData() { }
	static bool LoadFeatureData(	const string & filename,
									list<Coordinate *> & data,
									vector<char *> & featureList,
									bool loadFeatureList = true );
};

#endif
