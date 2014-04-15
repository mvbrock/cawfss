#################################################
## Makefile for the entire project
#################################################

all:
	make -C src

clean:
	make -C src clean

package: clean
	tar zcf ../cawfss-`date +%Y%m%d`.tar.gz ../cawfss
