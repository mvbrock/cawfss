/////////////////////////////
// K-Means Library
// Author: Matt Brock
// Date: 08-28-2007
// Last Updated: 09-25-2007
/////////////////////////////

#include <fstream>
#include <sstream>
#include <cmath>
#include "space.hxx"

//////////////////////////////////////////////////////////////
// Space Class
//////////////////////////////////////////////////////////////

Space::Space() {
}

Space::Space(int dimension) {
	SetDimension(dimension);
}

Space::Space(int dimension, list<Coordinate *> pointList) {
	SetDimension(dimension);
	AddPointList(pointList);
}

Space::~Space() {
	DeletePoints();
}

int Space::AddPoint(Coordinate * point) {
	pointList.push_back(point);
}

int Space::AddPointList(list<Coordinate *> pointList) {
	list<Coordinate *>::iterator iter = pointList.begin();
	while (iter != pointList.end()) {
		this->pointList.push_back(*iter);
		iter++;
	}
}

int AddPointList(char * filename) {
	ifstream f(filename, ifstream::in);
	streambuf sb();
	while (!f.eof()) {

	}
	f.close();
}

int Space::ClearPoints() {
	pointList.clear();
}

int Space::DeletePoints() {
	for (int i = pointList.size(); i > 0; i--) {
		Coordinate * coordinate = pointList.front();
		pointList.pop_front();
		delete coordinate;
	}
}

list<Coordinate *> Space::GetPoints() {
	return pointList;
}

int Space::Normalize() {
	list<Coordinate *>::iterator iter = pointList.begin();
	vector<double> greatestCoordinateData = (*iter)->GetCoordinateData();
	// Iterate through all of the points
	while (iter != pointList.end()) {
		Coordinate * coordinate = (*iter);
		// Create a vector which will represent our 
		vector<double> coordinateData = coordinate->GetCoordinateData();
		for (int i = 0; i < coordinateData.size(); i++) {
			if (greatestCoordinateData[i] < coordinateData[i])
				greatestCoordinateData[i] = coordinateData[i];
		}
		iter++;
	}
	Coordinate greatest(greatestCoordinateData);
	greatest.Invert();
	iter = pointList.begin();
	while (iter != pointList.end()) {
		Coordinate * coordinate = (*iter);
		*coordinate = *coordinate * greatest;
		iter++;
	}
}

int Space::GetDimension() {
	return dimension;
}

int Space::SetDimension(int dimension) {
	this->dimension = dimension;
}

void Space::Print() {
	list<Coordinate *>::iterator iter = pointList.begin();
	while (iter != pointList.end()) {
		(*iter)->Print();
		iter++;
	}
}

int Space::Size() {
	return pointList.size();
}

bool Space::HasValidPoints() {
	list<Coordinate *>::iterator iter = pointList.begin();
	while (iter != pointList.end()) {
		if ((*iter)->GetDimension() != this->GetDimension())
			return false;
		iter++;
	}
	return true;
}

//////////////////////////////////////////////////////////////
// Cluster Class
//////////////////////////////////////////////////////////////

Cluster::Cluster() {
	clusterCenter = new Coordinate();
}

Cluster::Cluster(Space & space) {
	SetDimension(space.GetDimension());
	clusterCenter = new Coordinate();
}

Cluster::Cluster(Space & space, Coordinate clusterCenter) {
	SetDimension(space.GetDimension());
	this->clusterCenter = new Coordinate();
	*this->clusterCenter = clusterCenter;
}

Cluster::Cluster(Space & space, Coordinate clusterCenter,
		list<Coordinate *> pointList) {
	SetDimension(space.GetDimension());
	this->clusterCenter = new Coordinate();
	SetClusterCenter(clusterCenter);
	AddPointList(pointList);
}

Cluster::~Cluster() {

	DeletePoints();
	delete clusterCenter;
}

int Cluster::SetClusterCenter(Coordinate & clusterCenter) {
	*this->clusterCenter = clusterCenter;
}

Coordinate * Cluster::GetClusterCenter() {
	return this->clusterCenter;
}

float Cluster::AverageDistance() {
	float averageDistance = 0;
	list<Coordinate *>::iterator coordIter = pointList.begin();
	while (coordIter != pointList.end()) {
		averageDistance += Coordinate::Distance(*clusterCenter, *(*coordIter));
		coordIter++;
	}
	averageDistance /= pointList.size();
	return averageDistance;
}

Coordinate Cluster::StandardDeviation() {
	return Coordinate::StandardDeviation(pointList, *clusterCenter);
}

float Cluster::Shift() {
	Coordinate averageCoordinate;
	float shiftDistance;

	if (pointList.size() != 0)
		averageCoordinate = Coordinate::Average(pointList, dimension);
	else {
		averageCoordinate = Coordinate::Random(dimension, r);
	}
	shiftDistance = Coordinate::Distance(*clusterCenter, averageCoordinate);
	*this->clusterCenter = averageCoordinate;
	return shiftDistance;
}

//////////////////////////////////////////////////////////////
// Coordinate Class
//////////////////////////////////////////////////////////////

