#include <iostream>
#include "cluster.hxx"

//////////////////////////////////////////////////////////////
// Cluster Class
//////////////////////////////////////////////////////////////


Cluster::Cluster()
{
	isCluster = true;
}

Cluster::Cluster( Space & space )
{
	isCluster = true;
	SetDimension( space.GetDimension() );
}

Cluster::Cluster( Space & space, Coordinate clusterCenter )
{
	isCluster = true;
	SetDimension( space.GetDimension() );
	SetClusterCenter( clusterCenter );
}

Cluster::Cluster( Space & space, Coordinate clusterCenter, list<Coordinate *> & pointList  )
{
	isCluster = true;
	SetDimension( space.GetDimension() );
	AddPointList( pointList );
}

Cluster::~Cluster()
{
}

int Cluster::SetClusterCenter( Coordinate & clusterCenter )
{
	this->clusterCenter = clusterCenter;
}

Coordinate & Cluster::GetClusterCenter()
{
	return this->clusterCenter;
}

float Cluster::AverageDistance()
{
	float averageDistance = 0;
	list<Coordinate *>::iterator coordIter = pointList.begin();
	while( coordIter != pointList.end() )
	{
		averageDistance += 
			Coordinate::Distance(
				clusterCenter,
				*(*coordIter)
			);
		coordIter++;
	}
	averageDistance /= pointList.size();
	return averageDistance;
}

double Cluster::StandardDeviation()
{
	return Coordinate::StandardDeviation( pointList, clusterCenter );
}


float Cluster::Shift()
{
	Coordinate averageCoordinate;
	float shiftDistance;

	if( pointList.size() != 0 )
	{
		averageCoordinate = Coordinate::Average(pointList, dimension);
	}
	else
	{
		averageCoordinate = Coordinate::Random(dimension, r);
	}
	shiftDistance = Coordinate::Distance(clusterCenter, averageCoordinate);
	this->clusterCenter = averageCoordinate;
	return shiftDistance;
}
