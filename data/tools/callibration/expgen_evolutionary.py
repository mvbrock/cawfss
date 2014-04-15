#!/usr/bin/python

import sys;
import os;

exeLoc = "~/cawfss/featureselection/evolutionary_app"
inputFilename = sys.argv[1]
outputFilePrefix = sys.argv[2]
outputFileSuffix = ".csv"

mutationProbability = "0.5"
survivalPercentage = "0.1"
evTerminationThreshold = "0.0"
maxIterations = "50"
populationSize = "100"
kMin = "20"
kMax = "30"
minFeatSubsetSize = "25"
maxFeatSubsetSize = "35"
clustTerminationThreshold = "0.3"
minClusterSize = "5"
mergeCoeff = "1.0"
splitCoeff = "1.0"

mutationProbArr = [["0.25","L"],["0.50","M"],["0.75","H"]]
survivalPercentArr = [["0.10","L"],["0.20","M"],["0.40","H"]]

expParamArr = []

for mutationProb in mutationProbArr:
	for survivalPercent in survivalPercentArr:
		expParamArr.append( [mutationProb,survivalPercent] )

for expParam in expParamArr:
	outputFilename = outputFilePrefix + expParam[0][1] + expParam[1][1] + outputFileSuffix
	shellCmd = exeLoc + " -f " + inputFilename + " -o " + outputFilename + " -m " + expParam[0][0] + " -s " + expParam[1][0] + \
			" -t " + evTerminationThreshold + " -m " + maxIterations + " -p " + populationSize + " -z " + kMin + \
			" -x " + kMax + " -c " + minFeatSubsetSize + " -v " + maxFeatSubsetSize + " -y " + clustTerminationThreshold + \
			" -e " + minClusterSize + " -j " + mergeCoeff + " -q " + splitCoeff
	print shellCmd