Coordinate::Coordinate() {
	this->coordinateData = new vector<double>;
}

Coordinate::Coordinate(vector<double> coordinateData) {
	this->coordinateData = new vector<double>;
	*this->coordinateData = coordinateData;
}

Coordinate::~Coordinate() {
	delete this->coordinateData;
}

vector<double> Coordinate::GetCoordinateData() {
	return *coordinateData;
}

void Coordinate::SetCoordinateData(vector<double> coordinateData) {
	*this->coordinateData = coordinateData;
}

int Coordinate::GetDimension() {
	return coordinateData->size();
}

void Coordinate::Print() {
	for (int i = 0; i < this->coordinateData->size(); i++)
		cout << (*this->coordinateData)[i] << ", ";
	cout << endl;
}

string Coordinate::ToString() {
	ostringstream outs;
	for (int i = 0; i < this->coordinateData->size(); i++) {
		outs << (*this->coordinateData)[i];
		if (i != this->coordinateData->size() - 1)
			outs << ", ";
	}
	return outs.str();
}

int Coordinate::Invert() {
	for (int i = 0; i < this->coordinateData->size(); i++) {
		double value = (*this->coordinateData)[i];
		if (value != 0) {
			value = pow(value, -1);
			(*this->coordinateData)[i] = value;
		}
	}
}

int Coordinate::Boundarize() {
	for (int i = 0; i < coordinateData->size(); i++) {
		if ((*coordinateData)[i] <= 0)
			(*coordinateData)[i] = 0.001;
		// TODO, create a > operator!
		if (1 <= (*coordinateData)[i])
			(*coordinateData)[i] = 0.999;
	}
}

Coordinate Coordinate::Reduced(list<int> dimensions) {
	dimensions.sort();
	vector<double> reducedCoordinateData;
	list<int>::iterator iter = dimensions.begin();
	for (int i = 0; i < coordinateData->size(); i++) {
		if (i == (*iter)) {
			reducedCoordinateData.push_back((*coordinateData)[i]);
			iter++;
		}
	}
	if (reducedCoordinateData.size() != dimensions.size()) {
		iter = dimensions.begin();
		while (iter != dimensions.end()) {
			printf("%d, ", (*iter));
			iter++;
		}
		printf("\n");
	}
	return Coordinate(reducedCoordinateData);
}

Coordinate & Coordinate::operator=(const Coordinate & rhs) {
	*this->coordinateData = *rhs.coordinateData;
	return *this;
}

Coordinate Coordinate::operator+(const Coordinate & rhs) {
	Coordinate coordinate;
	if (this->coordinateData->size() != rhs.coordinateData->size()) {
		cerr << "Coordinate::operator + : Mismatched dimensions." << endl;
		return coordinate;
	}
	for (int i = 0; i < this->coordinateData->size(); i++)
		coordinate.coordinateData->push_back(
				(*this->coordinateData)[i] + (*rhs.coordinateData)[i]);
	return coordinate;
}

Coordinate Coordinate::operator-(const Coordinate & rhs) {
	Coordinate coordinate;
	if (coordinateData->size() != rhs.coordinateData->size()) {
		cerr << "Coordinate::operator - : Mismatched dimensions." << endl;
		return coordinate;
	}

	for (int i = 0; i < rhs.coordinateData->size(); i++)
		coordinate.coordinateData->push_back(
				(*coordinateData)[i] - (*rhs.coordinateData)[i]);

	return coordinate;
}

Coordinate Coordinate::operator*(const Coordinate & rhs) {
	Coordinate coordinate;
	if (coordinateData->size() != rhs.coordinateData->size()) {
		cerr << "Coordinate::operator * : Mismatched dimensions. "
				<< coordinateData->size() << " : " << rhs.coordinateData->size()
				<< endl;
		return coordinate;
	}

	for (int i = 0; i < rhs.coordinateData->size(); i++)
		coordinate.coordinateData->push_back(
				(*coordinateData)[i] * (*rhs.coordinateData)[i]);

	return coordinate;
}

Coordinate Coordinate::operator/(const Coordinate & rhs) {
	Coordinate coordinate;
	if (coordinateData->size() != rhs.coordinateData->size()) {
		cerr << "Coordinate::operator / : Mismatched dimensions." << endl;
		return coordinate;
	}

	for (int i = 0; i < rhs.coordinateData->size(); i++) {
		if ((*rhs.coordinateData)[i] == 0) {
			cerr << "Coordinate::operator / : Divide-by-Zero." << endl;
			coordinate = Coordinate::Singular(0, rhs.coordinateData->size());
			return coordinate; //Coordinate::Singular(0,rhs.coordinateData->size());
		}
		coordinate.coordinateData->push_back(
				(*coordinateData)[i] / (*rhs.coordinateData)[i]);
	}
	return coordinate;
}

