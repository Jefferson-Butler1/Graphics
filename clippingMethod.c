#include "inOut.c"
#include "intersect.c"


int intersect_2_lines (double A[2], double B[2],
                       double C[2], double D[2],
                       double intersection[2]){
  double a1,b1,a2,b2,c1,c2;
  
  a1=-(A[1]-B[1]);
  b1=A[0]-B[0];
  a2=-(C[1]-D[1]);
  b2=C[0]-D[0];
  c1= -b1*A[1]-a1*A[0];
  c2= -b2*C[1]-a2*C[0];
  
  if ((a1*b2-a2*b1)==0){return 0;}
  
  intersection[0]= (c1*b2-c2*b1)/(a2*b1-a1*b2);
  intersection[1]= (a1*intersection[0] + c1)/-b1;
    return 1;  
}

int clip (double polyX[], double polyY[], int polySize,
	  double clipX[], double clipY[], int clipSize)

{

  if(clipSize < 3){
    return polySize;
  }
  int signal[2],polyIndexModified,si,sizeNew,m,clipIndexModified,
    pointOneInClip,pointTwoInClip;
  double  xNew[100],yNew[100],
    intersection[2],
    polyPointOne[2], polyPointTwo[2],
    clipPointOne[2], clipPointTwo[2];
  sizeNew=0;
  double center[2];
  for (int clipIndex = 0; clipIndex < clipSize; clipIndex++){
    center[0]=0;
    center[1]=0;
    for (int i = 0; i < clipSize; i++){
      center[0] += clipX[i];
      center[1] += clipY[i];
    }
    center[0] = center[0]/clipSize;
    center[1] = center[1]/clipSize;
    
    for (int polyIndex = 0; polyIndex < polySize; polyIndex++){
      polyIndexModified=polyIndex-1;
      if (polyIndexModified == -1){
	polyIndexModified = polySize - 1;
      }
      //k=0=q
      //j=1=p
      clipIndexModified = clipIndex+1;
      if (clipIndexModified == clipSize){
	clipIndexModified=0;
      }
      polyPointOne[0]=polyX[polyIndex];
      polyPointOne[1]=polyY[polyIndex];
      
      polyPointTwo[0]=polyX[polyIndexModified];
      polyPointTwo[1]=polyY[polyIndexModified];

      clipPointOne[0]=clipX[clipIndex];
      clipPointOne[1]=clipY[clipIndex];
      
      clipPointTwo[0]=clipX[clipIndexModified];
      clipPointTwo[1]=clipY[clipIndexModified];
      double xCo, yCo, c;
      
      xCo =  (clipY[clipIndexModified]-clipY[clipIndex]);
      yCo = -(clipX[clipIndexModified]-clipX[clipIndex]);
      c   = -yCo*clipY[clipIndex]-xCo*clipX[clipIndex];

      pointOneInClip = inOutEquation(xCo, yCo, 0, c, center, polyPointTwo);
      pointTwoInClip = inOutEquation(xCo, yCo, 0, c, center, polyPointOne);
      
      

      //good to good
      if (pointOneInClip == 1 && pointTwoInClip == 1 ) {
	xNew[sizeNew]=polyX[polyIndex];
	yNew[sizeNew]=polyY[polyIndex];
	sizeNew++;
      }
      //good to bad
      if (pointOneInClip == 1 && pointTwoInClip == 0 ) {
	
	si = intersect_2_lines(polyPointTwo, polyPointOne,
			       clipPointOne, clipPointTwo, intersection);
	if (si == 1){
	  xNew[sizeNew] = intersection[0];
	  yNew[sizeNew] = intersection[1];
	  sizeNew++;
	}

	else {
	  xNew[sizeNew] = polyX[polyIndex];
	  yNew[sizeNew] = polyY[polyIndex];
	  sizeNew++;
	}
      }
      //bad to good
      if (pointOneInClip == 0 && pointTwoInClip == 1 ) {
	
	si = intersect_2_lines(polyPointOne, polyPointTwo,
			       clipPointOne, clipPointTwo, intersection);
	if (si == 1){
	  xNew[sizeNew] = intersection[0]; 
	  yNew[sizeNew] = intersection[1];
	  sizeNew++;
	}
	else {
	  xNew[sizeNew]=polyX[polyIndex];
	  yNew[sizeNew]=polyY[polyIndex];
	  sizeNew++;
	}

	xNew[sizeNew]=polyX[polyIndex];
	yNew[sizeNew]=polyY[polyIndex];
	sizeNew++;
      } 

    }
    for(int r = 0; r<sizeNew; r++){
      polyX[r]=xNew[r];
      polyY[r]=yNew[r];
    }
    polySize=sizeNew;

    if (clipIndex < clipSize-1) {
      sizeNew = 0;
    }

  }
  return sizeNew;
}



