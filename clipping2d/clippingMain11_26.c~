#include "constants.c"
#include "clipMethod11_26.c"
/**
Oct23
7:30 pm
*/

/**
drawing and moving variables
**/

int numObjects ;
int numpoints [MAXOBJS] ;
int numpolys [MAXOBJS] ;
double x[MAXOBJS][MAXPTS] ;
double y[MAXOBJS][MAXPTS] ;

int psize[MAXOBJS][MAXPOLYS] ;
int con[MAXOBJS][MAXPOLYS][20] ;
double red[MAXOBJS][MAXPOLYS],grn[MAXOBJS][MAXPOLYS],blu[MAXOBJS][MAXPOLYS] ;
double center[MAXOBJS][2];

/**
clipping variables
**/
double clipX[MAXOBJS][MAXPTS] ;
double clipY[MAXOBJS][MAXPTS] ;
int clipSize [MAXOBJS];
double centerOfMass[MAXOBJS][2];


Bool boundObject(int shapeNumber){
  /**
     find bounds
  **/
  
  /*set working variables*/
  double points = numpoints[shapeNumber];
  double xhighest  = 0;
  double xlowest = CANVAS_SIZE;
  double yhighest  = 0;
  double ylowest = CANVAS_SIZE;
  double currentX,currentY;

  /*find min and max of x and y*/
  for(int i=0; i<points; i++){
    currentX = x[shapeNumber][i];
    currentY = y[shapeNumber][i];
    if(currentX < xlowest)   {xlowest  = currentX;}
    if(currentX > xhighest)  {xhighest = currentX;}
    if(currentY < ylowest)   {ylowest  = currentY;}
    if(currentY > yhighest)  {yhighest = currentY;}	 
  }
  /*write ORIGIONAL center of object */
  center[shapeNumber][0] = (xhighest + xlowest)/2;
  center[shapeNumber][1] = (yhighest + ylowest)/2;
  
  
  /*bounds of shape's origional points*/
  double realXDim = xhighest - xlowest;
  double realYDim = yhighest - ylowest;
  double relevantDim = fmax(realXDim,realYDim);
  
  double xOffset = relevantDim-realXDim/2;// + xObjectCenter;
  double yOffset = relevantDim-realYDim/2;// + yObjectCenter;
  
  /*find scale of for object, leaving room for padding*/
  double scale = (CANVAS_SIZE - DEFAULT_PADDING*2)  / ( relevantDim  ) ;
  
  /*scale to right size, translate to center, apply to points*/
  double transformationOne[3][3];
  double transformationTwo[3][3];
  makeIdentity(transformationOne);
  makeIdentity(transformationTwo);
  matrixMakeTranslate(transformationOne,
		  -center[shapeNumber][0],
		  -center[shapeNumber][1]);
  center[shapeNumber][0] -= center[shapeNumber][0];
  center[shapeNumber][1] -= center[shapeNumber][1];
  
  matrixMakeScale(transformationTwo, scale, scale);
  M2d_mat_mult(transformationOne,
	       transformationTwo,
	       transformationOne);
  matrixMakeTranslate(transformationTwo,
		  (CANVAS_SIZE/2)-center[shapeNumber][0],
		  (CANVAS_SIZE/2)-center[shapeNumber][1]);
  center[shapeNumber][0] = (CANVAS_SIZE/2)-center[shapeNumber][0];
  center[shapeNumber][1] = (CANVAS_SIZE/2)-center[shapeNumber][1];
  
  M2d_mat_mult(transformationOne,
	       transformationTwo,
	       transformationOne);
  
  M2d_mat_mult_points(x[shapeNumber],y[shapeNumber],
		      transformationOne,
		      x[shapeNumber],y[shapeNumber],
		      numpoints[shapeNumber]);
}

