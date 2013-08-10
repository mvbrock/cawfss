#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "featureselection.hxx"


/////////////////////////////////////
// Feature Selection Class
/////////////////////////////////////

FeatureSelection::FeatureSelection()
{
	data = new FeatureData();
	SetClusteringMethod( KMEANS );
	SetKRange( 0, 0 );
}

FeatureSelection::~FeatureSelection()
{
	delete data;
}

FeatureResults FeatureSelection::Evaluate( list<int> possibleFeatureSubset )
{
	FeatureResults results;

	Space * space = data->CreateSpace( possibleFeatureSubset );

	space->Normalize();

	switch( clusteringMethod )
	{
		case KMEANS:
		{
			double bestEvaluation = 1;
			double currentEvaluation = 1;
			int bestK = kRangeMin;

			for(int j = kRangeMin; j <= kRangeMax; j++)
			{
				KMeans kmeans( j, 0.01, space );
				kmeans.Run();
				currentEvaluation = kmeans.Evaluate();
				if( currentEvaluation < bestEvaluation )
				{
					bestEvaluation = currentEvaluation;
					bestK = j;
				}
			}
			results.SetClusters( bestK );
			results.SetEvaluation( bestEvaluation );
			results.SetFeatureSubset( possibleFeatureSubset );
			break;
		}
		case ISODATA:
		{
			IsoData isodata( kRangeMin, kRangeMax, 5, 0.01, 0.3, 0.05, space );
			isodata.Run();
			results.SetClusters( isodata.GetClusterCount() );
			results.SetEvaluation( isodata.Evaluate() );
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
	return results;
}

void FeatureSelection::LoadFeatureData( char *filename )
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

/////////////////////////////////////
// Feature Data Class
/////////////////////////////////////

FeatureData::FeatureData()
{
}

FeatureData::FeatureData( char * filename )
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
		printf("FeatureData::CreateSpace: Invalid points in space\n");
	}
	return space;
}

int FeatureData::LoadFeatureData( char * filename )
{
	ifstream in( filename );
	if( !in )
	{
		cerr<<"FeatureData::LoadFeatureData( char * ): Cannot open file \""<<filename<<"\"."<<endl;
		return -1;
	}

	int line = 0;
	char c = 0;

	string strValue;
		// Get the header data
	while( c != '\n' && !in.eof() )
	{
		in.get(c);
		if( c == ',' || c == '\n' )
		{
			char * c_str = new char[ strValue.length() ];
			strcpy( c_str, strValue.c_str() );
			featureList.push_back( c_str );
			strValue.clear();
			if( in.peek() == '\n' )
					c = in.get();
		}
		else
		{
			strValue += c;
		}
	}

		// Get the rest of the data
	while(!in.eof())
	{
		vector<double> coordinateData;
		c = 0;
			// Process a line in the file
		while( c != '\n' && !in.eof() )
		{
			double value;
			in.get( c );
				// A comma or a \n denotes the end of a field value, so we
				// stop adding characters to strValue and convert it to a double.
			if( c == ',' || c == '\n' )
			{
				value = strtod( strValue.c_str(), NULL );
				coordinateData.push_back( value );
				strValue.clear();
				if( in.peek() == '\n' )
					c = in.get();
			}
			else
			{
				strValue += c;
			}
		}
		if(!in.eof())
		{
			data.push_back( new Coordinate( coordinateData ) );
			line++;
		}
	}
	in.close();
	return 0;
}

/////////////////////////////
// Feature Results
/////////////////////////////

FeatureResults::FeatureResults()
{
	SetEvaluation( 1.0f );
}

FeatureResults::FeatureResults( double evaluation, int clusters, list<int> featureSubset )
{
	SetClusters( clusters );
	SetEvaluation( evaluation );
	SetFeatureSubset( featureSubset );
}

int FeatureResults::GetClusters()
{
	return clusters;
}

double FeatureResults::GetEvaluation()
{
	return evaluation;
}

list<int> FeatureResults::GetFeatureSubset()
{
	return this->featureSubset;
}

void FeatureResults::Print()
{
	list<int>::iterator iter;
	cout<<"Clusters: "<<this->clusters<<endl;
	cout<<"Evaluation: "<<this->evaluation<<endl;
	cout<<"Features: ";
	iter = featureSubset.begin();
	while( iter != featureSubset.end() )
	{
		cout<<(*iter)<<", ";
		iter++;
	}
	cout<<endl;
}

void FeatureResults::SetClusters( int clusters )
{
	this->clusters = clusters;
}

void FeatureResults::SetEvaluation( double evaluation )
{
	this->evaluation = evaluation;
}

void FeatureResults::SetFeatureSubset( list<int> featureSubset )
{
	this->featureSubset = featureSubset;
}

FeatureResults & FeatureResults::operator=( FeatureResults rhs )
{
	this->evaluation = rhs.evaluation;
	this->clusters = rhs.clusters;
	this->featureSubset = rhs.featureSubset;
	return *this;
}

bool FeatureResults::operator<( FeatureResults &rhs )
{
	return this->evaluation < rhs.evaluation;
}

