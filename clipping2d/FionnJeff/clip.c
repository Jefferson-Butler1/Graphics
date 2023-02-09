#include "FPToolkit.c"

double wx[100],wy[100],wc[2];
int wsize;



void window_center(double c[], double wx[], double wy[], int wsize){
  c[0]=0;
  c[1]=0;
  for (int i =0; i < wsize; i++){
    c[0] += wx[i];
    c[1] += wy[i];
  }
  c[0]=c[0]/wsize;
  c[1]=c[1]/wsize;

}

int in_out (int i, double P[2], double wx[],double wy[], int wsize){

  double b, c, a, cy, cx, cv, pv, cntr[2];
  cx = cy = 0;
  int j;
  //i=0;
  j = (i + 1) % wsize ;


  a=  (wy[j] - wy[i]);
  b= -(wx[j] - wx[i]);
  c= -b*wy[i] - a*wx[i];

  window_center(cntr, wx, wy, wsize);

  G_rgb(0, 1, 0);
  G_fill_circle(cntr[0], cntr[1], 3);

  cv = a*cntr[0] + b*cntr[1] + c;
  pv =  a*P[0] + b*P[1] + c;
  printf("%lf %lf \n", cv, pv);
  if (cv*pv < 0){
    return 0;
  }

  return 1;

}


int intersect_2_lines (double A[2], double B[2],
                       double C[2], double D[2],
                       double intersection[2]){
  double a1,b1,a2,b2,c1,c2;
  
  a1=-(A[1]-B[1]);
  b1=  A[0]-B[0];
  c1= -b1*A[1]-a1*A[0];

  a2=-(C[1]-D[1]);
  b2=  C[0]-D[0];
  c2=-(C[1]*b2-
       C[0]*a2);
  
  if ((a1*b2-a2*b1)==0){return 0;}
  
  intersection[0]=(c1*b2-c2*b1)/(a2*b1-a1*b2);
  intersection[1]= (a1*intersection[0] + c1)/-b1;
  //y = (a1*c2-a2*c1)/(a2*b1-a1*b2)
  return 1;
  
  
}


int  Clip_Polygon_Against_Convex_Window (double px[],  double py[],
					 int psize,
					 double wx[],double wy[],
					 int wsize)

{
  if(wsize < 3){
    printf("clipSize < 3\n");
    return psize;
  }
  int signal[2],k,si,pi,m;
  double p[2],q[2],
    pwx[100],pwy[100],
    a[2],b[2],c[2],d[2],
    intersection[2];
  pi=0;

  for (int i = 0; i < wsize; i++){
    for (int j = 0; j < psize; j++){
      k=j-1;
      if (k == -1){
	k=psize-1;
      }
      //k=0=q
      //j=1=p
      m= i+1;
      if (m==wsize){
	m=0;
      }
      p[0]=px[j];
      p[1]=py[j];
      q[0]=px[k];
      q[1]=py[k];
      signal[0] = in_out(i,q, wx, wy, wsize);
      signal[1] = in_out(i,p, wx, wy, wsize);
      printf("signal = %d %d \n", signal[0], signal[1]);

      //good to good
      if (signal[0] == 1 && signal[1] == 1 ) {
	pwx[pi]=px[j]; pwy[pi]=py[j];
	pi++;
      }
      //good to bad
      if (signal[0] == 1 && signal[1] == 0 ) {
	a[0]=px[k]; a[1]=py[k];
	b[0]=px[j]; b[1]=py[j];
	c[0]=wx[i]; c[1]=wy[i];
	d[0]=wx[m]; d[1]=wy[m];
	si = intersect_2_lines(a,b,c,d, intersection);
	if (si == 1){
	  pwx[pi]= intersection[0]; pwy[pi] = intersection[1];
	  pi++; 
	  printf("1 %lf %lf \n", intersection[0], intersection[1]);}
	else {pwx[pi]=px[j]; pwy[pi]=py[j];pi++;}
      }
      //bad to good
      if (signal[0] == 0 && signal[1] == 1 ) {
	a[0]=px[k]; a[1]=py[k];
	b[0]=px[j]; b[1]=py[j];
	c[0]=wx[i]; c[1]=wy[i];
	d[0]=wx[m]; d[1]=wy[m];
	si = intersect_2_lines(a,b,c,d, intersection);
	if (si == 1){
	  pwx[pi]= intersection[0]; 
	  pwy[pi] = intersection[1];pi++;
	  printf("2 %lf %lf \n", intersection[0], intersection[1]); }
	else {
	  pwx[pi]=px[j]; pwy[pi]=py[j]; pi++;}

	pwx[pi]=px[j];
	pwy[pi]=py[j];
	pi++;
      } 
    }
    for(int r = 0; r<pi; r++){
      px[r]=pwx[r];
      py[r]=pwy[r];
    }
    psize=pi;
    if (i < wsize-1) {pi = 0;}
  }
  return pi;
}


int main()
// this tests clipping of polygon to convex window
{
  int pn, wn,test, t[2];

  double px[100] = {  70,460,400} ;
  double py[100] = { 350, 25,550} ;
  //double px[100] = { 200, 300,400} ;
  //double py[100] = { 200, 300,200} ;

  pn = 3 ;
  wx[0]=100;
  wx[1]=600;
  wx[2]=550;
  wx[3]=150;
  
  wy[0]=150;
  wy[1]=200;
  wy[2]=450;
  wy[3]=500;

  wsize = 4 ;

  srand48(100) ;

  G_init_graphics (700, 700) ;
  G_rgb (0,0,0) ;
  G_clear() ;

  G_rgb (1,0,0) ;
  G_polygon(wx,wy,wsize) ;

  G_rgb (0,0,1) ;
  G_polygon(px,py,pn) ;

  G_rgb(1,1,0);
  G_fill_circle(wx[0],wy[0], 2);
  G_fill_circle(wx[1], wy[1], 4);
  G_fill_circle(wx[2],wy[2] , 6);
  G_fill_circle(wx[3],wy[3] , 8);


  G_fill_circle(px[0],py[0], 2);
  G_fill_circle(px[1],py[1], 4);
  G_fill_circle(px[2],py[2] , 6);
 

  G_wait_key() ;
  G_rgb (0,0,0) ;
  G_clear();
  pn =  Clip_Polygon_Against_Convex_Window (px, py, pn,
					    wx, wy, wsize) ;
  
  G_rgb (1,1,0) ;
  G_fill_polygon(px,py,pn) ;
  G_wait_key() ;
}