int read_object(FILE *f,int index){
  int i,j,k ;
  // point info
  fscanf(f,"%d",&numpoints[index]) ;
  if (numpoints[index] >= MAXPTS) {
    // need an extra for object centering
    printf("MAXPTS = %d :  exceeded.\n",MAXPTS) ;
    exit(1) ;
  }
  for (i = 0 ; i < numpoints[index] ; i++) {
    fscanf(f,"%lf %lf",&x[index][i],&y[index][i]) ;
  }
  // connectivity info
  fscanf(f,"%d",&numpolys[index]) ;
  if (numpolys[index] > MAXPOLYS) {
    printf("MAXPOLYS = %d :  exceeded.\n",MAXPOLYS) ;
    exit(1) ;
  }
  k = 0 ;
  for (i = 0 ; i < numpolys[index] ; i++) {
    fscanf(f,"%d",&psize[index][i]) ;
    for (j = 0 ; j < psize[index][i] ; j++) {
      fscanf(f,"%d",&con[index][i][j]) ;
    } // end for j
  } // end for i
    // color info :
  for (i = 0 ; i < numpolys[index] ; i++) {
    fscanf(f,"%lf %lf %lf",&red[index][i],
	   &grn[index][i],&blu[index][i]) ;
  }
}

int print_object (FILE *fout, int index)
{
  int i,j,k ;

  fprintf(fout, "%d\n",numpoints[index]) ;

  for (i = 0 ; i < numpoints[index] ; i++) {
    fprintf(fout, "%12.6lf %12.6lf\n",x[index][i],y[index][i]) ;
  }
  
  k = 0 ;
  for (i = 0 ; i < numpolys[index] ; i++) {
    fprintf(fout, "%3d    ",psize[index][i]) ;

    for (j = 0 ; j < psize[index][i] ; j++) {
      fprintf(fout, "%2d ", con[index][i][j]) ;
    }
    fprintf(fout, "\n") ;
  }


  for (i = 0 ; i < numpolys[index] ; i++) {
    fprintf(fout,"%lf %lf %lf\n",red[index][i],grn[index][i],blu[index][i]) ;
  }      
}
//oneIsBad(shapeNumber, newSize, pointOne, pointTwo, intersection, xNew, yNew, true);


/* int oneIsBad(int shapeNumber, int newSize, */
/* 	     double pointOne[2], double pointTwo[2], */
/* 	     double intersection [2], double xNew[], */
/* 	     double yNew[], Bool firstIsBad){ */
/*   //G_line(pointOne[0], pointOne[1], pointTwo[0], pointTwo[1]); */
/*   double clipOne[2], clipTwo[2]; */
/*   for (int firstClip = 0; firstClip < clipSize[shapeNumber]; firstClip++){ */
/*     int nextClip = (firstClip+1)%clipSize[shapeNumber]; */

/*     /\*get clip points*\/ */
/*     clipOne[0] = clipX[shapeNumber][firstClip]; */
/*     clipOne[1] = clipY[shapeNumber][firstClip]; */
/*     clipTwo[0] = clipX[shapeNumber][nextClip]; */
/*     clipTwo[1] = clipY[shapeNumber][nextClip]; */
/*     double xCo, yCo, cCo; */
/*     /\*finds line for clipping*\/ */
/*     xCo = clipOne[0]-clipTwo[1]; */
/*     yCo = clipTwo[0]-clipOne[0]; */
/*     cCo = clipOne[0]*clipTwo[1] - clipTwo[0]* clipOne[1]; */

/*     printf("clipping Line: %lfx + %lfy + %lf\n", xCo,yCo,cCo); */
/*     /\*tests if points are on the correct side*\/ */
/*     Bool signControl  = (xCo*centerOfMass[shapeNumber][0] + yCo*centerOfMass[shapeNumber][0] + cCo) >= 0; */
/*     printf("sign control: %d\n",signControl); */
    
/*     Bool isGoodPointOne  = ((xCo*pointOne[0] + yCo*pointOne[1] + cCo) >= 0) == signControl; */
/*     printf("isGoodPointOne: %lf\n",((xCo*pointOne[0] + yCo*pointOne[1] + cCo))); */

/*     Bool isGoodPointTwo  = ((xCo*pointTwo[0] + yCo*pointTwo[1] + cCo) >= 0) == signControl; */
/*     printf("isGoodPointTwo: %lf\n",(xCo*pointTwo[0] + yCo*pointTwo[1] + cCo)); */

