#include "datagen.hxx"

Space * DataGen::GenerateMatrixConfig( int dimension, unsigned int size, unsigned int numPts )
{
	RNG r;
	Space * s = new Space( dimension );
	list<Coordinate *> pointList;
	for( int i = 0; i < pow((double)size,dimension); i++ )
	{
		vector<double> clusterCenterData;
		double total = i;
		for( int k = dimension-1; k >= 0; k-- )
		{
			double value = total / pow((double)size, k);
			clusterCenterData.push_back( (int)value/(double)size );
			if( (int)value > 0 )
				total = (int)total % (int)pow((double)size, k);	
			
		}
		Coordinate clusterCenter( clusterCenterData, -1 );
		clusterCenter.Print();
		for( int j = 0; j < numPts; j++ )
		{
			
		}
	}
}

Space * DataGen::GenerateDiagonalConfig( int dimension, unsigned int numClusters, unsigned int numPts )
{
	RNG r;
	Space * s = new Space( dimension );
	list<Coordinate *> pointList;
	for( int i = 0; i < numClusters; i++ )
	{
		for( int j = 0; j < numPts; j++ )
		{
			Coordinate * coordinate = new Coordinate();
			*coordinate = ( Coordinate::Random( dimension, r ) /
					Coordinate::Singular( (double)numClusters, dimension ) ) +
					Coordinate::Singular( (double)i/(double)numClusters, dimension );
			pointList.push_back( coordinate );

		}
	}
	s->AddPointList( pointList );
	return s;
}

