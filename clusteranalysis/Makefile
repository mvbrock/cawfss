#################################################
## Makefile for the cluster analysis libraries
#################################################

CXXFLAGS=-g3

all: space.o kmeans.o kmeans_pyramid.o isodata.o rng.o test

space.o: space.cxx space.hxx
	g++ $(CXXFLAGS) -c -o space.o space.cxx

kmeans.o: kmeans.cxx kmeans.hxx space.hxx
	g++ $(CXXFLAGS) -c -o kmeans.o kmeans.cxx

kmeans_pyramid.o: kmeans_pyramid.cxx kmeans_pyramid.hxx
	g++ $(CXXFLAGS) -c -o kmeans_pyramid.o kmeans_pyramid.cxx

isodata.o: isodata.cxx isodata.hxx
	g++ $(CXXFLAGS) -c -o isodata.o isodata.cxx

rng.o: rng.cpp rng.h
	g++ $(CXXFLAGS) -c -o rng.o rng.cpp

test:	isodata.o kmeans.o space.o rng.o test.cxx
	g++ -o test test.cxx isodata.o kmeans.o space.o rng.o

clean:
	rm -f *.o *~ *.a test