/*     printf("pointOne:%d, pointTwo:%d\n",isGoodPointOne,isGoodPointTwo); */

    
/*     intersectFourPoints(pointOne, pointTwo,clipOne, clipTwo, intersection); */
/*     G_fill_circle(intersection[0], intersection[1], 5); */
/*     if(!isGoodPointTwo){ */
/*       printf("adding pointOne and the intersection\n"); */
/*       xNew[newSize] = pointOne[0]; */
/*       yNew[newSize] = pointOne[1]; */
/*       newSize++; */
/*       printf("newSize: %d, \nxNew[%d]:%lf\nyNew[%d]:%lf\n", */
/* 	     newSize,newSize, xNew[newSize], newSize, yNew[newSize]); */
/*       xNew[newSize] = intersection[0]; */
/*       yNew[newSize] = intersection[1]; */
/*       newSize++; */
/*       printf("newSize: %d, \nxNew[%d]:%lf\nyNew[%d]:%lf\n", */
/* 	     newSize,newSize, xNew[newSize], newSize, yNew[newSize]); */
/*       intersection[0] = 0; */
/*       intersection[1] = 0; */
/*       return newSize; */
/*     } */
/*     else if(!isGoodPointOne){ */
/*       printf("adding intersection and pointTwo\n"); */

/*       xNew[newSize] = pointTwo[0]; */
/*       yNew[newSize] = pointTwo[1]; */
/*       newSize++; */
/*       printf("newSize: %d, \nxNew[%d]:%lf\nyNew[%d]:%lf\n", */
/* 	     newSize,newSize, xNew[newSize], newSize, yNew[newSize]); */
/*       xNew[newSize] = intersection[0]; */
/*       yNew[newSize] = intersection[1]; */
/*       newSize++; */
/*      printf("newSize: %d, \nxNew[%d]:%lf\nyNew[%d]:%lf\n", */
/* 	     newSize,newSize, xNew[newSize], newSize, yNew[newSize]);  */
/*       intersection[0] = 0; */
/*       intersection[1] = 0; */
/*       return newSize; */
/*     } */
    
    
    
	  
/*   } */
/* } */

/* /\* int cutShape(int shapeNumber, int polyNumber, *\/ */
/* /\* 	     double xPoints[], double yPoints[], *\/ */
/* /\* 	     int polySize, double xNew[], double yNew[]){ *\/ */
/* /\*   /\\** *\/ */
/* /\* iterate through shape sides  *\/ */
/* /\* if side(SEGMENT) is intersected by cut(LINE) draw circle at intersection  *\/ */
/* /\* set end of side to be at intersection  *\/ */
/* /\*   **\\/ *\/ */
  
/* /\*   if(clipSize[shapeNumber] <3){ *\/ */
/* /\*     for(int i = 0; i < polySize; i++){ *\/ */
/* /\*       xNew[i] = xPoints[i]; *\/ */
/* /\*       yNew[i] = yPoints[i]; *\/ */
/* /\*     } *\/ */
/* /\*     return polySize;   *\/ */
/* /\*   } *\/ */
/* /\*   printf("\n\nBegin Clipping!\nClip Size:%d\n",clipSize[shapeNumber]); *\/ */

/* /\*   /\\*each side of polygon to clip*\\/ *\/ */
/* /\*   double pointOne[2], pointTwo[2]; *\/ */
  
/* /\*   int newSize = 0; *\/ */
/* /\*   printf("\npoly number: %d\n", polyNumber); *\/ */
/* /\*   for(int cornerOne = 0; cornerOne < polySize; cornerOne++){ *\/ */
/* /\*     int cornerTwo = (cornerOne+1)%polySize; *\/ */
/* /\*     pointOne[0] = xPoints[cornerOne]; *\/ */
/* /\*     pointOne[1] = yPoints[cornerOne]; *\/ */
    
/* /\*     pointTwo[0] = xPoints[cornerTwo]; *\/ */
/* /\*     pointTwo[1] = yPoints[cornerTwo]; *\/ */

