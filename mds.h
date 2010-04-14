#ifndef MDS_H
#define MDS_H
#include <gsl/gsl_rng.h>
#include <gsl/gsl_matrix.h>
#include "annealing.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <unistd.h>

#define SQR(x) (x)*(x)

double loss_function_simple (gsl_matrix *p, gsl_matrix *d, float lim);
double loss_function_simple_unsquared_d (gsl_matrix *p, gsl_matrix *d, float lim);
void step_function_internal (gsl_matrix *p, float var, gsl_rng * number_generator); /* var stands for width of distribution */
void print_matrix_2d (gsl_matrix *p, char* str);
void compute_distance_matrix_lt (gsl_matrix *d, gsl_matrix *p);
double sum_distance_matrix (gsl_matrix *d);
#endif 
