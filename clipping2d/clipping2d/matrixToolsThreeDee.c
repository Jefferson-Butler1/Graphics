#include <stdio.h>
#include <math.h>
#include "constants.c"
#define DRAWING_DIMENSION DRAWING_DIMENSIONS
#define makeIdentity makeIdentity1
/*


 ( x')          (x)
 ( y')  =   M * (y)  
 ( z')          (z)
 ( 1 )          (1)

instead of (x',y',z',1) = (x,y,z,1) * M  

*/




int printMatrix (double a[DRAWING_DIMENSION+1][DRAWING_DIMENSION+1])
{
  int r,c ;
  for (r = 0 ; r < DRAWING_DIMENSION+1 ; r++ ) {
      for (c = 0 ; c < DRAWING_DIMENSION +1 ; c++ ) {
           printf(" %12.4lf ",a[r][c]) ;
      }
      printf("\n") ;
  }

  return 1 ;
} 





int copyMatrix (double a[DRAWING_DIMENSION+1][DRAWING_DIMENSION+1],
		  double b[DRAWING_DIMENSION+1][DRAWING_DIMENSION+1])
// a = b
{
  int r,c ;
  for (r = 0 ; r < DRAWING_DIMENSION+1 ; r++ ) {
      for (c = 0 ; c < DRAWING_DIMENSION+1 ; c++ ) {
           a[r][c] = b[r][c] ;
      }
  }

  return 1 ;
} 





int makeIdentity (double a[DRAWING_DIMENSION+1][DRAWING_DIMENSION+1])
// a = I
{
  int r,c ;
  for (r = 0 ; r < DRAWING_DIMENSION+1 ; r++ ) {
      for (c = 0 ; c < DRAWING_DIMENSION+1 ; c++ ) {
           if (r == c) a[r][c] = 1.0 ;
               else    a[r][c] = 0.0 ;
      }
  }

  return 1 ;
} 





int makeTranslation (double a[DRAWING_DIMENSION+1][DRAWING_DIMENSION+1],
		     double dx, double dy, double dz)
{
  makeIdentity(a) ;
  a[0][DRAWING_DIMENSION] = dx ;
  a[1][DRAWING_DIMENSION] = dy ;
  a[2][DRAWING_DIMENSION] = dz ;
  return 1 ;
}





int makeScaling (double a[DRAWING_DIMENSION+1][DRAWING_DIMENSION+1],
		 double sx, double sy, double sz)
{
  makeIdentity(a) ;
  a[0][0] =  sx ;  a[1][1] = sy ;  a[2][2] = sz ;
  return 1 ;
}

double sine(int degrees){
  return sin(degrees*M_PI/180);
}

double cosine(int degrees){
  return cos(degrees* M_PI/180);
}

int makeXRotationCS (double a[DRAWING_DIMENSION+1][DRAWING_DIMENSION+1],
		     double cs, double sn)
// this one assumes cosine and sine are already known
{
  makeIdentity(a) ;

  a[1][1] =   cs ;  a[1][2] = -sn ;
  a[2][1] =   sn ;  a[2][2] =  cs ;

  return 1 ;
}



int makeYRotationCS (double a[DRAWING_DIMENSION+1][DRAWING_DIMENSION+1],
		     double cs, double sn)
// this one assumes cosine and sine are already known
{
  makeIdentity(a) ;

  a[0][0] =   cs ;  a[0][2] =  sn ;
  a[2][0] =  -sn ;  a[2][2] =  cs ;

  return 1 ;
}


int makeZRotationCS (double a[DRAWING_DIMENSION+1][DRAWING_DIMENSION+1],
		     double cs, double sn)
// this one assumes cosine and sine are already known
{
  makeIdentity(a) ;

  a[0][0] =   cs ;  a[0][1] = -sn ;
  a[1][0] =   sn ;  a[1][1] =  cs ;

  return 1 ;
}





int matrixMultiply (double res[DRAWING_DIMENSION+1][DRAWING_DIMENSION+1],
		    double a[DRAWING_DIMENSION+1][DRAWING_DIMENSION+1],
		    double b[DRAWING_DIMENSION+1][DRAWING_DIMENSION+1])
// res = a * b
// this is SAFE, i.e. the user can make a call such as 
// M3d_mat_mult(p,  p,q) or M3d_mat_mult(p,  q,p) or  M3d_mat_mult(p, p,p)
{
  double sum ;
  int r,c,k;
  double tmp[DRAWING_DIMENSION+1][DRAWING_DIMENSION+1] ;

  for (r = 0 ; r < DRAWING_DIMENSION+1 ; r++ ) {
      for (c = 0 ; c < DRAWING_DIMENSION+1 ; c++ ) {
	sum = 0.0 ;
	for (k = 0 ; k < DRAWING_DIMENSION+1 ; k++) {
	  sum = sum + a[r][k]*b[k][c] ;
	}
	tmp[r][c] = sum ;
      }
  }
  copyMatrix(res,tmp) ;
  return 1 ;
}

double vectorCrossProduct(double A[3], double B[3]){
  return ((A[1] * B[2] - A[2] * B[1]) * (A[1] * B[2] - A[2] * B[1]) +
	  (A[0] * B[2] - B[0] * A[2]) * (A[0] * B[2] - B[0] * A[2]) +
	  (A[0] * B[1] - A[1] * B[0]) * (A[0] * B[1] - A[1] * B[0]));
}

