#include <unistd.h>
#include <iostream>
#include <string>
#include "evolutionary.hxx"


int main(int argc, char ** argv)
{
	string inputFilename = "";
	string outputFilename = "";
	double mutationProb = -1.0f;
	double survivalPercent = -1.0f;
	double evTerminationThreshold = -1.0f;
	long maxIterations = -1;
	long populationSize = -1;
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

	while( ( opt = getopt( argc, argv, "f:o:m:s:t:i:p:z:x:c:v:y:e:j:q:" ) ) != -1 )
	{
		switch( opt )
		{
			case 'f':
				inputFilename = string( optarg );
				break;
			case 'o':
				outputFilename = string( optarg );
				break;
			case 'm':
				mutationProb = atof( optarg );
				break;
			case 's':
				survivalPercent = atof( optarg );
				break;
			case 't':
				evTerminationThreshold = atof( optarg );
				break;
			case 'i':
				maxIterations = atol( optarg );
				break;
			case 'p':
				populationSize = atol( optarg );
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
		<< "Mutation Prob.: " << mutationProb << endl
		<< "Survival %: " << survivalPercent << endl
		<< "Ev. Termination Thresh.: " << evTerminationThreshold << endl
		<< "Max. Iterations: " << maxIterations << endl
		<< "Pop. Size: " << populationSize << endl
		<< "K-Min: " << kMin << endl
		<< "K-Max: " << kMax << endl
		<< "Feat. Subset Min: " << featSubsetMin << endl
		<< "Feat. Subset Max: " << featSubsetMax << endl
		<< "Clust. Termination Thresh: " << clustTerminationThreshold << endl
		<< "Min. Cluster Size: " << minClusterSize << endl
		<< "Merge Coeff.: " << mergeCoeff << endl
		<< "Split Coeff.: " << splitCoeff << endl;

	if( inputFilename == "" || outputFilename == "" || mutationProb < 0.0f || survivalPercent < 0.0f ||
		evTerminationThreshold < 0.0f || maxIterations < 0 || populationSize < 0 || kMin < 0 || kMax < 0 ||
		featSubsetMin < 0 || featSubsetMax < 0 || clustTerminationThreshold < 0.0f || minClusterSize < 0 ||
		mergeCoeff < 0 || splitCoeff < 0 )
	{
		cout	<< "usage: ./evolutionary_app -f <input filename> -o <output filename> -m <mutation probability> "
			<< "-s <survival percentage> -t <evolutionary termination threshold> -i <max iterations> "
			<< "-p <population size> -z <k-min> -x <k-max> -c <min feature subset size> "
			<< "-v <max feature subset size> -y <clustering termination threshold> -e <min cluster size> "
			<< "-j <merge coefficient> -q <split coefficient>" << endl;
		return -1;
	}

	Evolutionary ev( outputFilename );
	ev.SetClusteringMethod( FeatureSelection::ISODATA );
	ev.LoadFeatureData(inputFilename);
	ev.SetFeatureSubsetRange( featSubsetMin, featSubsetMax );
	ev.SetKRange( kMin, kMax );
	ev.SetTerminationThreshold(clustTerminationThreshold);
	ev.SetIsoDataParams( minClusterSize, mergeCoeff, splitCoeff );
	ev.Initialize( mutationProb, survivalPercent, evTerminationThreshold, maxIterations, populationSize );	
	ev.Run();

	return 1;
}

