#include "mds.h"



float **new_float_matrix_2d(int dim1, int dim2)
/* Allocates and returns unitialized new matrix*/
{
  float **x;
  int i;

  x = (float **) malloc (dim1 * sizeof(float *));
  for (i = 0; i < dim1; i++){
    x[i] = (float *) malloc (dim2 * sizeof(float *));
  }
  return x;
}

float **copy_float_matrix_2d(float **p, int dim1, int dim2)
/* Initialize and return new 2d matrix copied from p*/
{
  float **x;
  int i,j;
  
  x = new_float_matrix_2d(dim1, dim2);
  for (i = 0; i < dim1; i++)
    for (j = 0; j < dim2; j++)
      x[i][j] = p[i][j];
  return x;
}

mds_data_t *new_mds_data()
{
  return (mds_data_t *) malloc (sizeof(mds_data_t *));
}

mds_data_t *copy_mds_data(mds_data_t *M)	  
/* Does NOT copy the d matrix */
{
  mds_data_t *mds_data;
  mds_data = new_mds_data();
  mds_data->pts = M->pts;
  mds_data->pdim = M->pdim;
  mds_data->d = M->d;   // Does NOT copy the d matrix
  mds_data->p = copy_float_matrix_2d(M->p, M->pts, M->pdim);

  return mds_data;
}


float loss_function_simple(mds_data_t *M, float lim)
/* M->d must be squared distances */
{
  int i,j,k;
  float psum,dist_p,dist_d,diff,loss;

  for (i = 0; i < M->pts; i++) {
    for (j = 0; j < i; j++) {
      for (k = 0,psum = 0; k < M->pdim; k++){
	diff = (M->p[i][k] - M->p[j][k]);
	psum += SQR(diff);
      }
      // dist_p = sqrt(psum);
      dist_p = psum;
      dist_d = M->d[i][j];
      diff = sqrt(dist_p) - sqrt(dist_d);
      // printf(" | %f %f %f",dist_p, dist_d, diff);
      loss += SQR(diff);
      // printf("%d,%d,%d\n",i,j,k);
      if ((lim > 0) & (loss > lim))
	return -1;
    }
  }
  return loss;
}

void step_function (mds_data_t *M, float var, gsl_rng* number_generator)
/* var stands for width of distribution
 * var must be supplied so as to reflect the temperature.
 */
{
  int i,j;
  for (i = 0; i < M->pts; i++)
    for (j = 0; j < M->pdim; j++)
      M->p[i][j] += var * 2 * (gsl_rng_uniform(number_generator) - 0.5);
  gsl_rng_uniform(number_generator);
}

void print_matrix_2d (float **p, int dim1, int dim2, char* str)
{
  int i,j;
  printf("printing matrix %s \n - - - - - - - - - - - - - - - - - - - - \n", str);
  for (i = 0; i < dim1; i++){
    for (j = 0; j < dim2; j++){
      printf(" %f ",p[i][j]);
    }
    printf("\n");
  }
  printf("\n - - - - - - - - - - - - - - - - - - - - \n");
}