/* /\*     printf("\n\n\n CornerOne:%d\n",cornerOne); *\/ */
/* /\*     printf("pointOne: (%lf,%lf)\n",pointOne[0],pointOne[1]); *\/ */
/* /\*     printf("pointTwo: (%lf,%lf)\n",pointTwo[0],pointTwo[1]); *\/ */
/* /\*     //G_line(pointOne[0], pointOne[1], pointTwo[0], pointTwo[1]); *\/ */
/* /\*     Bool first = inOut(clipX[shapeNumber],clipY[shapeNumber], *\/ */
/* /\* 		       clipSize[shapeNumber],pointOne); *\/ */
/* /\*     Bool second = inOut(clipX[shapeNumber],clipY[shapeNumber], *\/ */
/* /\* 			clipSize[shapeNumber],pointTwo); *\/ */
/* /\*     if(first && second){ *\/ */
/* /\*       /\\*WORKS*\\/ *\/ */
/* /\*       printf("going from point %d to point %d, both are good\n\n",cornerOne, cornerTwo); *\/ */
/* /\*       xNew[newSize] = pointTwo[0]; *\/ */
/* /\*       yNew[newSize] = pointTwo[1]; *\/ */
/* /\*       printf("newSize: %d, \nxNew[%d]:%lf\nyNew[%d]:%lf\n", *\/ */
/* /\* 	     newSize,newSize, xNew[newSize], newSize, yNew[newSize]); *\/ */
/* /\*       newSize++; *\/ */
/* /\*     } *\/ */
/* /\*     else if( first && !second ){ *\/ */
/* /\*       printf("going from point %d to point %d, second is bad\n\n",cornerOne, cornerTwo); *\/ */
      
/* /\*       double intersection [2]; *\/ */
/* /\*       double clipOne[2], clipTwo[2]; *\/ */
            
/* /\*       oneIsBad(shapeNumber, newSize, pointOne, pointTwo, intersection, xNew, yNew, true); *\/ */
      
/* /\*     } *\/ */
/* /\*     else if(!first && second ){ *\/ */
/* /\*       printf("going from point %d to point %d, first is bad\n\n",cornerOne, cornerTwo); *\/ */
/* /\*       double intersection [2]; *\/ */
/* /\*       double clipOne[2], clipTwo[2]; *\/ */
      
/* /\*       oneIsBad(shapeNumber, newSize, pointOne, pointTwo, intersection, xNew, yNew, false); *\/ */
/* /\*     } *\/ */
/* /\*     else if(!first && !second ){ *\/ */
/* /\*       printf("going from point %d to point %d, both are bad\n\n",cornerOne, cornerTwo); *\/ */
/* /\*     } *\/ */
/* /\*     else{ *\/ */
/* /\*       printf("something went terribly horribly wrong\n"); *\/ */
/* /\*     } *\/ */
    
   
/* /\*   } *\/ */
/* /\*   for(int i =0; i< newSize; i++){ *\/ */
    
/* /\*     printf("x[%d]: %lf\n", i,xNew[i]); *\/ */
/* /\*     printf("y[%d]: %lf\n", i,yNew[i]); *\/ */
/* /\*   } *\/ */
/* /\*   printf("%d\n", newSize); *\/ */
/* /\*   return newSize; *\/ */
/* /\* } *\/ */

