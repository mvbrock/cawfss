.adjustWidth <- function(...){
       options(width=1000)
       TRUE
}

dependentMatrix = function( dir )
{
	fileList = getFileList( dir )
	dataMatrix = matrix()
	for( i in 1:length( fileList ) )
	{
		dataRow = rbind( interpretDependentMatrix( fileList[ i ] ) )
		if( is.na( dataMatrix[1,1] ) == FALSE )
		{
			dataMatrix = rbind( dataMatrix, dataRow );
		}
		else
		{
			dataMatrix = dataRow;
		}
	}
	colnames( dataMatrix ) = c( "EliminateCount", "MergeCount", "SplitCount", "Evaluation", "DistortionDelta","ClusterCount" )
	dataMatrix
}

interpretDependentMatrix = function( file )
{
	fileData = read.csv( file )
	dataMatrix = as.matrix( fileData )
	
	elimCountVector = dataMatrix[,1]
	mergeCountVector = dataMatrix[,2]
	splitCountVector = dataMatrix[,3]
	evaluationVector = dataMatrix[,4]
	distortionVector = dataMatrix[,5]
	clusterCountVector  = dataMatrix[,6]

	elimCount = elimCountVector[ length( elimCountVector ) ]
	mergeCount = mergeCountVector[ length( mergeCountVector ) ]
	splitCount = splitCountVector[ length( splitCountVector ) ]
	evaluation = min( evaluationVector )
	distortion = mean( distortionVector )
	clusterCount = which.min( clusterCountVector )

	rbind( c( elimCount, mergeCount, splitCount, evaluation, distortion, clusterCount ) )
}

independentMatrix = function( dir )
{
	fileList = getFileList( dir )
	startIdx = nchar( fileList[1] ) - 7
	endIdx = nchar( fileList[1] ) - 4
	typeList = substr(fileList,startIdx,endIdx)

	dataMatrix = matrix();

	for( i in 1:length(typeList) )
	{
		kMax = switch( substr(typeList[i],1,1), L = 10, M = 100, H = 1000 )
		minCSize = switch( substr(typeList[i],2,2), L = 10, M = 100, H = 1000 )		
		mergeCrit = switch( substr(typeList[i],3,3), L = .5, M = 1.0, H = 2 )		
		splitCrit = switch( substr(typeList[i],4,4), L = .5, M = 1.0, H = 2 )		
		
		dataRow = rbind(c( kMax, minCSize, mergeCrit, splitCrit ))

		if( is.na( dataMatrix[1,1] ) == FALSE )
		{
			dataMatrix = rbind( dataMatrix, dataRow )		
		}
		else
		{
			dataMatrix = dataRow;
		}
	}
	colnames( dataMatrix ) = list( "KMax", "MinClusterSize", "MergeThreshold", "SplitThreshold" )
	dataMatrix
}

getFileList = function( dir )
{
	cmd = sprintf( "ls %s/*.csv", dir )
	system( cmd, intern=TRUE )
}


plotCorrelations = function( lmResult )
{
	coefTable = as.matrix(coef(lmResult))
	for( i in 1:length(depMat[1,]) )
	{
		plotIntercept = coefTable[1,i]
		for( j in 1:length(indMat[1,]) )
		{
			xLabel = colnames(indMat)[j]
			yLabel = colnames(depMat)[i]
			mainLabel = paste("Plot of ",xLabel," and ",yLabel)
			plot(	depMat[,i]~indMat[,j],
				xlab=xLabel,ylab=yLabel,
				main=mainLabel
				)
			plotSlope = coefTable[j+1,i]
			abline(plotIntercept,plotSlope)
		}
	}
}

getStandardError = function( lmResult )
{
	coefTableArray = coef(summary(lmResult))
	as.matrix( coefTableArray[1] )	
}

.adjustWidthCallBack <- addTaskCallback(.adjustWidth)

directory1 = "../csv/isodata_callibration_rand1/run1"
directory2 = "../csv/isodata_callibration_rand2/run1"
directory3 = "../csv/isodata_callibration_rand3/run1"
directory4 = "../csv/isodata_callibration_rand4/run1"

depMat1 = dependentMatrix(directory1)
depMat2 = dependentMatrix(directory2)
depMat3 = dependentMatrix(directory3)
depMat4 = dependentMatrix(directory4)
depMatAvg = (depMat1 + depMat2 + depMat3 + depMat4)/matrix(4,length(depMat1[,1]),length(depMat1[1,]))

indMat = independentMatrix(directory1)

depMatSc1 = scale(depMat1)
depMatSc2 = scale(depMat2)
depMatSc3 = scale(depMat3)
depMatSc4 = scale(depMat4)
depMatScAvg = scale(depMatAvg)

indMatSc = scale(independentMatrix(directory1))

indMat = rbind( indMat, indMat, indMat, indMat )
depMat = rbind( depMat1, depMat2, depMat3, depMat4 )

#indMatSc = rbind( indMatSc, indMatSc, indMatSc, indMatSc )
#depMatSc = rbind( depMatSc1, depMatSc2, depMatSc3, depMatSc4 )

kMax = indMatSc[,1]
minClusterSize = indMatSc[,2]
mergeThreshold = indMatSc[,3]
splitThreshold = indMatSc[,4]

summary(aov(depMatScAvg~kMax*minClusterSize*mergeThreshold*splitThreshold))

cbind(indMat,depMat)