int vectorPrint(double A[DRAWING_DIMENSION]){
  printf("<");
  for( int i = 0; i< DRAWING_DIMENSION; i++){
    printf(" %lf,", A[i]);
  }
  printf(">\n");
  return 0;
}
double vectorMagnitude(double A[DRAWING_DIMENSION]){
  double magnitude = 0;
  for(int i = 0; i<DRAWING_DIMENSION; i++){
    magnitude += A[i]* A[i];
  }
  magnitude = sqrt(magnitude);
  return magnitude;
}

int vectorNormalize(double A[DRAWING_DIMENSION],
		    double B[DRAWING_DIMENSION]){
  double magnitudeA = vectorMagnitude(A);
  if(magnitudeA == 0){ return 0;}
  for(int i = 0; i <DRAWING_DIMENSION; i++){
    B[i] =  A[i]/magnitudeA;
  }
  return 1;
}

int normalize(double A[], double B[]){
  return(vectorNormalize(A,B));
}

int vectorScale(double A[DRAWING_DIMENSION],
		double scale,
		double B[DRAWING_DIMENSION]){
  for( int i = 0; i <DRAWING_DIMENSION; i++){
    B[i] = A[i]* scale;
  }
  return 0;
}
int vectorInvert(double a[DRAWING_DIMENSION],
		 double b[DRAWING_DIMENSION]){
  for(int i = 0; i <DRAWING_DIMENSION; i++){
    b[i] = -a[i];
  }
  return 0;
}

double vectorDotProduct(double a[DRAWING_DIMENSION],
			double b[DRAWING_DIMENSION]){
  double sum = 0;
  for(int i = 0; i < DRAWING_DIMENSION; i++){
    sum += a[i]*b[i];
  }
  return sum;
}

int vectorSubtract(double A[DRAWING_DIMENSION],
		   double B[DRAWING_DIMENSION],
		   double result[DRAWING_DIMENSION]){
  
  for(int i = 0; i < DRAWING_DIMENSION; i++){
    result[i] = A[i] - B[i];
  }
  return 0;
}
int vectorAdd(double A[DRAWING_DIMENSION],
		   double B[DRAWING_DIMENSION],
		   double result[DRAWING_DIMENSION]){
  
  for(int i = 0; i < DRAWING_DIMENSION; i++){
    result[i] = A[i] + B[i];
  }
  return 0;
}

int matrixMultiplyByPoint (double P[3],   double m[4][4], double Q[3])
// P = m*Q
// SAFE, user may make a call like M3d_mat_mult_pt (W, m,W) ;
{
  double u,v,t ;

  u = m[0][0]*Q[0] + m[0][1]*Q[1] + m[0][2]*Q[2] + m[0][3] ;
  v = m[1][0]*Q[0] + m[1][1]*Q[1] + m[1][2]*Q[2] + m[1][3] ;
  t = m[2][0]*Q[0] + m[2][1]*Q[1] + m[2][2]*Q[2] + m[2][3] ;  

  P[0] = u ;
  P[1] = v ;
  P[2] = t ;
  
  return 1 ;
}





/* int matrixMultiplyPoints(double *X, double *Y, double *Z, */
/*                          double m[DRAWING_DIMENSION+1][DRAWING_DIMENSION+1], */
/*                          double *x, double *y, double *z, int numpoints) */
/* // |X0 X1 X2 ...|       |x0 x1 x2 ...| */
/* // |Y0 Y1 Y2 ...| = m * |y0 y1 y2 ...| */
/* // |Z0 Z1 Z2 ...|       |z0 z1 z2 ...|   */
/* // | 1  1  1 ...|       | 1  1  1 ...| */

/* // SAFE, user may make a call like M3d_mat_mult_points (x,y,z,  m, x,y,z,  n) ; */
/* { */
/*   double u,v,t ; */
/*   int i ; */

/*   for (i = 0 ; i < numpoints ; i++) { */
/*     u = m[0][0]*x[i] + m[0][1]*y[i] + m[0][2]*z[i] + m[0][3] ; */
/*     v = m[1][0]*x[i] + m[1][1]*y[i] + m[1][2]*z[i] + m[1][3] ; */
/*     t = m[2][0]*x[i] + m[2][1]*y[i] + m[2][2]*z[i] + m[2][3] ;     */

/*     X[i] = u ; */
/*     Y[i] = v ; */
/*     Z[i] = t ; */
/*   } */

/*   return 1 ; */
/* } */

double matrixDotProduct(double a[DRAWING_DIMENSION],
			double b[DRAWING_DIMENSION]){
  return(vectorDotProduct(a,b));
}



/* int matrixCrossProduct (double res[DRAWING_DIMENSION], */
/* 		    double a[DRAWING_DIMENSION], */
/* 		    double b[DRAWING_DIMENSION]) */
/* // res = a x b  , cross product of two vectors */
/* // SAFE: it is ok to make a call such as */
/* // D3d_x_product (a,  a,b) or */
/* // D3d_x_product (b,  a,b) or */
/* // D3d_x_product (a,  a,a)  */
/* { */
/*     double r[DRAWING_DIMENSION] ; */
/*     int v = 0; */
    
/*     r[0] = a[1]*b[2] - b[1]*a[2] ; */
/*     r[1] = b[0]*a[2] - a[0]*b[2] ; */
/*     r[2] = a[0]*b[1] - b[0]*a[1] ; */

/*     res[0] = r[0] ; */
/*     res[1] = r[1] ; */
/*     res[2] = r[2] ; */

/*     for(int i = 0; i < DRAWING_DIMENSION; i++){ */
/*       v+= res[i]*1; */
/*     } */
    
/*     if(v!=0) v = 1; */
    
/*     return v ; */
/* } */
