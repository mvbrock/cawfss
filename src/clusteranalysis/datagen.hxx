#include "space.hxx"

class DataGen
{
public:
	static Space * GenerateMatrixConfig( int dimension, unsigned int size, unsigned int numPts );

	static Space * GenerateDiagonalConfig( int dimension, unsigned int numClusters, unsigned int numPts );
};

