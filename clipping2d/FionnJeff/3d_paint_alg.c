#include "FPToolkit.c"
#include "M3d_matrix_toolsS.c"

#define MAXOBJS 10
#define MAXPTS 59000
#define MAXPOLYS 57500

#define S_WIDTH 800
#define S_HEIGHT 800

#define DEGREE 5

double RED, GREEN, BLUE ;

int numobjects, rvs ;
double cenX, cenY, cenZ ;
int numpoints[MAXOBJS] ;
int numpolys[MAXOBJS] ;
double x[MAXOBJS][MAXPTS] ;
double y[MAXOBJS][MAXPTS] ;
double z[MAXOBJS][MAXPTS] ;
int psize[MAXOBJS][MAXPOLYS] ;
int con[MAXOBJS][MAXPOLYS][20] ;
double listall[59000][3] ;
double color[3][3] = {{1,0,0},
		 {0,1,0},
		 {0,0,1}};


// creates the display window filter
int film (double xp[], double yp[], double zp[], int pts, double flmX[], double flmY[]) {
  double xTemp[pts], yTemp[pts] ;

  for (int i = 0; i < pts; i++) {
    flmX[i] = ((xp[i]/zp[i]) * 400) + 400 ;
    flmY[i] = ((yp[i]/zp[i]) * 400) + 400 ;
  }
  
}


// creates the center for the screen
void center_it (int objnum) {
  double xAvg, yAvg, zAvg = 0 ;

  for (int i = 0; i < numpoints[objnum]; i++) {
    xAvg += x[objnum][i]  ;
    yAvg += y[objnum][i]  ;
    zAvg += z[objnum][i]  ;
  }

  x[objnum][numpoints[objnum]] = xAvg/numpoints[objnum] ;
  y[objnum][numpoints[objnum]] = yAvg/numpoints[objnum] ;
  z[objnum][numpoints[objnum]] = zAvg/numpoints[objnum] ;

}


// reads object
int read_object(FILE *f, int objnum) {
  int i, j, k ;
  
  fscanf(f,"%d",&numpoints[objnum]) ;

  if (numpoints[objnum] >= MAXPTS) {
    printf("MAXPTS = %d :  exceeded.\n", MAXPTS) ;
    exit(1) ;
  }

  for (i = 0; i < numpoints[objnum]; i++) {
    fscanf(f, "%lf %lf %lf", &x[objnum][i], &y[objnum][i], &z[objnum][i]) ;
  }

  // connectivity info
  fscanf(f, "%d",&numpolys[objnum]) ;
  if (numpolys[objnum] > MAXPOLYS) {
    printf("MAXPOLYS = %d :  exceeded.\n", MAXPOLYS) ;
    exit(1) ;
  }

  k = 0 ;
  for (i = 0; i < numpolys[objnum]; i++) {
    fscanf(f, "%d",&psize[objnum][i]) ;
    
    for (j = 0; j < psize[objnum][i]; j++) {
      fscanf(f, "%d",&con[objnum][i][j]) ;
    } // end for j
  } // end for i

  center_it(objnum) ;
}


// finds the dot product
double dot_product (double v1[], double v2[]) {
  double product = 0 ;

  for (int i = 0; i < 3; i++) {
    product = product + (v1[i] * v2[i]) ;
  }

  return product ;
}

// ---------------------------------------------------------------------------------------
// finds if polygon is on back of object
int remove_back (double xx[], double yy[], double zz[], int n) {
  int signal ;
  double dot ;
  double v1[3], v2[3], temp[3], norm[3] ;

  cenX = cenY = cenZ = 0 ;
  for (int i = 0; i < n; i++) {
    cenX += xx[i] ;
    cenY += yy[i] ;
    cenZ += zz[i] ;
  }

  cenX = cenX/n ; cenY = cenY/n ; cenZ = cenZ/n ;
  
  v1[0] = xx[1] - xx[0] ;   v2[0] = xx[2] - xx[0] ;
  v1[1] = yy[1] - yy[0] ;   v2[1] = yy[2] - yy[0] ;
  v1[2] = zz[1] - zz[0] ;   v2[2] = zz[2] - zz[0] ;

  M3d_x_product(norm, v1, v2) ;
  
  temp[0] = cenX ;
  temp[1] = cenY ;
  temp[2] = cenZ ;
  
  dot = dot_product(norm, temp) * rvs ;

  if (dot > 0) {
    signal = 1 ;
  } else {
    signal = 0 ;
  }
  
  return signal ;
}


