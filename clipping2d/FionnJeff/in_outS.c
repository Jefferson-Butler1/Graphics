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



int in_out (double x[], double y[], int n, double P[2]) {
// return 1 if point P is inside the convex polygon
// else return 0

  int flag = 0 ;
  int k = 0 ;
  double dx, dy, ndx, click_math, center_math;

  for(int i = 0; i < n; i++) {
    k = (i + 1) % n ;
    dx = x[k] - x[i] ;
    dy = y[k] - y[i] ;
    ndx = -dx ;
    
    click_math = (dy*P[0]) + (ndx*P[1]) + ((dx*y[i]) - (dy*x[i])) ;
    
    center_math = (dy*C[0]) + (ndx*C[1]) + ((dx*y[i]) - (dy*x[i])) ;
    
    if((click_math * center_math) < 0) {
      flag = 1 ;
      break ;
    }
  }
  return flag ;
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
  
  int tracker = 0;
  
  while(P[1] > 20) {
    tracker = in_out(xp, yp, n, P) ;
    if (tracker != 0) {
      G_rgb(1, 1, 1) ;
    } else {
      G_rgb(0, 0, 0) ;
    }
    G_fill_circle(P[0], P[1], 2) ;
    G_wait_click(P) ;
  }
}
