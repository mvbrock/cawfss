#include <iostream>
#include <sstream>
#include "coordinate.hxx"
#include "refcntr.hxx"

//////////////////////////////////////////////////////////////
// Coordinate Class
//////////////////////////////////////////////////////////////


Coordinate::Coordinate()
{
}

Coordinate::Coordinate( vector<double> & coordinateData, unsigned int id )
{
	this->coordinateData = coordinateData;
	this->coordinateId = id;
}

Coordinate::~Coordinate()
{
}

vector<double> & Coordinate::GetCoordinateData()
{
	return coordinateData;
}

void Coordinate::SetCoordinateData( vector<double> & coordinateData )
{
	this->coordinateData = coordinateData;
}


int Coordinate::GetDimension()
{
	return coordinateData.size();
}

void Coordinate::Print()
{
	cout<<this->coordinateId<<":";
	for( int i = 0; i < this->coordinateData.size(); i++ )
		cout<<this->coordinateData[i]<<", ";
	cout<<endl;
}

string Coordinate::ToString()
{
	ostringstream outs;
	outs<<this->coordinateId<<":";
	for( int i = 0; i < this->coordinateData.size(); i++ )
	{
		outs<<this->coordinateData[i];
		if( i != this->coordinateData.size() - 1 )
			outs<<", ";
	}
	return outs.str();
}

int Coordinate::Invert()
{
	for( int i = 0; i < this->coordinateData.size(); i++ )
	{
			double value = this->coordinateData[i];
			if( value != 0 )
			{
				value = pow(value, -1);
				this->coordinateData[i] = value;
			}
	}
}

int Coordinate::Boundarize()
{
	for(int i = 0; i < coordinateData.size(); i++)
	{
		if( coordinateData[i] <= 0 )
			coordinateData[i] = 0.001;
			// TODO, create a > operator!
		if( 1 <= coordinateData[i] )
			coordinateData[i] = 0.999;
	}
}


Coordinate Coordinate::Reduced( list<int> & dimensions )
{
	dimensions.sort();
	vector<double> reducedCoordinateData;
	list<int>::iterator iter = dimensions.begin();
	for(int i = 0; i < coordinateData.size(); i++)
	{
		if( i == (*iter) )
		{
			reducedCoordinateData.push_back( coordinateData[i] );
			iter++;
		}
	}
	return Coordinate( reducedCoordinateData, this->coordinateId );
}


Coordinate & Coordinate::operator=(const Coordinate & rhs)
{
	this->coordinateData = rhs.coordinateData;
	return *this;
}

Coordinate Coordinate::operator+(const Coordinate & rhs)
{
	Coordinate coordinate;
	if( this->coordinateData.size() != rhs.coordinateData.size() )
	{
		cerr<<"Coordinate::operator + : Mismatched dimensions."<<endl;
		return coordinate;
	}
	for( int i = 0; i < this->coordinateData.size(); i++ )
		coordinate.coordinateData.push_back( this->coordinateData[i] + rhs.coordinateData[i] );
	return coordinate;
}

Coordinate Coordinate::operator-(const Coordinate & rhs)
{
	Coordinate coordinate;
	if( coordinateData.size() != rhs.coordinateData.size() )
	{
		cerr<<"Coordinate::operator - : Mismatched dimensions."<<endl;
		return coordinate;
	}
	
	for( int i = 0; i < rhs.coordinateData.size(); i++ )
		coordinate.coordinateData.push_back( coordinateData[i] - rhs.coordinateData[i] );

	return coordinate;
}

Coordinate Coordinate::operator*(const Coordinate & rhs)
{
	Coordinate coordinate;
	if( coordinateData.size() != rhs.coordinateData.size() )
	{
		cerr<<"Coordinate::operator * : Mismatched dimensions. "
			<<coordinateData.size()<<" : "<<rhs.coordinateData.size()<<endl;
		return coordinate;
	}

	for( int i = 0; i < rhs.coordinateData.size(); i++ )
		coordinate.coordinateData.push_back( coordinateData[i] * rhs.coordinateData[i] );
	
	return coordinate;
}

Coordinate Coordinate::operator/(const Coordinate & rhs)
{
	Coordinate coordinate;
	if( coordinateData.size() != rhs.coordinateData.size() )
	{
		cerr<<"Coordinate::operator / : Mismatched dimensions."<<endl;
		return coordinate;
	}

	for( int i = 0; i < rhs.coordinateData.size(); i++ )
	{
		if( rhs.coordinateData[i] == 0 )
		{
			cerr<<"Coordinate::operator / : Divide-by-Zero."<<endl;
			coordinate = Coordinate::Singular(0,rhs.coordinateData.size());
			return coordinate;//Coordinate::Singular(0,rhs.coordinateData.size());
		}
		coordinate.coordinateData.push_back( coordinateData[i] / rhs.coordinateData[i] );
	}
	return coordinate;
}

