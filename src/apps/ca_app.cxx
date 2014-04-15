#include <unistd.h>
#include <iostream>

#include "isodata.hxx"
#include "kmeans.hxx"
#include "clusterdata.hxx"

using namespace std;


int main( int argc, char ** argv )
{
	// Input data file
	string inputFilename;
	// Output of data from ISODATA iterations.
	string outputFilename;
	// The list of Coordinate objects generated from the input data file.
	list< Coordinate * > data;
	// The list of features.
	vector< char * > featureList;
	// The Space object.
	Space s;
	// The maximum K value
	unsigned int kMax = 0;
	// The minimum number of elements allowed in a cluster.
	unsigned int minClusterSize = 0;
	// The merge coefficient applied to 1/sqrt(kMax/2)
	double mergeCoeff = -1.0;
	// The split coefficient applied to 1/(2*sqrt(kMax/2))
	double splitCoeff = -1.0;
	// The termination condition for the algorithm.
	double terminationCondition = -1.0;
	// Maximum iterations allowed for the algorithm.
	unsigned int maxIterations = 0;
	// For determining the option.
	int opt;

	// Parse the command line arguments.
	while( ( opt = getopt( argc, argv, "f:o:k:c:m:s:t:i:" ) ) != -1 )
	{
		switch( opt )
		{
			case 'f':
				inputFilename = optarg;
				break;
			case 'o':
				outputFilename = optarg;
				break;
			case 'k':
				kMax = atoi( optarg );
				break;
			case 'c':
				minClusterSize = atoi( optarg );
				break;
			case 'm':
				mergeCoeff = atof( optarg );
				break;
			case 's':
				splitCoeff = atof( optarg );
				break;
			case 't':
				terminationCondition = atof( optarg );
				break;
			case 'i':
				maxIterations = atoi( optarg );
				break;
			default:
				cout << "Unknown option: -" << opt << endl;
				break;
		}
	}
	
	if(	inputFilename == "" || outputFilename == "" || kMax == 0 || minClusterSize == 0 || mergeCoeff == -1.0 ||
		splitCoeff == -1.0 || terminationCondition == -1.0 || maxIterations == 0 )
	{
		cout	<< "Usage: ./test -f <input file name> -o <output file name> -k <max k> "
			<< "-c <min cluster size> -m <merge coefficient> -s <split coefficient> "
			<< "-t <termination condition> -i <max iterations>" << endl;
		return -1;
	}

	// Create the data points within the space and normalize them to [0,1].
	ClusterData::LoadFeatureData( inputFilename, data, featureList, true );
	s.AddPointList( data );
	s.Normalize();
	// Create the ISODATA object.
	IsoData iso(	string(outputFilename),
			1,
			kMax,
			minClusterSize,
			terminationCondition,
			maxIterations,
			(1/sqrt(kMax/2))*mergeCoeff,
			(1/(2*sqrt(kMax/2)))*splitCoeff,
			&s );

	// Print out the attributes of the run.
	cout << "Input File Name: " << inputFilename << endl;
	cout << "Output File Name: " << outputFilename << endl;	
	cout << "K Max: " << kMax << endl;
	cout << "Min Cluster Size: " << minClusterSize << endl;
	cout << "Merge Coefficient: " << mergeCoeff << endl;
	cout << "Split Coefficient: " << splitCoeff << endl;
	cout << "Termination Condition: " << terminationCondition << endl;
	cout << "Max Iterations: " << maxIterations << endl;
	cout << "Dimension: " << s.GetDimension() << endl;
	cout << "Feature List Size:" << featureList.size() << endl;
	cout << "Data Size:" << data.size() << endl;
	cout << "Size:" << s.Size() << endl << endl;

	// Run the ISODATA algorithm.
	iso.Run();
	// Get the final clusters from the run.	
	list< Cluster * > clusterList = iso.GetClusters();
}

