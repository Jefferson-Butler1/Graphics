#include "FPToolkit.c"



/*Constants*/

int CANVAS_WIDTH = 800;
int CANVAS_HEIGHT = 800;
int DRAWING_DIMENSIONS = 2;
int BOX_WIDTH = 20;
int BOX_HEIGHT = 20;
int NUM_POINTS = 1000;
Bool debug = True;
Bool WAIT = False;
Bool GRID = False;

void initGraphics(){
  
  G_rgb(1,0,0);
  G_fill_rectangle(0,0,CANVAS_WIDTH, 10);
  
  G_rgb(1,1,1);
  if(GRID){
    /*horizontal lines */
    for(int i = 0; i<CANVAS_HEIGHT; i+=BOX_WIDTH){
   
      G_line(i,0,i,CANVAS_WIDTH);
    }
    /*vertical Lines */
    for(int i = 0; i<CANVAS_WIDTH; i += BOX_HEIGHT){
   
      G_line(0,i,CANVAS_HEIGHT,i);
    }
  }

}

void print_poly(double x[], double y[], int n)
{
  int i ;
  printf("\n") ;
  printf("%d\n",n) ;
  for (i = 0 ; i < n ; i++) {
    printf("%7.2lf %7.2lf\n",x[i],y[i]) ;
  }
  printf("\n") ;
}


double polygon_perimeter (double x[], double y[], int n)
{
  double a,b,c,p ;
  int i,j ;
  
  p = 0.0 ;
  for (i = 0 ; i < n ; i++) {
    j = i+1 ; if (j == n) { j = 0 ; }
    a = x[j] - x[i] ; b = y[j] - y[i] ;
    c = sqrt(a*a + b*b) ;
    p += c ;
  }
  
  return p ;
}
void snap(double point[]){
  for(int i = 0; i < DRAWING_DIMENSIONS; i++){
  point[i]= round(point[i]/BOX_HEIGHT)*BOX_HEIGHT;
  }
}


int click_and_save(double xvalues[], double yvalues[] ){
  initGraphics();
  G_rgb(.3,.3,.3);
  Bool keepGoing = True;
  int numSides = 0;
  double nextclick[DRAWING_DIMENSIONS];
  /*get first point*/
  G_wait_click(nextclick);
  //snap(nextclick);
  /*get next points*/ 
  for (int i = 0; i<NUM_POINTS && keepGoing ; i++){
    
    if(nextclick[1]>20){
      
      xvalues[i] = nextclick[0];
      yvalues[i] = nextclick[1];
      if(i>0){
	G_line(xvalues[i-1], yvalues[i-1], nextclick[0], nextclick[1]);
      }
      numSides ++;
      G_fill_circle(nextclick[0], nextclick[1], 5 );
      G_wait_click(nextclick);
      //snap(nextclick);
      
    }
    else{
      G_line(xvalues[0], yvalues[0], xvalues[numSides-1], yvalues[numSides-1]);
      if(debug){
	printf("shape Drawn\n");
      }
      
      keepGoing = False;
      
    }
    
  }
  xvalues[numSides] = xvalues[0];
  yvalues[numSides] = yvalues[0];
  return numSides;
}

Bool between(double y1, double y2, int i){
  if(debug){
    printf("\n\nBegin Between\n\ny1:%lf\ny2:%lf\n i:%d\n", y1,y2,i);
  }
  /*check which of the first two is larger*/
  if(y1 <y2){
    /*check if third val is between first two*/
    if(y1<=i && i <=y2){
      if(debug){
	printf("Between\n");
      }
      return True;
    }
  }
  
  else {
    /*check if third val is between or equal first two*/
    if(y2<=i && i <=y1){
      if(debug){
       printf("Between\n");
      }
      
      return True;
    }
  }
  if(debug){
    printf("Not between\n");
  }
  return False;
}

void makeLine(double line[2],double points[4]){
  /*find slope*/
  double m = (points[3]-points[1])/(points[3]-points[0]);
  /*find intercept*/
  double b = points[1] - m*points[0];
  /*set values*/ 
  line[0] = m;
  line[1] = b;
  return;
  
}

Bool betweenVals(double target, double boundOne, double boundTwo){
  return((boundOne < target) && (target < boundTwo)||
	 (boundTwo < target) && (target < boundOne));
}
/*NOT WORKING*/
Bool intersectLine(){
  //error
}
double intersect(double x1, double y1, double x2, double y2, int i){
  if(debug){
    printf("\n\nBegin intersect\n\nx1: %f, \ny1: %f, \nx2: %f, \ny2: %f, \ni: %d\n\n", x1,y1,x2,y2,i);
  }
  double xCoord;
  double yCoord;
  yCoord = i;
  /*
find difference in heights to find proportions 
find top point 
y == mx+b
x = (y+b)/m
from two points:
slope = y2-y1/
        x2-x1
y1-m*x1 = b 
   */
  /*find slope*/
  double m = (y2-y1)/(x2-x1);
  /*find intercept*/
  double b = y1 - m*x1;
  
  xCoord = (yCoord -b)/m;

  G_rgb(1,0,0);
  //G_fill_circle(xCoord, yCoord, 2);
  G_rgb(0,1,0);
  //G_line(x1,y1,x2,y2);
  if(debug){
    //printf("attempting to intersect\n");
    //printf("function for line is y = %fx+%f\n\n", m, b);
    printf("xCoord: %f\n", xCoord);
  }
  return xCoord;
}

/* Function to sort an array using insertion sort
Courtesy of https://www.geeksforgeeks.org/insertion-sort/
Not origional work!!
does have some modifications from the origional, so it can 
accept double arrays instead of ints  */
void insertionSort(double arr[], int n)
{
    int i, j;
    double key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
  
        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void drawBetween(double xVals [], int points, int yVal){
  
  if     (points == 1){G_fill_circle(xVals[0], yVal, 2);}
  else if(points == 0){  }
  else if(points != 1){
    for(int pointIndex = 0; pointIndex < points-1; pointIndex +=2){
      G_line(xVals[pointIndex], yVal, xVals[pointIndex + 1], yVal);
    }
  }
}

void myFillPolygon(double xp[], double yp[], int sides)
{
  for(double yPixel = 0.1; yPixel < CANVAS_HEIGHT; yPixel++){
    
    double xVals[sides];
    int numPoints = 0;
    
    for( int pointOne =0; pointOne < sides; pointOne ++){
      
      int pointTwo = (pointOne+1)%sides;
      
      if(between(yp[pointOne],yp[pointTwo], yPixel)){
	if(xp[pointOne] != xp[pointTwo]){
	  xVals[numPoints++] =
	    intersect(xp[pointOne],
		      yp[pointOne],
		      xp[pointTwo],
		      yp[pointTwo],
		      yPixel);
	}
      }
    }
    insertionSort(xVals, numPoints);
    drawBetween(xVals, numPoints, yPixel);    
  }
}
int test02()
{
  double xp[1000],yp[1000],p1 ;
  int m ;
 
  double P[2] ;

  
  G_rgb(1,0,0) ;
  m = click_and_save(xp,yp) ;
  print_poly(xp,yp,m);
 
  /* p1 = polygon_perimeter (xp,yp,m) ; */
  
  /* printf("p1 = %lf  \n",p1) ; */

  myFillPolygon(xp,yp,m) ;
 
  
  G_wait_key() ;
  
  print_poly(xp,yp,m) ;
 
	     
}



