#ifndef MDS_H
#define MDS_H
#include <gsl/gsl_rng.h>
#include "annealing.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SQR(x) (x)*(x)

typedef struct mds_data_t {
  float **p; //List of points
  int pts; // No. of points
  int pdim; // No. of dimensions in each ponit
  float **d; // Distance matrix
} mds_data_t;

mds_data_t *new_mds_data ();
float **new_float_matrix_2d (int dim1, int dim2);
float **copy_float_matrix_2d (float **p, int dim1, int dim2);
mds_data_t *copy_mds_data (mds_data_t *M);
float loss_function_simple (mds_data_t *M, float lim);
void step_function (mds_data_t *M, float var, gsl_rng * number_generator); /* var stands for width of distribution */
void print_matrix_2d (float **p, int dim1, int dim2, char* str);

#endif
