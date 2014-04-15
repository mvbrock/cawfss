#include <unistd.h>
#include <iostream>
#include <string>
#include "stepwise.hxx"


int main(int argc, char ** argv)
{
	string inputFilename = "";
	string outputFilename = "";
	int climbDirection = -1;
	int maxBacktrack = -1;
	double backtrackThreshold = -1;
	long kMin = -1;
	long kMax = -1;
	long featSubsetMin = -1;
	long featSubsetMax = -1;
	double clustTerminationThreshold = -1.0f;
	long maxIterations = -1;
	long minClusterSize = -1;
	double mergeCoeff = -1.0f;
	double splitCoeff = -1.0f;
	
	// For determining the option.
	int opt;

	while( ( opt = getopt( argc, argv, "f:o:d:m:t:z:x:c:v:y:b:e:j:q:" ) ) != -1 )
	{
		switch( opt )
		{
			case 'f':
				inputFilename = string( optarg );
				break;
			case 'o':
				outputFilename = string( optarg );
				break;
			case 'd':
				climbDirection = atoi( optarg );
				break;
			case 'm':
				maxBacktrack = atoi( optarg );
				break;
			case 't':
				backtrackThreshold = atof( optarg );
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
			case 'b':
				maxIterations = atol( optarg );
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

	cout<< "Input Filename: " << inputFilename << endl
		<< "Output Filename: " << outputFilename << endl
		<< "Climb Direction: " << climbDirection << endl
		<< "Max Back Track: " << maxBacktrack << endl
		<< "Back Track Threshold " << backtrackThreshold << endl
		<< "K-Min: " << kMin << endl
		<< "K-Max: " << kMax << endl
		<< "Feat. Subset Min: " << featSubsetMin << endl
		<< "Feat. Subset Max: " << featSubsetMax << endl
		<< "Clust. Termination Thresh: " << clustTerminationThreshold << endl
		<< "Max. Iterations: " << maxIterations << endl
		<< "Min. Cluster Size: " << minClusterSize << endl
		<< "Merge Coeff.: " << mergeCoeff << endl
		<< "Split Coeff.: " << splitCoeff << endl;

	if( inputFilename == "" || outputFilename == "" || climbDirection < 0 || maxBacktrack < 0 || backtrackThreshold < 0 ||
		kMin < 0 || kMax < 0 || featSubsetMin < 0 || featSubsetMax < 0 || clustTerminationThreshold < 0.0f ||
		minClusterSize < 0 || mergeCoeff < 0 || splitCoeff < 0 )
	{
		cout	<< "usage: ./hillclimbing_app -f <input filename> -o <output filename> -d <climb direction> "
			<< "-m <max backtrack> -t <backtrack threshold> -z <k-min> -x <k-max> -c <min feature subset size> "
			<< "-v <max feature subset size> -e <min cluster size> -y <clustering termination threshold>"
			<< "-b <max iterations> -j <merge coefficient> -q <split coefficient>" << endl;
		return -1;
	}

	Stepwise sw( outputFilename );
	sw.SetBacktrack( maxBacktrack, backtrackThreshold );
	sw.SetClusteringMethod( FeatureSelection::ISODATA );
	sw.LoadFeatureData(inputFilename);
	sw.SetFeatureSubsetRange( featSubsetMin, featSubsetMax );
	sw.SetKRange( kMin, kMax );
	sw.SetTerminationThreshold(clustTerminationThreshold);
	sw.SetIsoDataParams( minClusterSize, mergeCoeff, splitCoeff );
	sw.SetMaxIterations( maxIterations );
	sw.Initialize( climbDirection );	
	sw.Run();

	return 1;
}

