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
	colnames( dataMatrix ) = c( "Evaluation", "ClusterCount" )
	dataMatrix
}

interpretDependentMatrix = function( file )
{
	fileData = read.csv( file )
	dataMatrix = as.matrix( fileData )
	
	evaluationVector = dataMatrix[,1]
	clusterCountVector  = dataMatrix[,2]

	evaluation = min( evaluationVector )
	clusterCount = min( clusterCountVector )

	rbind( c( evaluation, clusterCount ) )
}

independentMatrix = function( dir )
{
	fileList = getFileList( dir )
	startIdx = nchar( fileList[1] ) - 5
	endIdx = nchar( fileList[1] ) - 4
	typeList = substr(fileList,startIdx,endIdx)

	dataMatrix = matrix();

	for( i in 1:length(typeList) )
	{
		mutationCoeff = switch( substr(typeList[i],1,1), L = 10, M = 100, H = 1000 )
		survivalProb = switch( substr(typeList[i],2,2), L = 10, M = 100, H = 1000 )		
		
		dataRow = rbind(c( mutationCoeff, survivalProb ))

		if( is.na( dataMatrix[1,1] ) == FALSE )
		{
			dataMatrix = rbind( dataMatrix, dataRow )		
		}
		else
		{
			dataMatrix = dataRow;
		}
	}
	colnames( dataMatrix ) = list( "MutationCoefficient", "SurvivalProbability" )
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

directory1 = "../csv/callibration/evolutionary_callibration_rand1"
directory2 = "../csv/callibration/evolutionary_callibration_rand2"
directory3 = "../csv/callibration/evolutionary_callibration_rand3"
directory4 = "../csv/callibration/evolutionary_callibration_rand4"

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

mutationCoeff = indMatSc[,1]
populationSize = indMatSc[,2]
print(indMatSc)
print(depMatScAvg)
summary(aov(depMatAvg~mutationCoeff*populationSize))

#cbind(indMat,depMat)


