#include "FPToolkit.c"
#include "matrixToolsThreeDee.c"

int printVector(double vector [], int num){
  printf("Vector:\n(");
  for(int i = 0; i < num; i++){
    printf("%lf,", vector[i]);
  }
  printf(")\n");
  return 1; 
}

int findPerpendicularVector(double pointOne[3], double pointTwo[3],
			    double pointThree[3], double result[3]){

  double vectorOne[3], vectorTwo[3];
  for (int i = 0; i < 3; i++){
    vectorOne[i] = pointOne[i] - pointThree[i];
    vectorTwo[i] = pointTwo[i] - pointThree[i];
  }
  matrixCrossProduct(result, vectorOne, vectorTwo);
  return 1;
}

int isBackFace(double xp[],double yp[],
	       double zp[], double np){
  double normalVector [3] = {0,0,0};
  double firstThreePoints[3][3]= {{0,0,0},
				  {0,0,0},
				  {0,0,0}};
  for(int i = 0; i<3; i++){
    firstThreePoints[i][0] = xp[i];
    firstThreePoints[i][1] = yp[i];
    firstThreePoints[i][2] = zp[i];
  }
  findPerpendicularVector(firstThreePoints[0], firstThreePoints[1],
			  firstThreePoints[2], normalVector);
  if(matrixDotProduct(firstThreePoints[0], normalVector) >0){
    return 1;
  }
  return -1;
}
