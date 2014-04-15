#################################################
## Makefile for the entire project
#################################################

all:
	make -C clusteranalysis
	make -C featureselection
	make -C apps

clusteranalysis:
	make -C clusteranalysis

featureselection:
	make -C featureselection

apps:
	make -C apps

clean:
	make -C clusteranalysis clean
	make -C featureselection clean
	make -C apps clean

package: clean
	tar zcf ../../cawfss-`date +%Y%m%d`.tar.gz ../../cawfss
