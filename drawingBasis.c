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

void initGraphics(int width, int height){
  G_init_graphics(width,height);
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

