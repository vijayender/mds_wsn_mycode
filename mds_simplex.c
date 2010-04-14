#include "mds_simplex.h"
#include "mds.h"
#include "commonFuncs.h"

double mds_f(const gsl_vector *, void *);


int mds_simplex_solve(float **_p, int pts, int pdim, float**  _d, float _max_step, float energy_limit,  int iters, int verbose_mode, float *loss)
{
  const gsl_multimin_fminimizer_type *fminimizer;
  gsl_multimin_function mds_min;

  //Conversion from the given data
  gsl_matrix *p, *d, *result;
  gsl_vector *x, *step_size;
  gsl_multimin_fminimizer *s;
  mds_data_t mds_data;
  int i, status, iter = 0;
  double lower_triangle, limit;
  /* Load data */
  p = convert_to_gsl_matrix(_p, pts, pdim);
  d = convert_to_gsl_matrix(_d, pts, pts);
  mds_data.p = p;
  mds_data.d = d;
  /* Create fminimizer of gsl */
  fminimizer = gsl_multimin_fminimizer_nmsimplex;
  mds_min.f = &mds_f;
  mds_min.n = p->size1 * p->size2;
  mds_min.params = &mds_data;
  /* Format data for fminimizer (accepts vector) */
  x = gsl_vector_alloc_from_block(p->block, 0, mds_min.n, 1);
  step_size = gsl_vector_alloc (mds_min.n);
  /* Set step size */
  for (i = 0; i < mds_min.n; i++)
    gsl_vector_set (step_size, i, _max_step);
  /* Initialize fminimizer */
  s = gsl_multimin_fminimizer_alloc(fminimizer, mds_min.n);
  gsl_multimin_fminimizer_set(s, &mds_min, x, step_size);
  lower_triangle = sum_distance_matrix(d);
  limit = lower_triangle  * energy_limit;
  do{
    iter++;
    status = gsl_multimin_fminimizer_iterate(s);
    if(verbose_mode){
      printf("%i: \n",iter);
      //    printf("x "); gsl_vector_fprintf (stdout, s->x, "%g"); 
      printf("f(x) %f\n", gsl_multimin_fminimizer_minimum (s));
      printf("size: %f\n", gsl_multimin_fminimizer_size (s));
      printf("limit: %f * %f\n", lower_triangle, energy_limit);
      printf("\n");
    }
    status = gsl_multimin_test_size (gsl_multimin_fminimizer_minimum (s),
				       limit);
  }
  while (iters > iter && status == GSL_CONTINUE);
  result = gsl_matrix_alloc_from_block(s->x->block, 0,
				       p->size1, p->size2,
				       p->tda);
  update_to_float(result, _p);
  *loss = gsl_multimin_fminimizer_minimum (s);
  
  return iter;
}

double mds_f(const gsl_vector * x, void *params)
/* Evaluates loss function of the given data */
{
  mds_data_t *mds_data;
  double loss;
  gsl_matrix *g;
  mds_data = (mds_data_t *) params;
  
  g = gsl_matrix_alloc_from_block(x->block, 0,
				  mds_data->p->size1,
				  mds_data->p->size2,
				  mds_data->p->tda );
  loss = loss_function_simple (g, mds_data->d, -1);
  return loss;
}
