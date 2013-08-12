#include <iostream>
#include <string>
#include "evolutionary.hxx"



int main()
{
	Evolutionary sw;
	sw.SetClusteringMethod( FeatureSelection::KMEANS );
	sw.SetFeatureSubsetRange( 2, 6 );
	sw.SetKRange(5,100);
	sw.LoadFeatureData("dhrystone_result_100.csv");
	sw.Initialize(0.10, 0.50, 0.04, 20);
	sw.Run();
	return 1;
}