int clipAgainstPlanes(double polyX[], double polyY[], double polyZ[],
		      int polySize,
		      double hither ){
  printf("got to clipping\n");
  printf("hither: %lf\n", hither);

  double yon = 1000;
  double halfAngle = 45;
  double clipSize = 6;
  /*planes as a 4x6 array of coefficients x planes*/
  double clipPlanes[6][4] = {
    //    {1,0, 0,        0},
    {0,0, 1,        -hither},
//    {0,0, 1,           -yon},
//    {0,1, tan(halfAngle), 0},
//    {0,1,-tan(halfAngle), 0},
//    {1,0, tan(halfAngle), 0},
//    {1,0,-tan(halfAngle), 0}
  };
  
  int signal[2],polyIndexModified,si,sizeNew,m,clipIndexModified,
    pointOneInClip,pointTwoInClip;
  double  xNew[100],yNew[100], zNew[100],
    intersection[3],
    polyPointOne[3], polyPointTwo[3],
    clipPointOne[3], clipPointTwo[2];
  sizeNew=0;
  double center[3] = {0,0,(hither+yon)/2};
  
  for (int clipIndex = 0; clipIndex < 6; clipIndex++){
    

    for (int polyIndex = 0; polyIndex < polySize; polyIndex++){
      polyIndexModified=polyIndex-1;
      if (polyIndexModified == -1){
	polyIndexModified = polySize - 1;
      }
      //k=0=q
      //j=1=p
      
      polyPointOne[0]=polyX[polyIndex];
      polyPointOne[1]=polyY[polyIndex];
      polyPointOne[2]=polyZ[polyIndex];
	    
      
      polyPointTwo[0]=polyX[polyIndexModified];
      polyPointTwo[1]=polyY[polyIndexModified];
      polyPointTwo[2]=polyZ[polyIndexModified];

      pointOneInClip = inOutEquation(clipPlanes[clipIndex][0],
				     clipPlanes[clipIndex][1],
				     clipPlanes[clipIndex][2],
				     clipPlanes[clipIndex][3],
				     center, polyPointTwo);
      pointTwoInClip = inOutEquation(clipPlanes[clipIndex][0],
				     clipPlanes[clipIndex][1],
				     clipPlanes[clipIndex][2],
				     clipPlanes[clipIndex][3],
				     center, polyPointOne);

      printf("pointOneInClip %d, pointTwoinclip %d \n",
	     pointOneInClip,
	     pointTwoInClip);
      double unitVector[3] = {0,0,0};
      vectorSubtract(polyPointOne, polyPointTwo, unitVector);
      if(pointOneInClip == 0 && pointTwoInClip == 0){

      }
      //good to good
      else if (pointOneInClip == 1 && pointTwoInClip == 1 ) {
	xNew[sizeNew]=polyX[polyIndex];
	yNew[sizeNew]=polyY[polyIndex];
	zNew[sizeNew]=polyZ[polyIndex];

	sizeNew++;
      }
      //good to bad
      else if (pointOneInClip == 1 && pointTwoInClip == 0 ) {
	
	si = intersect(clipPlanes[clipIndex],
		       polyPointOne,unitVector,
		       intersection);
	if (si == 1){
	  xNew[sizeNew] = intersection[0];
	  yNew[sizeNew] = intersection[1];
	  zNew[sizeNew] = intersection[2];
		
	  sizeNew++;
	}

	else {
	  xNew[sizeNew] = polyX[polyIndex];
	  yNew[sizeNew] = polyY[polyIndex];
	  zNew[sizeNew]=polyZ[polyIndex];

	  sizeNew++;
	}
      }
      //bad to good
      else if (pointOneInClip == 0 && pointTwoInClip == 1 ) {
	
	si = intersect(clipPlanes[clipIndex],
		       polyPointOne,unitVector,
		       intersection);
	if (si == 1){
	  xNew[sizeNew] = intersection[0]; 
	  yNew[sizeNew] = intersection[1];
	  zNew[sizeNew] = intersection[2];

	  sizeNew++;
	}
	else {
	  xNew[sizeNew]=polyX[polyIndex];
	  yNew[sizeNew]=polyY[polyIndex];
	  zNew[sizeNew]=polyZ[polyIndex];
	
	  sizeNew++;
	}

	xNew[sizeNew]=polyX[polyIndex];
	yNew[sizeNew]=polyY[polyIndex];
	zNew[sizeNew]=polyZ[polyIndex];
	sizeNew++;
      } 

    }
    for(int r = 0; r<sizeNew; r++){
      polyX[r] = xNew[r];
      polyY[r] = yNew[r];
      polyZ[r] = zNew[r];
    }
    polySize=sizeNew;

    if (clipIndex < clipSize-1) {
      sizeNew = 0;
    }

  }
  return sizeNew;
}
