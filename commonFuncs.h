#ifndef COMMON_FUNCS_H
#define COMMON_FUNCS_H
#include <gsl/gsl_matrix.h>

typedef struct mds_data_t{
  gsl_matrix *p;
  gsl_matrix *d;
} mds_data_t;


gsl_matrix * convert_to_gsl_matrix (float **_p, int dim1, int dim2);
void update_to_float (gsl_matrix *, float **);

#endif
