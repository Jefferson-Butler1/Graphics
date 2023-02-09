#include "intersect.c"

int main(){
  int hither = 0, yon = 100, halfAngle = 45;
  double clipPlanes[6][4] = {
    {7, 8, 9,        -hither},
    {0, 0, 1,           -yon},
    {0, 1, tan(halfAngle), 0},
    {0, 1,-tan(halfAngle), 0},
    {1, 0, tan(halfAngle), 0},
    {-528, 12,138, 3108}
  };

  double goodPoints[6][3] = {
    {0,0,hither},
    {0,0,yon},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {10,20,14}
  };
  double startPoints[6][3] = {
    {1,2,3},
    {0,1,2},
    {0,1,3},
    {0,1,4},
    {0,1,5},
    {3,6,2}
  };
  double unitVectors[6][3] = {
    {4,5,6},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {4,9,10}
  };


  
  double intersection [3];
  
  /* for(int i = 0; i < 6; i++){ */
  /*   intersectP(clipPlanes[i], */
  /* 		       startPoints[i],unitVectors[i], */
  /* 		       intersection, */
  /* 		       goodPoints[i]); */
  /*   printf("intersection at (%lf,%lf,%lf)\n", */
  /* 	   intersection[0], */
  /* 	   intersection[1], */
  /* 	   intersection[2]); */
  /* } */
  int i = 5;
  /* intersectionPoint(unitVectors[i], */
  /* 		 startPoints[i], */
  /* 		 clipPlanes[i], */
  /* 		 goodPoints[i], */
  /* 		 intersection); */
  
  /* printf("intersection at (%lf,%lf,%lf)\n", */
  /* 	 intersection[0], */
  /* 	 intersection[1], */
  /* 	 intersection[2]); */
  
  /* lineXPlane(intersection, */
  /* 	     startPoints[i], unitVectors[i], */
  /* 	     clipPlanes[i], */
  /* 	     goodPoints[i]); */
  
  /* printf("intersection at (%lf,%lf,%lf)\n", */
  /* 	 intersection[0], */
  /* 	 intersection[1], */
  /* 	 intersection[2]); */
  intersect(clipPlanes[i],
	     startPoints[i], unitVectors[i],
	     intersection);
  
  printf("intersection at (%lf,%lf,%lf)\n",
	 intersection[0],
	 intersection[1],
	 intersection[2]);
}
