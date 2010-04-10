#include "mds.h"
#include "mds_simplex.h"


void print_matrix (float **p, int dim1, int dim2, char* str)
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

int main(int argc, char *argv[]){
  int verbose_mode = 0;
  {
    int c;
    
    while ((c = getopt(argc, argv, "hv")) != -1)
      switch (c)
	{
	case 'h':
	  fprintf(stderr, "usage: test_mds_simplex [-v] [-h]\n");
	  exit(EXIT_SUCCESS);
	case 'v':
	  verbose_mode = 1;
	  break;
	default:
	  fprintf(stderr, "test_mds_simplex error: unknown option %c\n", c);
	  exit(EXIT_FAILURE);
	}
  }

  float **p, **d, psum, diff;
  int i,j,k;
  float val[][2] = {{1,2},{3,4},{5,6}};
  gsl_rng * number_generator = gsl_rng_alloc(gsl_rng_rand);
  
  p = new_float_matrix_2d(3, 2);
  d = new_float_matrix_2d(3, 3);

  for (i = 0; i < 3; i++)
    for (j = 0; j < 2; j++){
      p[i][j] = val[i][j];
    }

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++){
      for (k = 0, psum = 0; k < 2; k++){
	diff = p[i][k] - p[j][k];
	psum += SQR(diff);// + gsl_rng_uniform(number_generator) * 1 ;// Adding some noise
      }
      d[i][j] = psum;	/* d contains squared distances */
    }

  /*
   * Adding noise to the input `p' matrix.
   * This is easier to understand how close we are to the final answer
   * rather than adding noise to `d' matrix.
   * Random input:
   */
  for (i = 0; i < 3; i++)
    for (j = 0; j < 2; j++)
      p[i][j] = gsl_rng_uniform(number_generator) * 5;
  print_matrix(p, 3, 2, "p intially");

  mds_solve(p, 3, 2, d, 16, .001, 500, verbose_mode);
  
  exit(EXIT_SUCCESS);
  return 0;
}
