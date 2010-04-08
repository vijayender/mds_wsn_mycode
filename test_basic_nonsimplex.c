#include "mds.h"
#include "basic_nonsimplex.h"

int main(int argc, char *argv[]){
  int verbose_mode = 0;
  {
    int c;
    
    while ((c = getopt(argc, argv, "hv")) != -1)
      switch (c)
	{
	case 'h':
	  fprintf(stderr, "usage: test_basic_nonsimplex [-v] [-h]\n");
	  exit(EXIT_SUCCESS);
	case 'v':
	  verbose_mode = 1;
	  break;
	default:
	  fprintf(stderr, "test_basic_nonsimplex error: unknown option %c\n", c);
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
  print_matrix_2d(p, 3, 2, "p intially");

  mds_solve(p, 3, 2, d, 16, .001, 1, 1.005, 10, 1, 1e-5, verbose_mode);
  
  exit(EXIT_SUCCESS);
  return 0;
}