bool Coordinate::operator<(const Coordinate &rhs) {
	vector<double> rhsCoordinateData = *rhs.coordinateData;
	if (coordinateData->size() != rhs.coordinateData->size()) {
		cerr << "Coordinate::operator<: Mismatched dimensions." << endl;
		return false;
	}

	for (int i = 0; i < rhsCoordinateData.size(); i++) {
		if ((*coordinateData)[i] >= rhsCoordinateData[i])
			return false;
	}
	return true;
}

bool Coordinate::operator>(const Coordinate &rhs) {
	vector<double> rhsCoordinateData = *rhs.coordinateData;
	if (coordinateData->size() != rhs.coordinateData->size()) {
		cerr << "Coordinate::operator<: Mismatched dimensions." << endl;
		return false;
	}

	for (int i = 0; i < rhsCoordinateData.size(); i++) {
		if ((*coordinateData)[i] <= rhsCoordinateData[i])
			return false;
	}
	return true;
}

bool Coordinate::operator==(const Coordinate & rhs) {
	vector<double> rhsCoordinateData = *rhs.coordinateData;
	if (coordinateData->size() != rhs.coordinateData->size()) {
		cerr << "Coordinate::operator<: Mismatched dimensions." << endl;
		return false;
	}

	for (int i = 0; i < rhsCoordinateData.size(); i++) {
		if ((*coordinateData)[i] != rhsCoordinateData[i])
			return false;
	}
	return true;
}

bool Coordinate::operator!=(const Coordinate & rhs) {
	vector<double> rhsCoordinateData = *rhs.coordinateData;
	if (coordinateData->size() != rhs.coordinateData->size()) {
		cerr << "Coordinate::operator<: Mismatched dimensions." << endl;
		return false;
	}

	for (int i = 0; i < rhsCoordinateData.size(); i++) {
		if ((*coordinateData)[i] == rhsCoordinateData[i])
			return false;
	}
	return true;
}

double Coordinate::Distance(Coordinate & coordinateOne,
		Coordinate & coordinateTwo) {
	double distanceSquared = 0;

	if (coordinateOne.GetDimension() != coordinateTwo.GetDimension()) {
		cerr << "Coordinate::Distance(): Mismatched dimensions. "
				<< coordinateOne.GetDimension() << " : "
				<< coordinateTwo.GetDimension() << endl;
		exit(-1);
		return -1.0f;
	}
	Coordinate vectorDistanceSquared;

	vectorDistanceSquared = (coordinateTwo - coordinateOne)
			* (coordinateTwo - coordinateOne);

	vector<double> vectorDistanceSquaredData =
			vectorDistanceSquared.GetCoordinateData();
	for (int i = 0; i < vectorDistanceSquaredData.size(); i++)
		distanceSquared += vectorDistanceSquaredData[i];

	return sqrt(distanceSquared);
}

Coordinate Coordinate::Abs(Coordinate & coordinate) {
	int dimension = coordinate.GetDimension();
	vector<double> coordinateData = coordinate.GetCoordinateData();
	for (int i = 0; i < dimension; i++)
		coordinateData[i] = abs(coordinateData[i]);
	Coordinate newCoordinate(coordinateData);
	return newCoordinate;

}

Coordinate Coordinate::Average(list<Coordinate *> coordList, int dimension) {
	list<Coordinate *>::iterator iter = coordList.begin();

	// Create a Coordinate object to total up all of the coordinates
	// in the coordList list.
	Coordinate total = Coordinate::Singular(0, dimension);
	if (coordList.size() == 0)
		return total;

	while (iter != coordList.end()) {
		total = total + *(*iter);
		iter++;
	}

	total = total / Coordinate::Singular(coordList.size(), dimension);
	return total;
}

Coordinate Coordinate::Random(int dimension, RNG & r) {
	vector<double> coordinateData(dimension);
	r.uniform(coordinateData);
	return Coordinate(coordinateData);
}

Coordinate Coordinate::Singular(double value, int dimension) {
	vector<double> coordinateData;
	for (int i = 0; i < dimension; i++)
		coordinateData.push_back(value);
	Coordinate coordinate(coordinateData);
	return coordinate;
}

Coordinate Coordinate::SquareRoot(Coordinate & coordinate) {
	vector<double> coordinateData = coordinate.GetCoordinateData();
	for (int i = 0; i < coordinate.GetDimension(); i++) {
		coordinateData[i] = sqrt(coordinateData[i]);
	}
	Coordinate newCoordinate(coordinateData);
	return newCoordinate;
}

Coordinate Coordinate::StandardDeviation(list<Coordinate *> & coordList,
		Coordinate & coordinate) {
	int dimension = coordinate.GetDimension();

	if (coordList.size() == 0)
		return Coordinate::Singular(0, dimension);

	list<Coordinate *>::iterator iter = coordList.begin();

	Coordinate total = Coordinate::Singular(0, dimension);

	while (iter != coordList.end()) {
		Coordinate c = *(*iter) - coordinate;
		c = c * c;
		total = total + c;
		iter++;
	}

	Coordinate denom = Coordinate::Singular(coordList.size(), dimension);
	total = (total / denom);
	return Coordinate::SquareRoot(total);
}