// draws single object (mostly works but has some additions)
int draw_single_object (int objnum) {
  int h, i, j, np, signal ;
  double xp[100], yp[100], zp[100] ;
  double xFilm[100], yFilm[100] ;

  for (i = 0; i < numpolys[objnum]; i++) {
    np = psize[objnum][i] ;

    for (j = 0; j < np; j++) {
      h = con[objnum][i][j] ;
      xp[j] = x[objnum][h] ;
      yp[j] = y[objnum][h] ;
      zp[j] = z[objnum][h] ;
    }

    //signal = 1 ;
    signal = remove_back(xp, yp, zp, np) ;    
    film(xp, yp, zp, np, xFilm, yFilm) ;
    
    if (signal == 1) {
      G_rgb(1, 0, 0) ;
      G_polygon(xFilm, yFilm, np) ;
    }
    
  }
}
// ---------------------------------------------------------------------------------------

// draws an individual polygon from an object
int draw_single_poly (int objnum, int polynum) {
  int np, h, i ;
  double xp[100], yp[100], zp[100] ;
  double xFilm[100], yFilm[100] ;
  
  np = psize[objnum][polynum] ;

  for (i = 0; i < np; i++) {
    h = con[objnum][polynum][i] ;
    xp[i] = x[objnum][h] ;
    yp[i] = y[objnum][h] ;
    zp[i] = z[objnum][h] ;
  }

  film(xp, yp, zp, np, xFilm, yFilm) ;
  
  G_rgb(color[objnum][0],color[objnum][1],color[objnum][2]) ;
  G_fill_polygon(xFilm, yFilm, np) ;
  G_rgb(0, 0, 0) ;
  G_polygon(xFilm, yFilm, np) ;
}


// defines all the variables in listall
int init_array (int nah) {
  int np = 0 ;
  int indx = 0 ;
  int j = 0 ;
  double distance = 0 ;
  
  for (int obnum = 0; obnum < nah; obnum++) {
    for (int ponum = 0; ponum < numpolys[obnum]; ponum++) {
      np = psize[obnum][ponum] ;
      listall[indx][0] = obnum ;
      listall[indx][1] = ponum ;

      for (int k = 0; k < np; k++) {
	j = con[obnum][ponum][k] ;

	distance += sqrt(pow(x[obnum][j], 2) +
			 pow(y[obnum][j], 2) +
			 pow(z[obnum][j], 2)) ;
      }
      listall[indx][2] = distance / np ;
      indx++ ;
      distance = 0 ;
    }
  }
  printf("%d\n", indx);
  return indx ;
}


// prints the array for test purposes
void print_array(int indx) {
  int i ;
  for (i = 0 ; i < indx ; i++) {
    printf("%lf %lf %lf\n",listall[i][0], listall[i][1], listall[i][2]) ;
  }
  printf("\n") ;
}


// compares the distances within listall
static int compare (const void *a, const void *b) {
  const double * const ia = a, * const ib = b ;

  if (ia[2] < ib[2]) return 1 ;
  if (ia[2] > ib[2]) return -1 ;
}

void draw_all_object(int nah) {

  int indx = init_array(nah) ;
  print_array(indx) ;
  
  qsort(listall, indx, sizeof *listall, compare) ;
  print_array(indx) ;

  for (int i = 0; i < indx; i++) {
    draw_single_poly(listall[i][0], listall[i][1]) ;
  }
  
}


