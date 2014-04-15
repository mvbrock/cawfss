#################################################
## Makefile for the cluster analysis libraries
#################################################


LIB_NAME = clusteranalysis
INC_DIR = ../../include
LIB_DIR = ../../lib
OBJ_DIR = ../../objects

CXXFLAGS=-g03

FILE_SRC = \
	cluster.cxx \
	clusterdata.cxx \
	coordinate.cxx \
	datagen.cxx \
	isodata.cxx \
	kmeans.cxx \
	refcntr.cxx \
	space.cxx \
	rng.cxx
FILE_HEADERS = $(FILE_SRC:.cxx=.hxx)
FILE_OBJECTS = $(FILE_SRC:.cxx=.o)

all: $(FILE_OBJECTS)
	g++ $(CXXFLAGS) -fPIC -shared -o $(LIB_DIR)/lib$(LIB_NAME).so $(FILE_OBJECTS)
	ar rcs $(LIB_DIR)/lib$(LIB_NAME).a $(FILE_OBJECTS)
	ranlib $(LIB_DIR)/lib$(LIB_NAME).a
	mkdir -p $(INC_DIR)/$(LIB_NAME)
	mkdir -p $(LIB_DIR)
	mkdir -p $(OBJ_DIR)/$(LIB_NAME)
	cp *.hxx *.h $(INC_DIR)/$(LIB_NAME)
	cp *.o $(OBJ_DIR)/$(LIB_NAME)
	
%.o: %.cxx %.hxx
	g++ $(CXXFLAGS) -c -o $*.o $*.cxx

clean:
	rm -rf $(FILE_OBJECTS)
	rm -rf $(OBJ_DIR)/$(LIB_NAME)
	rm -rf $(INC_DIR)/$(LIB_NAME)
	rm -f $(LIB_DIR)/lib$(LIB_NAME).so $(LIB_DIR)/lib$(LIB_NAME).a
	
