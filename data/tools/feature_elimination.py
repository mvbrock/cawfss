#!/usr/bin/python

import sys;
import os;

if len(sys.argv) != 3:
	print "usage: ./feature_elimination.py <input filename> <output filename>"
	sys.exit(0)

inputFilename = sys.argv[1]
outputFilename = sys.argv[2]

inputFile = open( inputFilename, "r" )

# Read in the first line to determine the column names and initialize the column totals.
columnNames = inputFile.readline().split(',')
columnTotals = []
for name in columnNames:
	columnTotals.append(0)

# Read in all of the lines of the file.
inputLines = inputFile.readlines()
inputFile.close()

# Read in the file to total up the rows, determining which columns can be discarded.
for line in inputLines:
	columnArray = line.split(',')
	i = 0
	for column in columnArray:
		columnTotals[i] = columnTotals[i] + int(column)
		i = i + 1

# Open the output file
outputFile = open( outputFilename, "w" )

# Output the first row with the discarded columns
i = 0
outputString = ""
for name in columnNames:
	if columnTotals[i] > 0 or i == 0 or i == 1:
		outputString += "" + name + ","
	i += 1
outputFile.write( outputString[:-1] + "\n" )

# Output the rows with the discarded columns.
for line in inputLines:
	columnArray = line.split(',')
	i = 0
	outputString = ""
	for column in columnArray:
		if columnTotals[i] > 0 and i != 0:
			outputString += "" + column + ","
		i += 1
	outputFile.write( outputString[:-1] + "\n" )
	

outputFile.close()
