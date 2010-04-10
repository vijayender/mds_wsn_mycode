#include "iterative_majorize.h"
#include "mds.h"
#include "commonFuncs.h"

int mds_solve(float **_p, int pts, int pdim, float**  _d, int _iters, float energy_limit, int verbose_mode)
{
  //Conversion from the given data
  gsl_matrix *p, *d;
  iterative_majorizer_t *s;
  int iters = 0;

  s = iterative_majorizer_alloc();
  p = convert_to_gsl_matrix(_p, pts, pdim);
  d = convert_to_gsl_matrix(_d, pts, pts);
  print_matrix_2d(p, "p initial");
  print_matrix_2d(d, "d initial");
  iterative_majorizer_initialize(s, p, d);

  do{
    iters++;
    iterative_majorizer_iterate(s);
    printf("%5d: loss %f, loss_temp %f \n", iters, s->loss, s->loss_temp);
    printf("\n-----------------------------------\n");
  }while(_iters > iters && s->loss > energy_limit);

  update_to_float(s->x, _p);
  return iters;
}
