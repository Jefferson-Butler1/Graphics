#include "matrixToolsThreeDee.c"

// Function to find out whether they
// are parallel, perpendicular or neither
/*Return 1 if they are parallel to each other,
       2 if they are perpendicular to each other or
       0 otherwise. */
int perpendicular(double A[], double B[])
{
// Find A.B
  int dot = vectorDotProduct(A,B);
  // Find A X B
  int cross = vectorCrossProduct(A,B);
  
  if      (  dot == 0)  return 2;
  else if (cross == 0)  return 1;

  return 0;
}

int lineXPlane(double intersection[], double vector[],
		double point[], double normal[],
		double planePoint[]) {
    // get d value
    double d = vectorDotProduct(normal, planePoint);
    if (vectorDotProduct(normal, vector) == 0) {
        return 0;
	// No intersection, the line is parallel to the plane
    }
    // Compute the X value for the
    //directed line ray intersecting the plane
    double x = ((d - vectorDotProduct(normal, point)) /
	       vectorDotProduct(normal,vector));
    // output contact point
    normalize(vector, vector);
    vectorScale(vector, x, vector);
    vectorAdd(point, vector, intersection);
      
    //Make sure your ray vector is normalized
    return 1;
}

	     
int intersect_2_lines (double A[2], double B[2],
                       double C[2], double D[2],
                       double intersection[2]){
  double a1,b1,a2,b2,c1,c2;
  a1 = b1 = a2 = b2 = c1 = c2 = 0;
  a1=-(A[1]-B[1]);
  b1=A[0]-B[0];
  a2=-(C[1]-D[1]);
  b2=C[0]-D[0];
  c1= -b1*A[1]-a1*A[0];
  c2= -b2*C[1]-a2*C[0];
  
  if ((a1*b2-a2*b1)==0){return 0;}
  if(b1 == 0) b1 = 0.00001;
  intersection[0]= (c1*b2-c2*b1)/(a2*b1-a1*b2);
  intersection[1]= (a1*intersection[0] + c1)/-b1;
  //y = (a1*c2-a2*c1)/(a2*b1-a1*b2)
  return 1;  
}
