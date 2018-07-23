#include <iostream>
#include <fstream>
#include <string.h>

#include "clusterdata.hxx"

bool ClusterData::LoadFeatureData(	const string & filename,
									list<Coordinate *> & data,
									vector<char *> & featureList,
									bool loadFeatureList )
{
	ifstream in( filename.c_str() );
	if( !in )
	{
		cerr<<"FeatureData::LoadFeatureData( char * ): Cannot open file \""<<filename<<"\"."<<endl;
		return false;
	}

	int line = 0;
	char c = 0;

	string strValue;

	if( loadFeatureList == true )
	{
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
	}
	// Get the rest of the data
	unsigned int coordinateId = 0;
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
			data.push_back( new Coordinate( coordinateData, coordinateId ) );
			coordinateId++;
			line++;
		}
	}
	in.close();
	return true;
}

