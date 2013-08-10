#include <stdio.h>
#include "isodata.hxx"


int main()
{
	int dimension = 2;
	RNG r;
	Space s(dimension);

	IsoData k(10, 20, 250, 0.01f, 0.1f, 0.1f, &s);

	list<Coordinate *> pointList;
	for(int i = 0; i < 5000; i++ )
	{
		Coordinate * coordinate = new Coordinate();
		*coordinate = Coordinate::Random(dimension,r);
		pointList.push_back( coordinate );
	}

	s.AddPointList( pointList );
	k.Run();
}

