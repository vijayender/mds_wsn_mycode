/* The following file has codes to test the functions in mds.c
 */
#include "mds.h" // It includes stdlib, stdio

#define print_p(x,y) print_matrix_2d(x->p, x->pts, x->pdim, y)
#define print_d(x,y) print_matrix_2d(x->d, x->pts, x->pts, y)

int main (int argc, char *argv[]){
  mds_data_t *M, *M1;
  float val[][2] = {{1,2},{3,4},{5,6}};
  float val2[][2] = {{1.1,2},{3,4.1},{5.1,6}};
  // TESTING creation and copying
  M = new_mds_data();
  M->pts = 3;
  M->pdim = 2;
  M->p = new_float_matrix_2d(M->pts, M->pdim);
  M->d = new_float_matrix_2d(M->pts, M->pts);

  print_matrix_2d(M->d, M->pts, M->pts, "d");

  
  M->p[0][0] = 1;
  M->p[1][1] = -3;

  print_matrix_2d(M->p, M->pts, M->pdim, "p" );

  M1 = copy_mds_data(M);
  M1->p[0][1] = 120;

  print_matrix_2d(M->p, M->pts, M->pdim, "p" );
  print_matrix_2d(M1->p, M1->pts, M1->pdim, "p1" );

  // TESTING loss function simple  <---- will be used in energy_function
  int i,j,k;
  float psum,diff;
  for (i = 0; i < 3; i++)
    for (j = 0; j < 2; j++){
      M->p[i][j] = val[i][j];
      M1->p[i][j] = val2[i][j];
    }

  print_matrix_2d(M->p, M->pts, M->pdim, "p");
  print_matrix_2d(M1->p, M1->pts, M1->pdim, "p1");
  
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++){
      for (k = 0, psum = 0; k < 2; k++){
	diff = M->p[i][k] - M->p[j][k];
	psum += SQR(diff);
      }
      M->d[i][j] = psum;	/* d contains squared distances */
    }
  print_d(M1, "d1");
  print_p(M1, "p1");
  /* Observe that both matrices have same d */
  printf("computed loss %f ... expected value of loss 2.49e-5 \n", loss_function_simple(M1,-1));

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++){
      for (k = 0, psum = 0; k < 2; k++){
	diff = M1->p[i][k] - M1->p[j][k];
	psum += SQR(diff);
      }
      M1->d[i][j] = psum;	/* d contains squared distances */
    }

  print_d(M1,"d1");  
  // TESTING step_function
  /* create a random number gerneator */
  gsl_rng *numbers_generator = gsl_rng_alloc(gsl_rng_rand);
  //gsl_rng_set(numbers_generator, 15);
  step_function(M, 0.1, numbers_generator);
  print_p(M,"p after step_function");
  printf("computed loss %f after step_function\n",loss_function_simple(M,-1));
  gsl_rng_free(numbers_generator);
  return 0;
}
