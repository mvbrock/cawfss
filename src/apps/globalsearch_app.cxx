#include <unistd.h>
#include <iostream>
#include <string>
#include "globalsearch.hxx"


int main(int argc, char ** argv)
{
	string inputFilename = "";
	string outputFilename = "";
	long featureSubsetSize = -1;
	long kMin = -1;
	long kMax = -1;
	long featSubsetMin = -1;
	long featSubsetMax = -1;
	double clustTerminationThreshold = -1.0f;
	long minClusterSize = -1;
	double mergeCoeff = -1.0f;
	double splitCoeff = -1.0f;
	
	// For determining the option.
	int opt;

	while( ( opt = getopt( argc, argv, "f:o:s:z:x:c:v:y:e:j:q:" ) ) != -1 )
	{
		switch( opt )
		{
			case 'f':
				inputFilename = string( optarg );
				break;
			case 'o':
				outputFilename = string( optarg );
				break;
			case 's':
				featureSubsetSize = atol( optarg );
				break;
			case 'z':
				kMin = atol( optarg );
				break;
			case 'x':
				kMax = atol( optarg );
				break;
			case 'c':
				featSubsetMin = atol( optarg );
				break;
			case 'v':
				featSubsetMax = atol( optarg );
				break;
			case 'y':
				clustTerminationThreshold = atof( optarg);
				break;
			case 'e':
				minClusterSize = atol( optarg );
				break;
			case 'j':
				mergeCoeff = atof( optarg );
				break;
			case 'q':
				splitCoeff = atof( optarg );
				break;
			default:
				cout << "Unknown option: -" << opt << endl;
				break;
		}
	}

	cout	<< "Input Filename: " << inputFilename << endl
		<< "Output Filename: " << outputFilename << endl
		<< "Feature Subset Size: " << featureSubsetSize << endl
		<< "K-Min: " << kMin << endl
		<< "K-Max: " << kMax << endl
		<< "Feat. Subset Min: " << featSubsetMin << endl
		<< "Feat. Subset Max: " << featSubsetMax << endl
		<< "Clust. Termination Thresh: " << clustTerminationThreshold << endl
		<< "Min. Cluster Size: " << minClusterSize << endl
		<< "Merge Coeff.: " << mergeCoeff << endl
		<< "Split Coeff.: " << splitCoeff << endl;

	if( inputFilename == "" || outputFilename == "" || featureSubsetSize < 0 || kMin < 0 || kMax < 0 ||
		featSubsetMin < 0 || featSubsetMax < 0 || clustTerminationThreshold < 0.0f || minClusterSize < 0 ||
		mergeCoeff < 0 || splitCoeff < 0 )
	{
		cout	<< "usage: ./globalsearch_app -f <input filename> -o <output filename> -s <feature subset size> "
			<< "-z <k-min> -x <k-max> -c <min feature subset size> -v <max feature subset size> "
			<< "-y <clustering termination threshold> -e <min cluster size> -j <merge coefficient> "
			<< "-q <split coefficient>" << endl;
		return -1;
	}

	GlobalSearch gs( outputFilename );
	gs.SetClusteringMethod( FeatureSelection::ISODATA );
	gs.LoadFeatureData(inputFilename);
	gs.SetFeatureSubsetRange( featSubsetMin, featSubsetMax );
	gs.SetKRange( kMin, kMax );
	gs.SetTerminationThreshold(clustTerminationThreshold);
	gs.SetIsoDataParams( minClusterSize, mergeCoeff, splitCoeff );
	gs.Initialize( featureSubsetSize );	
	gs.Run();

	return 1;
}

