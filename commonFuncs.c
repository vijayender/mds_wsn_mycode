#include "commonFuncs.h"

gsl_matrix * convert_to_gsl_matrix (float **_p, int dim1, int dim2)
{
  gsl_matrix * p;
  int i,j;

  p = gsl_matrix_alloc(dim1, dim2);
  for (i = 0; i < dim1; i++)
    for (j = 0; j < dim2; j++)
      gsl_matrix_set(p,i,j,_p[i][j]);
  return p;
}

void update_to_float (gsl_matrix *p,float **_p)
{
  int i,j;
  
  for (i = 0; i < p->size1; i++)
    for (j = 0; j < p->size2; j++)
      _p[i][j] = gsl_matrix_get(p,i,j);
}