int draw_object (int index)
{
  G_rgb(0,0,0);
  G_rectangle(0,0,800,800);
  G_clear();
  G_rgb(.5,.5,.5);
  G_line(0,CANVAS_SIZE/2,CANVAS_SIZE,CANVAS_SIZE/2);
  G_line(CANVAS_SIZE/2,0,CANVAS_SIZE/2,CANVAS_SIZE);
  G_rgb(0,0,0) ;
  
  if(PRINT_CENTER)printf("center at: (%.2lf, %.2lf)\n",center[index][0],center[index][1]);
  
  int h,i,j ;
  double xp[100],yp[100];
  double newX[100], newY[100];
  int np ;
  for (i = 0 ; i < numpolys[index] ; i++) {
    np = psize[index][i] ;
    for (j = 0 ; j < np ; j++) {
      h = con[index][i][j] ;
      xp[j] = x[index][h] ;
      yp[j] = y[index][h] ;
    }
    G_rgb(red[index][i], grn[index][i], blu[index][i]) ;
    
    int newSize = clipMethod(index,i,xp,yp,
			     clipX[index],
			     clipY[index],
			     newX, newY,
			     np, clipSize[index]) ;
    //G_wait_key();
    G_rgb(red[index][i], grn[index][i], blu[index][i]) ;

    G_fill_polygon(newX,newY,newSize) ;
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
		      -center[object][0],
		      -center[object][1]);
  matrixMakeRotation(transformationTwo, degrees*M_PI/180);
  M2d_mat_mult(transformationOne,transformationTwo,transformationOne);
  
  matrixMakeTranslate(transformationTwo,
		      center[object][0],
		      center[object][1]);
  M2d_mat_mult(transformationOne,transformationTwo, transformationOne);
  
  M2d_mat_mult_points(x[object],y[object],
		      transformationOne,
		      x[object],y[object],
		      numpoints[object]);
 
}

//CHANGE order of parameters
int clip(double xp[100], double yp[100],int size, int shapeNumber, int polyNumber){
  /* for each polygon, truncate given xp and yp bounds*/
  for(int i =0; i < numpolys[shapeNumber]; i++){
    
    clipMethod(shapeNumber, i,xp, yp,
	       clipX[shapeNumber],
	       clipY[shapeNumber],
	       xp, yp,
	       psize[shapeNumber][polyNumber],
	       clipSize[shapeNumber]);
  }
  G_rgb(.1,.2,.1);
  //G_fill_polygon(xp,yp,sides) ;
  if(PRINT_CLIPPING)printf("end Clipping \n");
  G_wait_key();
}

int getClip(int shapeNumber){
  if(PRINT_CLIPPING)printf("begin Clipping\n");
  clipSize[shapeNumber] = click_and_save(clipX[shapeNumber],clipY[shapeNumber]);
}


int main(int argc, char ** argv)
{
  
  FILE *fin ;
  int key,w ;
  char fname[100] ;
  
  if(argc==1){
    printf("How many objects would you like to draw?\n");
    scanf("%d", &numObjects);
    
    for(int i = 0; i<numObjects; i++){
      printf("enter name of %dth xy file ", i+1) ;
      scanf("%s",fname);
      fin = fopen(fname,"r") ;
      if (fin == NULL) {
	printf("can't read file, %s\n",fname) ;
	i--;
	continue;
      }
      read_object(fin,i) ;    
    }
  }
  else{
    for(int i =1; i<argc; i++){
      numObjects = argc - 1 ;
      fin = fopen(argv[i],"r") ;
      if (fin == NULL) {
	printf("can't read file, %s\n",fname) ;
	i--;
	continue;
      }
      printf("read file %d from Command Line \n", i);
      read_object(fin,i-1) ;
      boundObject(i-1);
    }
      
  }

  
  G_init_graphics(CANVAS_SIZE,CANVAS_SIZE) ;
  G_rgb(.2,.2,.2);
  G_line(0,CANVAS_SIZE/2,CANVAS_SIZE,CANVAS_SIZE/2);
  G_line(CANVAS_SIZE/2,0,CANVAS_SIZE/2,CANVAS_SIZE);



  // drawobject(0) ;
  // allow user to click the clipping window
  // double clipx[100],clipy[100] ; int clipsize  ... global

  
  G_rgb(0,0,0) ;
  key = 49;
  int realIndex;
  while(key != ' '){
    if('0' <= key && key <= '9') {
      int w = key-'1';
      if (w < numObjects) { realIndex = w ; }
    }
    
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
    
    if(realIndex < numObjects){
       G_clear() ;
       G_rgb(0,0,0) ;
       draw_object(realIndex);
    }
    G_rgb(0,0,0);
    key = G_wait_key();
  }
}


