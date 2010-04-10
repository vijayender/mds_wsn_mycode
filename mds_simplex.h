#ifndef MDS_SIMPLEX_H
#define MDS_SIMPLEX_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_multimin.h>

int mds_solve(float **p, int pts, int pdim, float**  d, float _max_step, float energy_limit, int iters,  int verbose_mode);


#endif
