#################################################
## Makefile for the feature selection libraries
#################################################

LIB_NAME = featureselection
INC_DIR = ../../include
LIB_DIR = ../../lib
OBJ_DIR = ../../objects
CXXFLAGS=-g03 -I$(INC_DIR)/clusteranalysis
LDFLAGS=-L$(LIB_DIR) -lclusteranalysis

FILE_SRC = \
	dynamicprogramming.cxx \
	evolutionary.cxx \
	featuredata.cxx \
	featureresults.cxx \
	featureselection.cxx \
	globalsearch.cxx \
	hillclimbing.cxx \
	stepwise.cxx
FILE_HEADERS = $(FILE_SRC:.cxx=.hxx)
FILE_OBJECTS = $(FILE_SRC:.cxx=.o)

all: $(FILE_OBJECTS)
	g++ $(LDFLAGS) $(CXXFLAGS) -fPIC -shared -o $(LIB_DIR)/lib$(LIB_NAME).so $(FILE_OBJECTS) $(LIB_DIR)/libclusteranalysis.a
	ar rcs $(LIB_DIR)/lib$(LIB_NAME).a $(FILE_OBJECTS)
	ranlib $(LIB_DIR)/lib$(LIB_NAME).a
	mkdir -p $(INC_DIR)/$(LIB_NAME)
	mkdir -p $(LIB_DIR)
	mkdir -p $(OBJ_DIR)/$(LIB_NAME)
	cp *.hxx $(INC_DIR)/$(LIB_NAME)
	cp *.o $(OBJ_DIR)/$(LIB_NAME)
	
%.o: %.cxx %.hxx $(FILE_HEADERS)
	g++ $(CXXFLAGS) -c -o $*.o $*.cxx

clean:
	rm -rf $(FILE_OBJECTS)
	rm -rf $(OBJ_DIR)/$(LIB_NAME)
	rm -rf $(INC_DIR)/$(LIB_NAME)
	rm -f $(LIB_DIR)/lib$(LIB_NAME).so $(LIB_DIR)/lib$(LIB_NAME).a
	
	
