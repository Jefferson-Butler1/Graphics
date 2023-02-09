#include "constants.c"
#include "findPerpendicularVector.c"
#include "clippingMethod.c"


double windowX[4] = {50,  50, 750, 750};
double windowY[4] = {50, 750, 750,  50};
int windowSize = 4;


int assert(Bool list[], int length){
  for(int i = 0; i < length; i++){
    //printf("%d",list[i]);
    if(!list[i]){
      printf("assertion Error\n");
      //G_wait_key();
    }
  }
  
  return 0;
}
int normalizeObject(double xp[], double yp[], double zp[], double np){
  int max = 0;
  for(int i =0; i < np; i++){
    if(xp[i] > max){max = xp[i];}
    if(yp[i] > max){max = yp[i];}
    if(zp[i] > max){max = zp[i];}
  }
  for (int i = 0; i < np; i++){
    xp[i] /= max;
    yp[i] /= max;
    zp[i] /= max;
  }
  return 0;
}
int getCenter(double xp[], double yp[], double zp[],
	      int np, double center[3]){
  center[0] = 0;
  center[1] = 0;
  center[2] = 0;
  
  for(int i = 0; i< np; i++){
    center[0] += xp[i];
    center[1] += yp[i];
    center[2] += zp[i];
  }
  center[0] /= np;
  center[1] /= np;
  center[2] /= np;
  return 0;
}
double getLighting(double lightLoc[3],
		   double ambient,
		   double maxDiffuse,
		   double specPow,
		   double xp[],
		   double yp[],
		   double zp[],
		   int np,
		   double centerOfObj[]
		   ){
  normalizeObject(xp, yp, zp, np);
  double observer[3] = {0,0,0};
  double pointOne[3] = {xp[0], yp[0], zp[0]};
  double pointTwo[3] = {xp[1], yp[1], zp[1]};
  double pointThree[3] = {xp[2], yp[2], zp[2]};
  double centerOfPoly[3], Lu[3], Nu[3], Eu[3], Ru[3],
    relationalCenterOfObject[3];
  
  getCenter(xp,yp,zp, np, centerOfPoly);
  
  findPerpendicularVector(pointOne, pointTwo,pointThree, Nu);
  vectorSubtract(lightLoc, centerOfPoly, Lu);
  vectorSubtract(observer, centerOfPoly, Eu);
  
  vectorNormalize(Nu, Nu);
  vectorNormalize(Lu, Lu);
  vectorNormalize(Eu, Eu);
  
  vectorScale(Nu, 2*vectorDotProduct(Nu, Lu), Ru);
  vectorSubtract(Ru, Lu, Ru);
  vectorNormalize(Ru, Ru);
  
  double diffuse  = maxDiffuse*vectorDotProduct(Nu, Lu);
  double specular = (1-(ambient + maxDiffuse))*
    pow(vectorDotProduct(Eu, Ru),specPow);
  double intensity = ambient + diffuse + specular;
  if(intensity < ambient){
    return ambient;
  }
  printf("intensity: %lf\n", intensity);
  /* Bool assertions [] = {0<=intensity, intensity <= 1}; */
  /* assert(assertions, 2); */

  return intensity;
}

double planarizePoint(double XY, double Z){
  return ((XY/Z)*400) +400;
}

int planarize(double xp[], double yp[], double zp[],
	      int points,
	      double planarX[], double planarY[]){
  double tempX [points], tempY[points];
  for(int i = 0; i < points; i++){
    planarX[i] = planarizePoint(xp[i], zp[i]);
    planarY[i] = planarizePoint(yp[i], zp[i]);
  }
  return 1;
}

int getAllPolyPoints(double allPolys[][DRAWING_DIMENSION],
		     double x [MAXOBJS][MAXPTS],
		     double y [MAXOBJS][MAXPTS],
		     double z [MAXOBJS][MAXPTS],
		     int numPolys[MAXOBJS],
		     int psize[MAXOBJS][MAXPOLYS],
		     int con[MAXOBJS][MAXPOLYS][20],
		     int numObjects){
  int h,i,j,index ;
  double xp [100] ,yp [100], zp[100] ;
  double xbb[100], ybb[100] ;
  int np ;
  int polyNumber = 0;
  double distance = 0;
  int negative = 1;
  
  for(index = 0; index < numObjects; index++){
    for (i = 0 ; i < numPolys[index] ; i++) {
      np = psize[index][i] ;
      allPolys[polyNumber][0] = index ;
      allPolys[polyNumber][1] = i ;
      
      for (j = 0 ; j < np ; j++) {
	h = con[index][i][j] ;
	
	distance += sqrt(pow(x[index][h],2) +
			 pow(y[index][h],2) +
			 pow(z[index][h],2));
	if(z[index][h] < -10){
	  negative = -1;
	}
      }
      allPolys[polyNumber][2] = (distance*negative) / np;
      polyNumber++;
      distance = 0;
    }
  }
  return polyNumber;
}

