/* The following file has codes to test the functions in mds.c
 */
#include "mds.h" // It includes stdlib, stdio

#define print_p(x,y) print_matrix_2d(x, y)

int main (int argc, char *argv[]){
  gsl_matrix *p, *p1, *d;
  float val[][2] = {{1,2},{3,4},{5,6}};
  float val2[][2] = {{1.1,2},{3,4.1},{5.1,6}};
  // TESTING creation and copying

  p = gsl_matrix_alloc(3, 2);
  d = gsl_matrix_alloc(3, 3);

  gsl_vector *vec = gsl_vector_alloc_from_block(p->block, 0, p->size1 * p->size2, 1);

  print_matrix_2d(d, "d");

  gsl_matrix_set(p,0,0,3);
  gsl_matrix_set(p,1,1,-3);

  print_matrix_2d(p, "p" );

  /* M1 = copy_mds_data(M); */
  /* M1->p[0][1] = 120; */
  p1 = gsl_matrix_alloc(3, 2);
  gsl_matrix_memcpy(p1, p);
  gsl_matrix_set(p1, 0, 1, 0.1);
  print_matrix_2d(p, "p");
  print_matrix_2d(p1, "p1");

  // TESTING loss function simple  <---- will be used in energy_function
  int i,j,k;
  float psum,diff;
  for (i = 0; i < 3; i++)
    for (j = 0; j < 2; j++){
      *gsl_matrix_ptr(p,i,j) = val[i][j];
      *gsl_matrix_ptr(p1,i,j) = val2[i][j];
    }

  print_matrix_2d(p, "p");
  print_matrix_2d(p1, "p1");
  
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++){
      for (k = 0, psum = 0; k < 2; k++){
  	diff = gsl_matrix_get(p,i,k) - gsl_matrix_get(p,j,k);
  	psum += SQR(diff);
      }
      gsl_matrix_set(d,i,j,psum);	/* d contains squared distances */
    }
  print_p(d, "d1");
  print_p(p1, "p1");
  /* Observe that both matrices have same d */
  printf("computed loss %f ... expected value of loss 2.49e-5 \n", loss_function_simple(p1,d,-1));

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++){
      for (k = 0, psum = 0; k < 2; k++){
  	diff = gsl_matrix_get(p1,i,k) - gsl_matrix_get(p1,j,k);
  	psum += SQR(diff);
      }
      gsl_matrix_set(d,i,j,psum);
    }

  print_p(d,"d1");
  // TESTING step_function

  //Printing the block
  double *block = p->block->data;

  for (i = 0; i < p->block->size; i++)
    printf(" %f -- %f ",block[i], gsl_vector_get(vec, i));
  printf("A proof of row ordered data and accessing same matrix as vector\n\n");
  
  /* create a random number gerneator */
  gsl_rng *numbers_generator = gsl_rng_alloc(gsl_rng_rand);
  //gsl_rng_set(numbers_generator, 15);
  step_function_internal(p, 0.1, numbers_generator);
  print_p(p, "p after step_function");
  printf("computed loss %f after step_function\n",loss_function_simple(p, d, -1));
  gsl_rng_free(numbers_generator);
  return 0;
}
