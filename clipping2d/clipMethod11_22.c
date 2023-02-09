#include "FPToolkit.c"
#include "tools/intersect.c"
#include "tools/inOuts.c"


int getCenter(double x[], double y[], int numPoints, double center[]){
  double sumX, sumY;
  sumX = sumY = 0;
  for(int i = 0; i < numPoints; i++){
    sumX += x[i];
    sumY += y[i];
  }
  center[0] = sumX / numPoints;
  center[1] = sumY / numPoints;
  return 0;
}

int clipAgainstLine(double polyX[], double polyY[],
		    int polySize,
		    double pointOne[], double pointTwo[],
		    double clipCenter[],
		    double newPolyXReturn[],
		    double newPolyYReturn[]){
  double polyPointOne[2], polyPointTwo[2], intersection[2],
    newPolyX[1000], newPolyY[1000];  
  int newSize = 0; 
  for(int polyPointIndex = 0;polyPointIndex < polySize; polyPointIndex++){
    polyPointOne[0] = polyX[polyPointIndex];
    polyPointOne[1] = polyY[polyPointIndex];
    polyPointTwo[0] = polyX[(polyPointIndex+1)%polySize];
    polyPointTwo[1] = polyY[(polyPointIndex+1)%polySize];
    intersectFourPoints(polyPointOne, polyPointTwo,
			pointOne, pointTwo, intersection);
    
    if(inOut(polyX,polyY,polySize, intersection)){
      newPolyX[newSize] = intersection[0];
      newPolyY[newSize] = intersection[1];
      newSize ++;
      
    }
    else{
      newPolyX[newSize] = polyPointOne[0];
      newPolyY[newSize] = polyPointOne[1];
      newSize ++;
      newPolyX[newSize] = polyPointOne[0];
      newPolyY[newSize] = polyPointOne[1];
      newSize ++;
    }
  }
  for(int i = 0; i < newSize; i++){
    newPolyXReturn[i] = newPolyX[i];
    newPolyYReturn[i] = newPolyY[i];
  }
  printf("Polysize: %d-> newSize: %d\n", polySize, newSize);
  return newSize;
}

int clipMethod(int index, int numpoly,
	       double xPointsPoly[],
	       double yPointsPoly[],
	       double xPointsClip[],
	       double yPointsClip[],
	       double xNew[],
	       double yNew[],
	       int polySize,
	       int clipSize){
  int sizeNew = 0;
  if(clipSize < 3){
    for(int i = 0; i < polySize; i++){
      xNew[i] = xPointsPoly[i];
      yNew[i] = yPointsPoly[i];
    }
    return polySize;
  }
  double clipPointOne[2], clipPointTwo[2],
    intersectionWithClip[2];
  for(int clipPointIndex = 0; clipPointIndex < clipSize; clipPointIndex++){
    clipPointOne[0] = xPointsClip[clipPointIndex];
    clipPointOne[1] = yPointsClip[clipPointIndex];
    clipPointTwo[0] = xPointsClip[(clipPointIndex+1)%clipSize];
    clipPointTwo[1] = yPointsClip[(clipPointIndex+1)%clipSize];
    
    /*Aislinn's name for the center of the clipping poly */
    double ghfbfrtrf [2];
    getCenter(xPointsClip, yPointsClip, clipSize, ghfbfrtrf);
    
    polySize = clipAgainstLine(xPointsPoly, yPointsPoly,polySize,
			       clipPointOne, clipPointTwo ,ghfbfrtrf,
			       xPointsPoly, yPointsPoly);
  }
  for(int i =0; i< polySize; i++){
    xNew[sizeNew] = xPointsClip[i];
    yNew[sizeNew] = yPointsClip[i];
  }
  printf("done clipping, polySize: %d\n", polySize);
  return polySize;
}