static int compare(const void *a, const void *b)
{
    const double * const ia = a, * const ib = b;
    if(ia[2] < ib[2]) return 1;
    if(ia[2] > ib[2]) return -1;

    return 0;
}

int drawSinglePoly(int index, int poly, int signal,
		   double x [MAXOBJS][MAXPTS],
		   double y [MAXOBJS][MAXPTS],
		   double z [MAXOBJS][MAXPTS],
		   int psize[MAXOBJS][MAXPOLYS],
		   int con[MAXOBJS][MAXPOLYS][20],
		   double color[][3],
		   double lightLoc[],
		   double center[3],
		   double hither){
  printf("hither: %lf\n", hither);

  double xp [100] ,yp [100], zp[100] ;
  double xbb[100], ybb[100], intensity, f ;
  int np, h, j;
  int backFace;

  np = psize[index][poly] ;
  for (j = 0 ; j < np ; j++) {
    h = con[index][poly][j] ;
    xp[j] = x[index][h];
    yp[j] = y[index][h];
    zp[j] = z[index][h];
  }
  backFace = signal; //isBackFace(xp, yp, zp, np);
  double ambient = .2, maxDiffuse = .5, specPow = 50;
  //printf("getLighting\n");
  if(backFace == signal){
    intensity = getLighting(lightLoc, ambient,
			    maxDiffuse, specPow,
			    xp, yp, zp, np, center);
    double ar = color[index][0],
      ag = color[index][1],
      ab = color[index][2];
    /* double ar = 0, ag = 0, ab = 0; */
    f = ambient + maxDiffuse;
    if(intensity <= f){
      f = intensity/f;
      ar = color[index][0] * f;
      ag = color[index][1] * f;
      ab = color[index][2] * f;	
    }else if(intensity == f){
      ar = color[index][0]*intensity,
      ag = color[index][1]*intensity,
      ab = color[index][2]*intensity;
    }    
    else{

      f = (intensity-f)/(1-f);
      ar = color[index][0] + f*(1-color[index][0]);
      ag = color[index][1] + f*(1-color[index][1]);
      ab = color[index][2] + f*(1-color[index][2]);
    }
    printf("Entering clipping\n");
    printf("hither: %lf\n", hither);

    clipAgainstPlanes(xp, yp, zp, np, hither);
    /* for( int i = 0; i < np; i++){ */
    /*   if(zp[i] < hither){ */
    /* 	np = 0; */
    /* 	printf("behind clipping Plane :(\n"); */
    /*   } */
    /* } */
    planarize(xp,yp, zp, np, xbb, ybb);
    G_rgb(ar,ag, ab);
    
    G_fill_polygon(xbb,ybb,np) ;
    G_rgb(0,0,0);
    G_polygon(xbb,ybb,np);
  }
  return 1;
}

int drawSingleObject (int index, int signal,
		      double x [MAXOBJS][MAXPTS],
		      double y [MAXOBJS][MAXPTS],
		      double z [MAXOBJS][MAXPTS],
		      int numPolys[MAXOBJS],
		      int psize[MAXOBJS][MAXPOLYS],
		      int con[MAXOBJS][MAXPOLYS][20],
		      double color[][3],
		      double lightLoc[],
		      double centers[][3])
{
  /* for (int i = 0 ; i < numPolys[index] ; i++) { */
  /*   drawSinglePoly(index, i, signal, */
  /* 		   x, y, z, psize, con, color, */
  /* 		   lightLoc, centers[i], hither); */
  /* } */
  return 1;
}

int paintersAlgorithm(double allPolys[][3],
		      int numPolys,
		      int signal[],
		      double x [MAXOBJS][MAXPTS],
		      double y [MAXOBJS][MAXPTS],
		      double z [MAXOBJS][MAXPTS],
		      int psize[MAXOBJS][MAXPOLYS],
		      int con[MAXOBJS][MAXPOLYS][20],
		      double color[][3],
		      double lightLoc[],
		      double centers[][3],
		      double hither){
  printf("PaintersAlgorithm hither: %lf\n", hither);

  qsort(allPolys, numPolys, sizeof *allPolys, compare);
  
  for(int i = 0; i < numPolys; i++){
    if(allPolys[i][2] > 0){
      
      drawSinglePoly(allPolys[i][0],
		     allPolys[i][1],
		     signal[(int)allPolys[i][0]],
		     x, y, z, psize, con, color,
		     lightLoc, centers[(int)allPolys[i][0]],
		     hither);
    }
  }
  return 1;
}
