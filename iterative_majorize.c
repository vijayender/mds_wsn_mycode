#include "iterative_majorize.h"
#include "mds.h"
#include "gsl/gsl_blas.h"


iterative_majorizer_t * iterative_majorizer_alloc ()
{
  return (iterative_majorizer_t *) malloc (sizeof(iterative_majorizer_t));
}

void iterative_majorizer_initialize (iterative_majorizer_t *s, gsl_matrix *x, gsl_matrix *d)
{
  s->d = d;
  s->x = x;
  // Initialize the other variables
  s->x_temp = gsl_matrix_alloc(x->size1, x->size2);
  s->BZ = gsl_matrix_alloc(d->size1, d->size2);
  s->DZ = gsl_matrix_alloc(d->size1, d->size2);
  s->loss = loss_function_simple(s->x, s->d, -1);
}
void iterative_majorizer_iterate (iterative_majorizer_t *s)
{
  int i,j;
  gsl_matrix * x_temp;
  double d_temp,psum;
  // To Find X_new :
  // Find DZ
  compute_distance_matrix_lt(s->DZ, s->x);
  print_matrix_2d(s->x, "from x");
  print_matrix_2d(s->DZ, "DZ");
  // Compute B(X) using d1/dz
  for (i = 0; i < s->d->size1; i++){
    for (j = 0,psum = 0; j < s->d->size2; j++){
      if (i == j || gsl_matrix_get(s->DZ, i, j) < s->epsilon)
	gsl_matrix_set(s->BZ, i, j, 0);
      else if(i < j)
	gsl_matrix_set(s->BZ, i, j, -gsl_matrix_get(s->d, j, i)/gsl_matrix_get(s->DZ, j, i)/s->d->size1); /* Symmetry */
      else
	gsl_matrix_set(s->BZ, i, j, -gsl_matrix_get(s->d, i, j)/gsl_matrix_get(s->DZ, i, j)/s->d->size1);
      psum += gsl_matrix_get(s->BZ,i,j);
    }
    gsl_matrix_set(s->BZ, i, i, -psum);
  }
  //Update the diagonal elements
    
  print_matrix_2d(s->BZ,"BZ");
  // Multiply B(x)*x in DZ and store in x_temp
  gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,
		  1.0, s->BZ, s->x,
		  0.0, s->x_temp);
  print_matrix_2d(s->x_temp, "new x");
  s->loss_temp = loss_function_simple_unsquared_d(s->x_temp, s->d, -1);
  // swap pointers x and x_temp
  x_temp = s->x_temp; s->x_temp = s->x; s->x = x_temp;
  d_temp = s->loss_temp; s->loss_temp = s->loss; s->loss = d_temp;
}
