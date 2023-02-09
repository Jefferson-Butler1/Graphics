#include "FPToolkit.c"
#define false False
#define true True 

int swidth, sheight ;

int inOut (double x[], double y[], int n, double p[2])
// return 1 if point P is inside the convex polygon
// else return 0
{
  /*find center of mass point*/
  double avgX, avgY;
  avgX = avgY = 0 ;
  for(int cornerOne = 0; cornerOne < n; cornerOne++){
    avgX += x[cornerOne];
    avgY += y[cornerOne];    
  }
  avgX /= n;
  avgY /= n;
  G_rgb(1,1,1);
  G_fill_circle(avgX,avgY,2);
  double xCo, yCo, cCo;
  //printf("drew circle of center of mass\n");
  for (int cornerOne = 0; cornerOne <n; cornerOne++){
    int cornerTwo = (cornerOne+1)%n;
    xCo = y[cornerOne]-y[cornerTwo];
    yCo = x[cornerTwo]-x[cornerOne];
    cCo = x[cornerOne]*y[cornerTwo]-x[cornerTwo]*y[cornerOne];
    Bool signControl  = (xCo*avgX + yCo*avgY + cCo) >= 0;
    Bool signTest     = (xCo*p[0] + yCo*p[1] + cCo) >= 0;
    if(signControl != signTest){
      return 0;
    }
  }
  return 1;
}
/*(y1 – y2)x + (x2 – x1)y + (x1y2 – x2y1) = 0*/
void clickPoints(double x[], double y[], int n){
  /*loops until click beneath y = 20, 
    draws a circle at the point of either 
    yellow or blue based on return of inOut
  */
  printf("reached clickPoints\n");
  
  double p[2] ;

  G_rgb(0,1,0.5) ;
  G_fill_rectangle(0,0,swidth,20) ;
 
  G_rgb(1,0,0) ;
  G_wait_click(p);

  int i = 0 ;
  double center[2];
  while (p[1] > 20) {
    if(inOut(x,y,n,p)) {G_rgb(1,1,0);}
    else               {G_rgb(0,0,1);}
    G_fill_circle(p[0],p[1],5);
    G_wait_click(p) ;
  }

  return ;
  
}

