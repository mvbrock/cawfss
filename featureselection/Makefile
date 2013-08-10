################################################
## Makefile for the feature selection program
#################################################

CXX_FLAGS=-g3 -I../

CLUSTERANALYSIS_OBJECTS = \
	../clusteranalysis/kmeans.o \
	../clusteranalysis/kmeans_pyramid.o \
	../clusteranalysis/isodata.o \
	../clusteranalysis/space.o \
	../clusteranalysis/rng.o

FEATURESELECTION_OBJECTS = \
	featureselection.o \
	hillclimbing.o \
	stepwise.o \
	evolutionary.o \
	dynamicprogramming.o


all:
	make -C ../clusteranalysis
	make libfeatureselection.a
	make test

featureselection.o: featureselection.cxx featureselection.hxx
	g++ $(CXX_FLAGS) -c -o featureselection.o featureselection.cxx

hillclimbing.o: hillclimbing.cxx hillclimbing.hxx
	g++ $(CXX_FLAGS) -c -o hillclimbing.o hillclimbing.cxx

stepwise.o: stepwise.cxx stepwise.hxx
	g++ $(CXX_FLAGS) -c -o stepwise.o stepwise.cxx

evolutionary.o: evolutionary.cxx evolutionary.hxx
	g++ $(CXX_FLAGS) -c -o evolutionary.o evolutionary.cxx

dynamicprogramming.o: dynamicprogramming.cxx dynamicprogramming.hxx
	g++ $(CXX_FLAGS) -c -o dynamicprogramming.o dynamicprogramming.cxx

libfeatureselection.a: $(FEATURESELECTION_OBJECTS) $(CLUSTERANALYSIS_OBJECTS)
	ar rc libfeatureselection.a $(FEATURESELECTION_OBJECTS) $(CLUSTERANALYSIS_OBJECTS)
	ranlib libfeatureselection.a

test: test.cxx libfeatureselection.a
	g++ $(CXX_FLAGS) -o test test.cxx libfeatureselection.a

localclean:
	rm -f *.o *~ *.a

clean:
	make clean -C ../clusteranalysis
	rm -f *.o *~ *.a test
