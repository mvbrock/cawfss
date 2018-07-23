#ifndef FEATUREDATA_HXX
#define FEATUREDATA_HXX

#include <vector>
#include <list>

#include "space.hxx"
#include "coordinate.hxx"

/** \brief Interfaces with the feature data.
  *
  * FeatureData is used to store the original feature data and create feature
  * spaces from it based on an arbitrary feature subset.
  */
class FeatureData
{
public:
	/// Construct the class
	FeatureData();
	/// Construct the class, loading a file containing comma-seperated values (CSV format) into memory.
	FeatureData( const string filename);
	/// Deconstruct the class.
	~FeatureData();
	/// Return the number of features in the data.
	int GetFeatureCount();
	/// Return the list of feature names in the data.
	vector<char *> GetFeatureList();
	/// Create a new space containing the data points specified by the feature subset.
	Space * CreateSpace( list<int> featureSubset );
	/// Load a file containing comma-seperated values (CSV format) into memory.
	int LoadFeatureData( const string filename );
private:
	// The feature names.
	vector<char *> featureList;
	// The feature data.
	list<Coordinate *> data;
};

#endif
