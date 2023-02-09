int intersect(double plane[],
	      double linePoint[], double lineVector[],
	      double intersection[]){
  printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");


  double var, constantPiece, t;
  var = 0;
  constantPiece = plane[3];
  for(int i = 0; i < 3; i++){
    constantPiece += plane[i] * linePoint[i];
    var += plane[i] * lineVector[i];
  }
  t = constantPiece / (-1*var);
  printf("intersection:");
  for(int i = 0; i < 3; i++){
    intersection[i] = linePoint[i] + t*lineVector[i];
    printf("%lf", intersection[i]);
  }
  printf("\n");
  return 1;
}

