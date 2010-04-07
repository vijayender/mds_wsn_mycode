#include "lossFunctions.h"
#include <stdlib.h>
#include <stdio.h>


float loss_function_simple(float **p, int pdim, int pts, float** d, float lim)
{
  int i,j,k;
  float psum,dist_p,dist_d,diff,loss;

  for (i = 0; i < pts; i++) {
    for (j = 0; j < i; j++) {
      for (k = 0,psum = 0; k < pdim; k++){
	diff = (p[i][k] - p[j][k]);
	psum += SQR(diff);
      }
      dist_p = sqrt(psum);
      dist_d = d[i][j];
      diff = dist_p - dist_d;
      loss += SQR(diff);
      //      printf("%d,%d,%d\n",i,j,k);
      if ((lim > 0) & (loss > lim))
	return -1;
    }
  }
  return loss;
}
