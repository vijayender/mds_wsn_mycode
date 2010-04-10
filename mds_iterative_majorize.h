#ifndef MDS_ITERATIVE_H
#define MDS_ITERATIVE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_multimin.h>

int mds_solve(float **p, int pts, int pdim, float**  d, float energy_limit, int iters,  int verbose_mode);


#endif
