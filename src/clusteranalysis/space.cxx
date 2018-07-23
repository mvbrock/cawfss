/////////////////////////////
// K-Means Library
// Author: Matt Brock
// Date: 08-28-2007
// Last Updated: 09-25-2007
/////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "space.hxx"
#include "refcntr.hxx"


//////////////////////////////////////////////////////////////
// Space Class
//////////////////////////////////////////////////////////////

Space::Space()
{
	isCluster = false;
	SetDimension( 0 );
}

Space::Space( int dimension )
{
	isCluster = false;
	SetDimension( dimension );
}

Space::Space( int dimension, list<Coordinate *> & pointList )
{
	isCluster = false;
	SetDimension( dimension );
	AddPointList( pointList );
}

Space::~Space()
{
	if( isCluster == false )
	{
		DeletePoints();
	}
}

bool Space::AddPoint( Coordinate * point )
{
	pointList.push_back( point );
	return true;
}

bool Space::AddPointList( list<Coordinate *> & pointList )
{
	list<Coordinate *>::iterator iter = pointList.begin();
	if( this->pointList.size() == 0 )
		this->dimension = (*iter)->GetDimension();
	else
		this->dimension = (*this->pointList.begin())->GetDimension();
	while( iter != pointList.end() )
	{
		this->pointList.push_back( *iter );
		// Check for dimension inequalities within the coordinates.
		if( this->dimension != (*iter)->GetDimension() )
		{
			cerr << "AddPointList(): Mistmatched dimensions: " << dimension << ", " << (*iter)->GetDimension() << endl;
			dimension = 0;
			return false;
		}
		else
			this->dimension = (*iter)->GetDimension();
		iter++;
	}
	return true;
}

int AddPointList( char * filename )
{
	ifstream f( filename, ifstream::in );
	streambuf sb();
	while( !f.eof() )
	{
		
	}
	f.close();
}

int Space::ClearPoints()
{
	pointList.clear();
}

int Space::DeletePoints()
{
	for( int i = pointList.size(); i > 0; i-- )
	{
		Coordinate * coordinate = pointList.front();
		pointList.pop_front();
		delete coordinate;
	}
}

list<Coordinate *> & Space::GetPoints()
{
	return pointList;
}

int Space::Normalize()
{
	list<Coordinate *>::iterator iter = pointList.begin();
	vector<double> greatestCoordinateData = (*iter)->GetCoordinateData();
	// Iterate through all of the points
	while( iter != pointList.end() )
	{
		Coordinate * coordinate = (*iter);
		// Create a vector which will represent our 
		vector<double> coordinateData = coordinate->GetCoordinateData();
		for(int i = 0; i < coordinateData.size(); i++)
		{
			if( greatestCoordinateData[i] < coordinateData[i] )
				greatestCoordinateData[i] = coordinateData[i];
		}
		iter++;
	}
	Coordinate greatest( greatestCoordinateData, -1 );
	greatest.Invert();
	iter = pointList.begin();
	while( iter != pointList.end() )
	{
		Coordinate * coordinate = (*iter);
		*coordinate = *coordinate * greatest;
		iter++;
	}
}


int Space::GetDimension()
{
	return this->dimension;
}

int Space::SetDimension( int dimension )
{
	this->dimension = dimension;
}

void Space::Print()
{
	list<Coordinate *>::iterator iter = pointList.begin();
	while( iter != pointList.end() )
	{
		(*iter)->Print();
		iter++;
	}
}

int Space::Size()
{
	return pointList.size();
}

bool Space::HasValidPoints()
{
	list<Coordinate *>::iterator iter = pointList.begin();
	while( iter != pointList.end() )
	{
		if( (*iter)->GetDimension() != this->GetDimension() )
			return false;
		iter++;
	}
	return true;
}


