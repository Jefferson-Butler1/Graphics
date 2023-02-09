#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include  "FPToolkit.c"
#include "constants.c"

int numObjects ;
int numpoints [MAXOBJS] ;
int numpolys [MAXOBJS] ;
double x[MAXOBJS][MAXPTS] ;
double y[MAXOBJS][MAXPTS] ;

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

double wcenter[2], wx[59000], wy[59000],
  polygony[10][59000],polygonx[10][59000],
  r[10][57500], g[10][57500],b[10][57500],
  cx[10], cy[10], f[10];
int psize[10][57500], con[10][57500][20];
int npt[10],npg[10], wsize,wsw;

int read_file(FILE *w, int c){
  int j =0;
  fscanf(w, "%d", &numpoints[c]);

  for (int i = 0; i < numpoints[c]; i++){
    fscanf(w, "%lf %lf", &x[c][i], &y[c][i]);
  }

  fscanf(w, "%d", &numpolys[c]);

  while (j < numpolys[c]){
    fscanf(w, "%d", &psize[c][j]);
    for (int k = 0; k < psize[c][j]; k++){
      fscanf(w, "%d", &con[c][j][k]);
      int x = con[c][j][k];
    }
    j++;
  }

  for(int k =0; k< numpolys[c]; k++){
    fscanf(w, "%lf %lf %lf", &r[c][k], &g[c][k], &b[c][k]);
  }

}

int build_poly(int c){

  for (int k = 0; k < numpolys[c]; k++ ){

    for (int m = 0; m < psize[c][k]; m++){
      polygonx[c][m] = x[c][con[c][k][m]];
      polygony[c][m] = y[c][con[c][k][m]];
    }
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
  center[0] = center[0]/wsize;
  center[1] = center[1]/wsize;


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
  double gx,gy,lx,ly;

  gx=x[c][0]; lx=x[c][0];
  gy=y[c][0]; ly=y[c][0];

  for (int i  = 1; i < numpoints[c]; i++){
    if (gx < x[c][i]){gx=x[c][i];}
    if (lx > x[c][i]){lx=x[c][i];}
    if (gy < y[c][i]){gy=y[c][i];}
    if (ly > y[c][i]){ly=y[c][i];}
  }

  double xr = gx-lx;
  double yr= gy-ly;

  cx[c] = (lx + (xr/2));
  cy[c] = (ly + (yr/2));
  f[c] = ((400/xr)+(400/yr))/2; 
}

int adjust_points(int c){
  double t1[3][3], s1[3][3], t2[3][3],res[3][3];

  M2d_make_translation(t1, -cx[c],-cy[c]);
  M2d_make_scaling(s1, f[c],f[c]);
  M2d_make_translation(t2, 400,400);

  M2d_mat_mult(res,t2,s1);
  M2d_mat_mult(res,res,t1);
  M2d_mat_mult_points(x[c],y[c],res,x[c],y[c],numpoints[c]);
}

int draw(int c){
  int pi=0;
  G_rgb(0,0,0);
  G_clear();
  G_rgb(0,1,0);
  G_fill_rectangle(0,0,800,20);
  G_polygon(wx,wy,wsize);
  for (int k = 0; k < numpolys[c]; k++ ){
    for (int m = 0; m < psize[c][k]; m++){

      polygonx[c][m] = x[c][con[c][k][m]];
      polygony[c][m] = y[c][con[c][k][m]];
    }

    pi=psize[c][k];
    if (True ){
      pi = window(polygonx[c], polygony[c],psize[c][k],
		  wx, wy, wsize);
    }

    G_rgb(r[c][k],g[c][k],b[c][k]);
    G_fill_polygon(polygonx[c], polygony[c],pi);

  }
}

int rotate(int c, double degrees){
  int temp;
  double rotate[3][3], t1[3][3],t2[3][3];
  double r = degrees * M_PI/180;

  M2d_make_translation(t1, -400,-400);
  M2d_make_rotation(rotate, r); 
  M2d_make_translation(t2, 400,400);

  M2d_mat_mult(t2,t2,rotate);
  M2d_mat_mult(t2, t2,t1);
  M2d_mat_mult_points(x[c],y[c],t2,x[c],y[c],numpoints[c]);

}



int stretch(int c, double dx, double dy){
  double s[3][3];
  M2d_make_scaling(s, dx,dy);
  M2d_mat_mult_points(x[c], y[c], s, x[c],y[c],npt[c]);

}

int translate(int c, int dx, int dy){
  double t[3][3];
  M2d_make_translation(t, dx,dy);
  M2d_mat_mult_points(x[c], y[c], t, x[c],y[c],npt[c]);
}


int matrixTranslate(int shapeNumber, double dx, double dy){
  center[shapeNumber][0] += dx;
  center[shapeNumber][1] += dy;
  
  double transformationOne[3][3];
  matrixMakeTranslate(transformationOne, dx, dy);
  M2d_mat_mult_points(x[shapeNumber],y[shapeNumber],
		      transformationOne,
		      x[shapeNumber],y[shapeNumber],
		      numpoints[shapeNumber]);
  
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
		      numpoints[shapeNumber]);
 
}


int matrixRotate(int object, double degrees){
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
		      -400,
		      -400);
  matrixMakeRotation(transformationTwo, degrees*M_PI/180);
  M2d_mat_mult(transformationOne,transformationTwo,transformationOne);
  
  matrixMakeTranslate(transformationTwo,
		      400,
		      400);
  M2d_mat_mult(transformationOne,transformationTwo, transformationOne);
  
  M2d_mat_mult_points(x[object],y[object],
		      transformationOne,
		      x[object],y[object],
		      numpoints[object]);
}

int getClip(int shapeNumber){
  wsize = click_and_save(wx,wy);
}

int main(int argc, char **argv) {
  wsw=0;
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
  G_rgb(0,1,0);
  
  G_wait_click(point);
  while (point[1] > 20){
    G_fill_circle(point[0],point[1],3);
    wx[wsize]= point[0];
    wy[wsize]= point[1];
    wsize++;
    if (wsize>=2){G_line(wx[wsize-2],wy[wsize-2],wx[wsize-1],wy[wsize-1]);}
    G_wait_click(point);
  }

  G_line(wx[0],wy[0],wx[wsize-1],wy[wsize-1]);
  wsw= 1;
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


