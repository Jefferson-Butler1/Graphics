/* #include <stdio.h> */
/* #include <math.h> */
/* #include <stdlib.h> */
/* #include  "FPToolkit.c" */
#include "constants.c"

int numObjects ;
int numPoints [MAXOBJS] ;
int numPolys [MAXOBJS] ;
double x[MAXOBJS][MAXPTS] ;
double y[MAXOBJS][MAXPTS] ;
double f[MAXOBJS];

int psize[MAXOBJS][MAXPOLYS] ;
int con[MAXOBJS][MAXPOLYS][20] ;
double red[MAXOBJS][MAXPOLYS],
  grn[MAXOBJS][MAXPOLYS],
  blu[MAXOBJS][MAXPOLYS] ;
double center[MAXOBJS][2];

/**
clipping variables
**/
double clipX[MAXOBJS][MAXPTS] ;
double clipY[MAXOBJS][MAXPTS] ;
int clipSize [MAXOBJS];

int read_file(FILE *w, int c){
  int j =0;
  fscanf(w, "%d", &numPoints[c]);

  for (int i = 0; i < numPoints[c]; i++){
    fscanf(w, "%lf %lf", &x[c][i], &y[c][i]);
  }

  fscanf(w, "%d", &numPolys[c]);

  while (j < numPolys[c]){
    fscanf(w, "%d", &psize[c][j]);
    for (int k = 0; k < psize[c][j]; k++){
      fscanf(w, "%d", &con[c][j][k]);
      int x = con[c][j][k];
    }
    j++;
  }

  for(int k =0; k< numPolys[c]; k++){
    fscanf(w, "%lf %lf %lf", &red[c][k], &grn[c][k], &blu[c][k]);
  }

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

int find_center(int c){
  double xMax, yMax, xMin, yMin;

  xMax=0; xMin=CANVAS_SIZE;
  yMax=0; yMin=CANVAS_SIZE;

  for (int i  = 0; i < numPoints[c]; i++){
    if (xMax < x[c][i]){xMax=x[c][i];}
    if (xMin > x[c][i]){xMin=x[c][i];}
    if (yMax < y[c][i]){yMax=y[c][i];}
    if (yMin > y[c][i]){yMin=y[c][i];}
  }

  double xr = xMax - xMin;
  double yr = yMax - yMin;

  center[c][0] = (xMin + xMax)/2;
  center[c][1] = (yMin + yMax)/2;
  f[c] = ((500/xr)+(500/yr))/2; 
}

int adjust_points(int c){
  double t1[3][3], s1[3][3], t2[3][3],res[3][3];

  M2d_make_translation(t1, -center[c][0],-center[c][1]);
  M2d_make_scaling(s1, f[c],f[c]);
  M2d_make_translation(t2, 400,400);

  M2d_mat_mult(res,t2,s1);
  M2d_mat_mult(res,res,t1);
  M2d_mat_mult_points(x[c],y[c],res,x[c],y[c],numPoints[c]);
  center[c][0] = 400;
  center[c][1] = 400; 
}

int draw(int c){
  int pi=0;
  G_rgb(0,0,0);
  G_clear();
  G_rgb(0,.2,0);
  G_polygon(clipX[c],clipY[c],clipSize[c]);

  double xp[1000], yp[1000];
  for (int k = 0; k < numPolys[c]; k++ ){
    for (int m = 0; m < psize[c][k]; m++){

      xp[m] = x[c][con[c][k][m]];
      yp[m] = y[c][con[c][k][m]];
    }

    pi=psize[c][k];
    if (True ){
      pi = window(xp, yp, psize[c][k],
		  clipX[c], clipY[c], clipSize[c]);
    }

    G_rgb(red[c][k],grn[c][k],blu[c][k]);
    G_fill_polygon(xp, yp,pi);

  }
}

int matrixTranslate(int shapeNumber, double dx, double dy){
  center[shapeNumber][0] += dx;
  center[shapeNumber][1] += dy;
  
  double transformationOne[3][3];
  matrixMakeTranslate(transformationOne, dx, dy);
  M2d_mat_mult_points(x[shapeNumber],y[shapeNumber],
		      transformationOne,
		      x[shapeNumber],y[shapeNumber],
		      numPoints[shapeNumber]);
  
}

int matrixScale(int shapeNumber, double xs, double ys){
  double transformationOne[3][3];
  double transformationTwo[3][3];
  
  matrixMakeTranslate(transformationOne,
		      -center[shapeNumber][0],
		      -center[shapeNumber][1]);
  matrixMakeScale(transformationTwo, xs, ys);
  M2d_mat_mult(transformationOne,transformationTwo,transformationOne);

  matrixMakeTranslate(transformationTwo,
		      center[shapeNumber][0],
		      center[shapeNumber][1]);
  M2d_mat_mult(transformationOne,transformationTwo,transformationOne);
  M2d_mat_mult_points(x[shapeNumber],y[shapeNumber],
		      transformationOne,
		      x[shapeNumber],y[shapeNumber],
		      numPoints[shapeNumber]);
 
}


int matrixRotate(int shapeNumber, double degrees){
  /*translates to origin
    rotates by degrees
    translates back
    applies to points
  */
  double transformationOne[3][3];
  double transformationTwo[3][3];
  M2d_make_identity(transformationOne);
  M2d_make_identity(transformationTwo);
  
  matrixMakeTranslate(transformationOne,
		      -center[shapeNumber][0],
		      -center[shapeNumber][1]);
  matrixMakeRotation(transformationTwo, degrees*M_PI/180);
  M2d_mat_mult(transformationOne,transformationTwo,transformationOne);
  
  matrixMakeTranslate(transformationTwo,
		      center[shapeNumber][0],
		      center[shapeNumber][1]);
  M2d_mat_mult(transformationOne,transformationTwo, transformationOne);
  
  M2d_mat_mult_points(x[shapeNumber],y[shapeNumber],
		      transformationOne,
		      x[shapeNumber],y[shapeNumber],
		      numPoints[shapeNumber]);
}

int getClip(int shapeNumber){
  clipSize[shapeNumber] = click_and_save(clipX[shapeNumber],clipY[shapeNumber]);
}

int main(int argc, char **argv) {
  char list[10][100];
  int noo,i,c, key, num;
  double point[2];
  c=0;
  i=1;
  int realIndex = 0;
  noo = argc;
  key = 'a';

  if (argc>10){printf("too many objects.\n"); exit(0);}

  while (i < argc){
    FILE *w;
    w=fopen(argv[i], "r");
    if (w==NULL){
      printf("unable to open file.\n");
      exit(0);
    }
    read_file(w,c);
    find_center(c);
    adjust_points(c);
    c++;
    i++;
  }

  G_init_graphics (800,800);


  draw(realIndex);
  
  while (key != ' '){
    key = G_wait_key();
    if     (key == 'a'){matrixTranslate(realIndex, -5,0);}
    else if(key == 's'){matrixTranslate(realIndex, 0,-5);}
    else if(key == 'w'){matrixTranslate(realIndex, 0,5);}
    else if(key == 'd'){matrixTranslate(realIndex, 5,0);}
    
    else if(key == 'e'){matrixRotate(realIndex, -5);}
    else if(key == 'q'){matrixRotate(realIndex,  5);}
    
    else if(key == '='){matrixScale(realIndex, 1.05, 1.05);}
    else if(key == '-'){matrixScale(realIndex, 0.95, 0.95);}
    
    else if(key == 'c'){getClip(realIndex);}
    else if(key == '`'){G_close(); main(argc,argv);}

    
    if (key >='0' && key < noo + '0'-1){
      realIndex = key-'0';
    }
    //window(num);
    G_clear() ;

    draw(realIndex);
  }
}



/* good to good = output second good
   good to bad = intersection
   bad to good = intersection + good
   bad to bad = nothing */


