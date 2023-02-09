#include "tools/inOuts.c"
#include "tools/intersect.c"

int printVector(double vector[]){
  printf("<%lf,%lf>\n", vector[0], vector[1]);
}

int in_out (int clipIndex, double P[2],
	    double clipX[], double clipY[], int clipSize){
	      
  double a, b, c,cy,cx, cv, pv,center[2], signs[2];
  cx=cy=0;
  int j;
  j = (clipIndex+1)%clipSize ; 


  a=  (clipY[j]-clipY[clipIndex]);
  b= -(clipX[j]-clipX[clipIndex]);
  c= -b*clipY[clipIndex]-a*clipX[clipIndex];
  
  center[0]=0;
  center[1]=0;
  for (int i = 0; i < clipSize; i++){
    center[0] += clipX[i];
    center[1] += clipY[i];
  }
  center[0] = center[0]/clipSize;
  center[1] = center[1]/clipSize;


  signs[0] = a*center[0] + b*center[1] + c;
  signs[1] =  a*P[0] + b*P[1] + c;

  if (signs[0]*signs[1] < 0){
    return 0;
  }

  return 1;

}

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
  //y = (a1*c2-a2*c1)/(a2*b1-a1*b2)
  return 1;
  
  
}

int  window (double polyX[], double polyY[], int polySize,
	     double clipX[], double clipY[], int clipSize)

{
  int signal[2],
    polyIndexModified,si,sizeNew,m,
    clipIndexModified,
    pointOneInClip,pointTwoInClip;
  double  xNew[100], yNew[100], intersection[2],
    polyPointOne[2], polyPointTwo[2],
    clipPointOne[2], clipPointTwo[2];
  
  sizeNew=0;

  for (int clipIndex = 0; clipIndex < clipSize; clipIndex++){
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
      
      pointOneInClip = in_out(clipIndex,polyPointTwo,
			      clipX, clipY, clipSize);
      pointTwoInClip = in_out(clipIndex,polyPointOne,
			      clipX, clipY, clipSize);
      

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
