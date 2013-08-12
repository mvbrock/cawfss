#include <map>
#include "clusteranalysis/rng.h"
#include "evolutionary.hxx"

using namespace std;

Evolutionary::Evolutionary() {
}

Evolutionary::~Evolutionary() {
}

void Evolutionary::Initialize(double mutationCoefficient,
		double selectionPercentage, double terminationCondition,
		int populationSize) {
	this->mutationCoefficient = mutationCoefficient;
	this->selectionPercentage = selectionPercentage;
	this->terminationCondition = terminationCondition;
	this->populationSize = populationSize;
	for (int i = 0; i < populationSize; i++) {
		list<int> member = GenerateRandomMember();
		currentPopulation.push_back(member);
	}
}

FeatureResults Evolutionary::Run() {
	FeatureResults result;
	do {
		result = this->Step();
		result.Print();
	} while (result.GetEvaluation() > this->terminationCondition);

	return result;
}

FeatureResults Evolutionary::Step(int direction) {
	int survivingPopulationSize = (int) (currentPopulation.size()
			* selectionPercentage);
	list<FeatureResults> populationEvaluation;
	vector<list<int> >::iterator memberIter = currentPopulation.begin();

	while (memberIter != currentPopulation.end()) {
		FeatureResults result = this->Evaluate((*memberIter));
		populationEvaluation.push_back(result);
		memberIter++;
	}
	populationEvaluation.sort();

	currentFeatureSubset = populationEvaluation.front().GetFeatureSubset();

	for (int i = 0; i < currentPopulation.size() - survivingPopulationSize; i++)
		populationEvaluation.pop_back();

	currentPopulation.clear();

	// Convert the list to a vector so we can grab random members
	// and procreate.
	// TODO: Find a better way to do this so we don't have to
	// create copies of the surviving population.
	vector<list<int> > survivingMembers;
	list<FeatureResults>::iterator resultIter = populationEvaluation.begin();
	while (resultIter != populationEvaluation.end()) {
		survivingMembers.push_back((*resultIter).GetFeatureSubset());
		resultIter++;
	}

	RNG r;
	for (int i = 0; i < this->populationSize; i++) {
		int randIndexOne = r.rand_int() % survivingPopulationSize;
		int randIndexTwo;
		do {
			randIndexTwo = r.rand_int() % survivingPopulationSize;
		} while (randIndexTwo == randIndexOne);

		list<int> newMember = this->Procreate(survivingMembers[randIndexOne],
				survivingMembers[randIndexTwo]);
		this->MutateMember(newMember);
		currentPopulation.push_back(newMember);
	}

	return populationEvaluation.front();
}

struct lint {
	bool operator()(int i1, int i2) {
		return (i1 < i2);
	}
};

list<int> Evolutionary::GenerateRandomMember() {
	list<int> member;
	map<int, bool, lint> selectedFeatures;

	if (data->GetFeatureCount() != 0) {
		// The size of our member will be between the
		// subsetSizeMax and subsetSizeMin.
		int subsetSize = (r.rand_int() % (subsetSizeMax - subsetSizeMin + 1))
				+ subsetSizeMin;

		for (int i = 0; i < subsetSize; i++) {
			int memberValue;
			// Loop until we find a member value that
			// hasn't been selected for the member yet.
			do {
				memberValue = r.rand_int() % data->GetFeatureCount();
			} while (selectedFeatures[memberValue] == true);

			// Push back the member value and indicate
			// that it has been used.
			member.push_back(memberValue);
			selectedFeatures[memberValue] = true;
		}
	}
	return member;
}

list<int> Evolutionary::Procreate(list<int> memberOne, list<int> memberTwo) {
	// Merge the two lists together
	list<int> member = memberOne;
	member.merge(memberTwo);

	// Remove any duplicate features.
	map<int, bool, lint> uniqueFeatures;
	list<int>::iterator memberIter = member.begin();

	while (memberIter != member.end()) {
		uniqueFeatures[(*memberIter)] = true;
		memberIter++;
	}
	member.clear();

	map<int, bool, lint>::iterator mapIter = uniqueFeatures.begin();
	while (mapIter != uniqueFeatures.end()) {
		member.push_back((*mapIter).first);
		mapIter++;
	}
	// Sort the list of unique features.
	member.sort();

	// Determine the size of the combined subset
	int subsetSize = (r.rand_int() % (subsetSizeMax - subsetSizeMin + 1))
			+ subsetSizeMin;
	// The subset size determines how many features we will
	// remove from the combined members.
	int removalCount = member.size() - subsetSize;

	// Create a list of random features to remove by
	// using a map to generate unique feature indices.
	map<int, bool, lint> featuresToRemove;
	for (int i = 0; i < removalCount; i++) {
		int removalValue;
		// Generate random indices until we get a unique
		// feature index.
		do {
			removalValue = r.rand_int() % data->GetFeatureCount();
		} while (featuresToRemove[removalValue] == true);
		// Add it to the list.
		featuresToRemove[removalValue] = true;
	}

	// Setup our iterators so we can scan through the list
	// of features in the combined members and erase the
	// features we need to erase.
	mapIter = featuresToRemove.begin();
	memberIter = member.begin();

	// Loop until one of the iterators has reached its end.
	while (memberIter != member.end() && mapIter != featuresToRemove.end()) {
		// If we have a match, remove the feature from the
		// combined member.
		if ((*memberIter) == (*mapIter).first) {
			member.erase(memberIter);
			memberIter--;
			mapIter++;
		}
		// Iterate the feature removal map iterator if the value
		// of the current key is less than the value of the current
		// feature in the combined member.
		if ((*mapIter).first < (*memberIter))
			mapIter++;
		memberIter++;
	}
	return member;
}

void Evolutionary::MutateMember(list<int> & member) {
	// Decide whether or not to mutate the member.
	if (r.uniform() > mutationCoefficient)
		return;

	// For now the probability of an add or a removal
	// is statically set at 0.5
	bool addFeature = r.rand_int() % 2;
	bool removeFeature = r.rand_int() % 2;

	// Map the member's features.	
	map<int, bool, lint> memberFeatures;
	list<int>::iterator memberIter = member.begin();
	while (memberIter != member.end()) {
		memberFeatures[(*memberIter)] = true;
		memberIter++;
	}

	// If we need to add a feature or the member size is at
	// or less than the subsetSizeMin...
	if (addFeature == true || member.size() <= this->subsetSizeMin) {
		int featureToAdd;
		// Generate random indices until we get a feature
		// that doesn't exist in the member.
		do {
			featureToAdd = r.rand_int() % data->GetFeatureCount();
		} while (memberFeatures[featureToAdd] == true);
		// Add this feature.
		member.push_back(featureToAdd);
	}
	if (removeFeature == true || member.size() >= this->subsetSizeMax) {
		int featureToRemove;
		// Generate random indices until we get a feature
		// that exists in the member.
		// MORE EFFICIENT: RANDOMLY CHOOSE ONE OF THE
		// MEMBER FEATURES VIA AN INDEX
		do {
			featureToRemove = r.rand_int() % data->GetFeatureCount();
		} while (memberFeatures[featureToRemove] != true);

		memberIter = member.begin();
		// Search through the member features until we find
		// that feature to remove.
		while (memberIter != member.end()) {
			if ((*memberIter) == featureToRemove) {
				member.erase(memberIter);
				break;
			}
			memberIter++;
		}
	}
	// Sort at the end.
	member.sort();
}
