int inOutEquation (double xCo, double yCo, double zCo, double c,
		   double goodPoint[], double testPoint[]){
  double signs[2];
  signs[0] = xCo*goodPoint[0] + yCo*goodPoint[1] + zCo*goodPoint[2] + c;
  signs[1] = xCo*testPoint[0] + yCo*testPoint[1] + zCo*testPoint[2] + c;
  return !(signs[0]*signs[1] < 0);
}
