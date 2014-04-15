#!/usr/bin/python

import sys;
import os;

exeLoc = "~/Downloads/cawfss/featureselection/clusteranalysis/test"
inputFilename = sys.argv[1]
outputFilePrefix = sys.argv[2]
outputFileSuffix = ".csv"
maxIterations = "50"
terminationCondition = "0.0"

kMaxArr = [["10","L"],["100","M"],["1000","H"]]
minCSizeArr = [["10","L"],["100","M"],["1000","H"]]
mergeCoeffArr = [["0.5","L"],["1.0","M"],["2.0","H"]]
splitCoeffArr = [["0.5","L"],["1.0","M"],["2.0","H"]]

expParamArr = []

for kMax in kMaxArr:
	for minCSize in minCSizeArr:
		for mergeCoeff in mergeCoeffArr:
			for splitCoeff in splitCoeffArr:
				expParamArr.append( [kMax, minCSize, mergeCoeff, splitCoeff] )

for expParam in expParamArr:
	outputFilename = outputFilePrefix + expParam[0][1] + expParam[1][1] + expParam[2][1] + expParam[3][1]  + outputFileSuffix
	paramArray = [	"", "-f", inputFilename, "-o", outputFilename, "-k", expParam[0][0], "-c", expParam[1][0],
			"-m", expParam[2][0], "-s", expParam[3][0], "-t", terminationCondition, "-i", maxIterations]
	shellCmd = 	exeLoc + " -f " + inputFilename + " -o " + outputFilename + " -k " + expParam[0][0] + " -c " + \
			expParam[1][0] + " -m " + expParam[2][0] + " -s " + expParam[3][0] + " -t " + \
			terminationCondition + " -i " + maxIterations
	print shellCmd