bool Coordinate::operator<( const Coordinate &rhs )
{
	vector<double> rhsCoordinateData = rhs.coordinateData;
	if( coordinateData.size() != rhs.coordinateData.size() )
	{
		cerr<<"Coordinate::operator<: Mismatched dimensions."<<endl;
		return false;
	}

	for(int i = 0; i < rhsCoordinateData.size(); i++)
	{
		if (coordinateData[i] >= rhsCoordinateData[i] )
			return false;
	}
	return true;
}

bool Coordinate::operator>( const Coordinate &rhs )
{
	vector<double> rhsCoordinateData = rhs.coordinateData;
	if( coordinateData.size() != rhs.coordinateData.size() )
	{
		cerr<<"Coordinate::operator<: Mismatched dimensions."<<endl;
		return false;
	}

	for(int i = 0; i < rhsCoordinateData.size(); i++)
	{
		if (coordinateData[i] <= rhsCoordinateData[i] )
			return false;
	}
	return true;
}


bool Coordinate::operator==( const Coordinate & rhs )
{
	vector<double> rhsCoordinateData = rhs.coordinateData;
	if( coordinateData.size() != rhs.coordinateData.size() )
	{
		cerr<<"Coordinate::operator<: Mismatched dimensions."<<endl;
		return false;
	}

	for(int i = 0; i < rhsCoordinateData.size(); i++)
	{
		if (coordinateData[i] != rhsCoordinateData[i] )
			return false;
	}
	return true;
}

bool Coordinate::operator!=( const Coordinate & rhs )
{
	vector<double> rhsCoordinateData = rhs.coordinateData;
	if( coordinateData.size() != rhs.coordinateData.size() )
	{
		cerr<<"Coordinate::operator<: Mismatched dimensions."<<endl;
		return false;
	}

	for(int i = 0; i < rhsCoordinateData.size(); i++)
	{
		if (coordinateData[i] == rhsCoordinateData[i] )
			return false;
	}
	return true;
}


double Coordinate::Distance( Coordinate & coordinateOne, Coordinate & coordinateTwo )
{
	double distanceSquared = 0;

	if( coordinateOne.GetDimension() != coordinateTwo.GetDimension() )
	{
		cerr<<"Coordinate::Distance(): Mismatched dimensions. "
			<<coordinateOne.GetDimension()<<" : "<<coordinateTwo.GetDimension()<<endl;
		return -1.0f;
	}
	Coordinate vectorDistanceSquared;

	vectorDistanceSquared = ( coordinateTwo - coordinateOne ) * ( coordinateTwo - coordinateOne );

	vector<double> vectorDistanceSquaredData = vectorDistanceSquared.GetCoordinateData();
	for( int i = 0 ; i < vectorDistanceSquaredData.size(); i++ )
		distanceSquared += vectorDistanceSquaredData[i];
	
	return sqrt( distanceSquared );
}

Coordinate Coordinate::Abs( Coordinate & coordinate )
{
	int dimension = coordinate.GetDimension();
	vector<double> coordinateData = coordinate.GetCoordinateData();
	for( int i = 0; i < dimension; i++ )
		coordinateData[i] = abs( coordinateData[i] );
	Coordinate newCoordinate( coordinateData, coordinate.coordinateId );
	return newCoordinate;

}

Coordinate Coordinate::Average(list<Coordinate *> coordList, int dimension)
{
	list<Coordinate *>::iterator iter = coordList.begin();

		// Create a Coordinate object to total up all of the coordinates
		// in the coordList list.
	Coordinate total = Coordinate::Singular( 0, dimension );
	if( coordList.size() == 0 )
		return total;

	while( iter != coordList.end() )
	{
		total = total + *(*iter);
		iter++;
	}

	total = total / Coordinate::Singular( coordList.size(), dimension );
	return total;
}

Coordinate Coordinate::Random( int dimension, RNG & r )
{
	vector<double> coordinateData( dimension );
	r.uniform(coordinateData);
	return Coordinate( coordinateData, 0 );
}

Coordinate Coordinate::Singular( double value, int dimension )
{
	vector<double> coordinateData;
	for( int i = 0; i < dimension; i++ )
		coordinateData.push_back( value );
	Coordinate coordinate( coordinateData, -1 );
	return coordinate;
}


Coordinate Coordinate::SquareRoot( Coordinate & coordinate )
{
	vector<double> coordinateData = coordinate.GetCoordinateData();
	for( int i = 0; i < coordinate.GetDimension(); i++ )
	{
		coordinateData[i] = sqrt( coordinateData[i] );
	}
	Coordinate newCoordinate( coordinateData, -1 );
	return newCoordinate;
}

double Coordinate::StandardDeviation( list<Coordinate *> & coordList, Coordinate & coordinate )
{
	int dimension = coordinate.GetDimension();

	if( coordList.size() == 0 )
		return 0;
	
	list<Coordinate *>::iterator iter = coordList.begin();

	double total = 0;

	while( iter != coordList.end() )
	{
		double dist = Coordinate::Distance(*(*iter), coordinate);
		dist = dist * dist;
		total = total + dist;
		iter++;
	}
	
	total = (total / coordList.size());
	return sqrt( total );
}

