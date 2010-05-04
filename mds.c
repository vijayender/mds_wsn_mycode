#include "mds.h"

double loss_function_simple(gsl_matrix *p, gsl_matrix *d, float lim)
/* M->d must be squared distances */
{
  int i,j,k;
  double psum,dist_p,dist_d,diff,loss=0;

  for (i = 0; i < p->size1; i++) {
    for (j = 0; j < i; j++) {
      for (k = 0,psum = 0; k < p->size2; k++){
	diff = (gsl_matrix_get(p,i,k) - gsl_matrix_get(p,j,k));
	psum += SQR(diff);
      }
      // dist_p = sqrt(psum);
      dist_p = psum;
      dist_d = gsl_matrix_get(d,i,j);
      diff = sqrt(dist_p) - sqrt(dist_d);
      //printf(" | %f %f %f",dist_p, dist_d, diff);
      loss += SQR(diff);
      //printf("%d,%d,%d\n",i,j,k);
      if ((lim > 0) & (loss > lim))
	return -1;
    }
  }
  return loss;
}

double loss_function_simple_unsquared_d(gsl_matrix *p, gsl_matrix *d, float lim)
/* M->d must be squared distances */
{
  int i,j,k;
  double psum,dist_p,dist_d,diff,loss=0;

  for (i = 0; i < p->size1; i++) {
    for (j = 0; j < i; j++) {
      for (k = 0,psum = 0; k < p->size2; k++){
	diff = (gsl_matrix_get(p,i,k) - gsl_matrix_get(p,j,k));
	psum += SQR(diff);
      }
      // dist_p = sqrt(psum);
      dist_p = psum;
      dist_d = gsl_matrix_get(d,i,j);
      diff = sqrt(dist_p) - dist_d;
      // printf(" | %f %f %f",dist_p, dist_d, diff);
      loss += SQR(diff);
      // printf("%d,%d,%d\n",i,j,k);
      if ((lim > 0) & (loss > lim))
	return -1;
    }
  }
  return loss;
}

void step_function_internal (gsl_matrix *p, float var, gsl_rng* number_generator)
/* var stands for width of distribution
 * var must be supplied so as to reflect the temperature.
 */
{
  int i,j;
  for (i = 0; i < p->size1; i++)
    for (j = 0; j < p->size2; j++)
      *gsl_matrix_ptr(p,i,j) += var * 2 * (gsl_rng_uniform(number_generator) - 0.5);
  gsl_rng_uniform(number_generator);
}

void print_matrix_2d (gsl_matrix *p, char* str)
{
  int i,j;
  printf("printing matrix %s \n - - - - - - - - - - - - - - - - - - - - \n", str);
  for (i = 0; i < p->size1; i++){
    for (j = 0; j < p->size2; j++){
      printf(" %f ",gsl_matrix_get(p,i,j));
    }
    printf("\n");
  }
  printf("\n - - - - - - - - - - - - - - - - - - - - \n");
}

void compute_distance_matrix_lt (gsl_matrix *d, gsl_matrix *p)
/* Fills only lower triangular matrix */
{
  int i,j,k;
  double diff,psum;
  
  for (i = 0; i < p->size1; i++)
    for (j = 0; j < i; j++){
      for (k = 0,psum = 0; k < p->size2; k++){
	diff = gsl_matrix_get(p, i ,k) - gsl_matrix_get(p, j, k);
	psum += SQR(diff);
      }
      gsl_matrix_set(d, i, j, sqrt(psum));
    }
}

double sum_distance_matrix (gsl_matrix *d)
/* Provides sum of the lower triangle elements of distance matrix provided */
{
  int i,j;
  double sum = 0;
  for (i = 0; i < d->size1; i++)
    for (j = 0; j < i; j++)
      sum += gsl_matrix_get(d, i, j);
  return sum;
}

double sum_distance_matrix_sqr (gsl_matrix *d)
/* Provides sum of the lower triangle elements of distance matrix provided */
{
  int i,j;
  double sum = 0;
  for (i = 0; i < d->size1; i++)
    for (j = 0; j < i; j++)
      sum += SQR(gsl_matrix_get(d, i, j));
  return sum;
}