// the main function that contains the commands
int main (int argc, char **argv) {
  // initialize the screen
  G_init_graphics(S_WIDTH, S_HEIGHT) ;
  G_rgb(0, 0, 0) ;
  G_clear() ;
  
  G_rgb(1,0,0);
  G_fill_rectangle(0, 0, S_WIDTH, 10);
  
  G_rgb(1,1,1);
  
  // lots of stuff here ....
  FILE *fin ;
  int key, w ;
  char fname[100] ;
  int i, j, sum ;
  int  onum = 0 ; // onum marks the current object

  // finds angle stuff
  double t = DEGREE*M_PI/180 ;
  double c = cos(t) ;
  double s = sin(t) ;

  for (i = 0; i < argc; i++) {
    if (i+1 < argc) {
      printf("argv[%d] is %s\n", i+1, argv[i+1]) ;
    }
  }

  numobjects = argc - 1 ;
  
  for (onum = 1; onum < numobjects+1; onum++) {
    fin = fopen(argv[onum], "r") ;
    
    if (fin == NULL) {
      printf("can't read file, %s\n", argv[onum]) ;
      exit(1) ;
    }
    
    read_object(fin, onum-1) ;
  }
  
  int  sign = 1 ;
  int  action = 't' ;  
  int  q, k, numpts ;
  double V[4][4] ;
  double final[4][4] ;

  onum = 0 ;
  rvs = 1 ;
  
  while (1) {

    M3d_make_identity(V) ;
    M3d_make_identity(final) ;


    q = G_wait_key() ;

    // to quit
    if (q == 'q') {
      exit(0) ;

      // changes which face is removed
    } else if (q == '-') {
      rvs = -rvs ;

      // changes the sign
    } else if (q == 'c') {
      sign = -sign ;
      c = cos(t * sign) ;
      s = sin(t * sign) ;

      // chooses translation
    } else if (q == 't') {
      action = q ;

      // chooses rotation
    } else if (q == 'r') {
      action = q ;

      // allows the selection of objects
    } else if (('0' <= q) && (q <= '9')) {
      k = q - '0' ;  
      if (k < numobjects) { onum = k ; }

      // translates along x
    } else if ((q == 'x') && (action == 't')) {
      M3d_make_translation(V, 0.5 * sign, 0, 0) ;
      M3d_mat_mult(final, V, final) ;

      // translates along y
    } else if ((q == 'y') && (action == 't')) {
      M3d_make_translation(V, 0, 0.5 * sign, 0) ;
      M3d_mat_mult(final, V, final) ;

      // translates along z
    } else if ((q == 'z') && (action == 't')) {
      M3d_make_translation(V, 0, 0, 0.5 * sign) ;
      M3d_mat_mult(final, V, final) ;

      // rotates around x
    } else if ((q == 'x') && (action == 'r')) {
      numpts = numpoints[onum] ;
      
      M3d_make_translation(V, -x[onum][numpts], -y[onum][numpts], -z[onum][numpts]) ;
      M3d_mat_mult(final, V, final) ;

      M3d_make_x_rotation_cs(V, c, s) ;
      M3d_mat_mult(final, V, final) ;

      M3d_make_translation(V, x[onum][numpts], y[onum][numpts], z[onum][numpts]) ;
      M3d_mat_mult(final, V, final) ;

      // rotates around y
    } else if ((q == 'y') && (action == 'r')) {
      numpts = numpoints[onum] ;
      
      M3d_make_translation(V, -x[onum][numpts], -y[onum][numpts], -z[onum][numpts]) ;
      M3d_mat_mult(final, V, final) ;

      M3d_make_y_rotation_cs(V, c, s) ;
      M3d_mat_mult(final, V, final) ;

      M3d_make_translation(V, x[onum][numpts], y[onum][numpts], z[onum][numpts]) ;
      M3d_mat_mult(final, V, final) ;

      // rotates around z
    } else if ((q == 'z') && (action == 'r')) {
      numpts = numpoints[onum] ;
      
      M3d_make_translation(V, -x[onum][numpts], -y[onum][numpts], -z[onum][numpts]) ;
      M3d_mat_mult(final, V, final) ;

      M3d_make_z_rotation_cs(V, c, s) ;
      M3d_mat_mult(final, V, final) ;

      M3d_make_translation(V, x[onum][numpts], y[onum][numpts], z[onum][numpts]) ;
      M3d_mat_mult(final, V, final) ;

      // if the key is not bound
    } else {
      printf("no action\n") ;
    }



    M3d_mat_mult_points (x[onum],y[onum],z[onum],  final,
			 x[onum],y[onum],z[onum],numpoints[onum]+1) ;
      // the numpoints[onum]+1 is because we have stored the center
      // of the object at the arrays' end

    G_rgb(0,0,0) ; 
    G_clear() ;
    G_rgb(0,0,1) ;
    
    // draws all the objects
    draw_all_object(numobjects) ;
  }
}
