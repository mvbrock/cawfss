/////////////////////////////
// K-Means Library
// Author: Matt Brock
// Date: 08-28-2007
// Last Updated: 09-25-2007
/////////////////////////////

#include <iostream>
#include <fstream>
#include <cmath>

#include "isodata.hxx"


IsoData::IsoData(	string outputFilename,
			unsigned int kMin,
			unsigned int kMax,
			unsigned int minPoints,
			double terminationThreshold,
			unsigned int maxIterations,
			double mergeThreshold,
			double splitThreshold,
			Space * space )
{
	
	this->eliminateCount = 0;
	this->mergeCount = 0;
	this->splitCount = 0;
	
	this->outputFilename = outputFilename;
	this->kMin = kMin;
	this->kMax = kMax;
	this->terminationThreshold = terminationThreshold;
	this->maxIterations = maxIterations;	
	this->minPoints = minPoints;
	this->mergeThreshold = mergeThreshold;
	this->splitThreshold = splitThreshold;
	
	SetSpace( space );
	AddClusterList( (kMin + kMax) / 2 );
}

IsoData::~IsoData()
{
}

int IsoData::Run()
{
		// If we don't have any dimensions, then we don't have any points!
	if( space->GetDimension() == 0 )
		return 0;

	ofstream outFile;
	
	bool bOutputToFile = true;
	if( this->outputFilename == "" )
		bOutputToFile = false;
	else
		outFile.open( this->outputFilename.c_str() );
	
	int currentIterations = 0;
	double distortionDelta = terminationThreshold;
	double stdDevCur = maxDistance;
	double stdDevPrev = maxDistance;
	do {
		stdDevCur = Group();
		
		Merge();
		Split();
		Eliminate();
		if( bOutputToFile == true )
		{
			double within;
			double between;
			Evaluate(within,between);
			outFile	<< GetEliminateCount() << ", "
				<< GetMergeCount() << ", "
				<< GetSplitCount() << ", "
				<< within << ", "
				<< between << ", "
				<< distortionDelta << ", "
				<< GetClusterCount() << endl;
		}		
		distortionDelta = abs(stdDevPrev - stdDevCur)/stdDevPrev;
		stdDevPrev = stdDevCur;
		currentIterations++;
	}
	while(	distortionDelta > terminationThreshold &&
		( currentIterations < this->maxIterations || this->maxIterations == 0 ) );
	
	if( bOutputToFile == true )
		outFile.close();

	return 0;
}


int IsoData::GetClusterCount()
{
	return clusterList.size();
}

int IsoData::Eliminate()
{
	list<Cluster *>::iterator clusterIter = clusterList.begin();
	
	while( clusterIter != clusterList.end() )
	{
		if( (*clusterIter)->Size() < minPoints &&
			clusterList.size() > kMin )
		{
			Cluster * cluster = (*clusterIter);
			delete cluster;
			clusterList.erase( clusterIter );
			clusterIter--;
			eliminateCount++;
		}
		clusterIter++;
	}
	Group();
}

int IsoData::Merge()
{
	list<Cluster *>::iterator firstIter = clusterList.begin();
	
	Coordinate firstCoord;
	Coordinate secondCoord;
	
	while( firstIter != clusterList.end() )
	{
		list<Cluster *>::iterator secondIter = clusterList.begin();
		while( secondIter != clusterList.end() )
		{
			firstCoord = (*firstIter)->GetClusterCenter();
			secondCoord = (*secondIter)->GetClusterCenter();
			if( 	Coordinate::Distance( firstCoord, secondCoord ) < mergeThreshold &&
				firstIter != secondIter &&
				clusterList.size() > kMin )
			{
					// Create our new cluster containing all of the points
					// from the two clusters.
				int dimension = (*firstIter)->GetDimension();
				
				Coordinate averageCoord = (firstCoord+secondCoord)/Coordinate::Singular(2,dimension);
				
				Cluster * averageCluster = new Cluster();
				
				averageCluster->SetClusterCenter( averageCoord );
				averageCluster->SetDimension( dimension );
				
				this->clusterList.push_front( averageCluster );
				
				Cluster * cluster1 = (*firstIter);
				Cluster * cluster2 = (*secondIter);
				delete cluster1;
				delete cluster2;
				firstIter = this->clusterList.erase( firstIter );
				secondIter = this->clusterList.erase( secondIter );
				firstIter--;
				secondIter--;
				mergeCount++;
			}
			secondIter++;
		}
		firstIter++;
	}
	Group();
}

int IsoData::Split()
{
	list<Cluster *>::iterator clusterIter = clusterList.begin();

	while( clusterIter != clusterList.end() )
	{
		double stdDev = (*clusterIter)->StandardDeviation();
		if( 	stdDev > splitThreshold &&
			clusterList.size() < kMax )
		{
			Coordinate clusterCenter = (*clusterIter)->GetClusterCenter();
			stdDev = stdDev / 2;
			
			Coordinate clusterCenterOne = clusterCenter - Coordinate::Singular( stdDev, space->GetDimension());
			Coordinate clusterCenterTwo = clusterCenter + Coordinate::Singular( stdDev, space->GetDimension());
			
			clusterCenterOne.Boundarize();
			clusterCenterTwo.Boundarize();
			
			Cluster * clusterOne = new Cluster();
			Cluster * clusterTwo = new Cluster();
			
			clusterOne->SetClusterCenter( clusterCenterOne );
			clusterTwo->SetClusterCenter( clusterCenterTwo );
			clusterOne->SetDimension( space->GetDimension() );
			clusterTwo->SetDimension( space->GetDimension() );
			
			clusterList.push_front( clusterOne );
			clusterList.push_front( clusterTwo );
			
			Cluster * cluster = (*clusterIter);
			delete cluster;
			this->clusterList.erase( clusterIter );
			clusterIter--;
			splitCount++;
		}
		clusterIter++;
	}
	Group();
}

