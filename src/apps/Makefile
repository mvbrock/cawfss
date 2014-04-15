#################################################
## Makefile for the feature selection libraries
#################################################

APP_NAMES = \
	ca_app \
	evolutionary_app \
	globalsearch_app \
	hillclimbing_app \
	stepwise_app
BIN_DIR = ../../bin
INC_DIR = ../../include
LIB_DIR = ../../lib
OBJ_DIR = ../../objects/apps

CXXFLAGS=-g03 -I$(INC_DIR)/clusteranalysis -I$(INC_DIR)/featureselection

FILE_SRC = \
	ca_app.cxx \
	evolutionary_app.cxx \
	globalsearch_app.cxx \
	hillclimbing_app.cxx \
	stepwise_app.cxx
FILE_HEADERS = $(FILE_SRC:.cxx=.hxx)
FILE_OBJECTS = $(FILE_SRC:.cxx=.o)

all: $(APP_NAMES)
	
%: %.cxx
	g++ $(CXXFLAGS) $(LDFLAGS) -o $* $*.cxx $(LIB_DIR)/libfeatureselection.a $(LIB_DIR)/libclusteranalysis.a 
	mv $* $(BIN_DIR)

clean:
	rm -f $(BIN_DIR)/*