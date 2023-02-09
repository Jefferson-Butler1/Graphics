#include "FPToolkit.c"

int swidth, sheight ;
double C[2] ;

int click_and_save (double *x, double *y) {
  int n ;
  double P[2] ;

  G_rgb(0,1,0.5) ;
  G_fill_rectangle(0,0,swidth,20) ;

  G_rgb(1,0,0) ;
  G_wait_click(P);

  n = 0 ;
  while (P[1] > 20) {
    x[n] = P[0] ;
    y[n] = P[1] ;
    G_circle(x[n],y[n],2) ;
    if (n > 0) { G_line(x[n-1],y[n-1], x[n],y[n]) ;}
    n++ ;
    G_wait_click(P) ;
  }

  return n ;
}

void center(double x[], double y[], int n) {
  double sumX, sumY = 0 ;
  for(int i = 0; i < n; i++) {
    sumX += x[i] ;
    sumY += y[i] ;
  }
  C[0] = sumX/n ;
  C[1] = sumY/n ;
  
  G_rgb(0, .5, .5) ;
  G_fill_circle(C[0], C[1], 3) ;
}


int in_out (double clipOne[2], double clipTwo[2],
	    double center[2] , double point[2]) {
// return 1 if point P is inside the convex polygon
// else return 0
  
  int flag = 0;
  double dx, dy, ndx, click_math, center_math;
  
  dx = clipTwo[0] - clipOne[0] ;
  dy = clipTwo[1] - clipOne[1] ;
  ndx = -dx ;
  
  click_math = ((dy*point[0]) + (ndx*point[1]) +
		((dx*clipOne[1]) - (dy*clipOne[0]))) ;
  
  center_math = ((dy*center[0]) + (ndx*center[1]) +
		 ((dx*clipOne[1]) - (dy*clipOne[0]))) ;
  
  if((click_math * center_math) < 0) {
    return 1;
  }
  
  return 0 ;
}




int main() {
  double xp[1000],yp[1000] ;
  int n ;
  double P[2] ;

  swidth = 700 ; sheight = 700 ;
  G_init_graphics(swidth, sheight) ;
  G_rgb(0,0,0) ;
  G_clear() ;

  G_rgb(1,0,0) ;
  n = click_and_save(xp,yp) ;
  G_rgb(0,1,0) ;
  G_fill_polygon(xp,yp,n) ;
  
  center(xp, yp, n) ;
  G_wait_click(P) ;
  
  double pointOne[2];
  double pointTwo[2];
  int tracker = 1;
  int k;
  
  while(P[1] > 20) {
    tracker = 1;
    for(int i = 0; i < n; i++){
      printf("tracker: %d\nCenter: <%lf,%lf>\n", tracker, C[0], C[1] );
      printf("point: <%lf,%lf>\n", P[0], P[1] );
      k = (i + 1) % n;
      pointOne[0] = xp[i];
      pointOne[1] = yp[i];
      
      pointTwo[0] = xp[k];
      pointTwo[1] = yp[k];
      
      tracker *= in_out(pointOne, pointTwo, C, P) ;
    }
    if(tracker != 0){
      G_rgb(1, 0, 0);
    }
    else{
      G_rgb(1, 1, 1);
    }
    G_fill_circle(P[0], P[1], 2) ;
    G_wait_click(P) ;
    printf("\n\nNew Point\n");
  }
}
