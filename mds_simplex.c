#include "mds_simplex.h"
#include "mds.h"
#include "commonFuncs.h"

double mds_f(const gsl_vector *, void *);


int mds_solve(float **_p, int pts, int pdim, float**  _d, float _max_step, float energy_limit,  int iters, int verbose_mode)
{
  const gsl_multimin_fminimizer_type *fminimizer;
  gsl_multimin_function mds_min;

  //Conversion from the given data
  gsl_matrix *p, *d, *result;
  gsl_vector *x, *step_size;
  gsl_multimin_fminimizer *s;
  mds_data_t mds_data;
  int i, status, iter = 0;

  
  p = convert_to_gsl_matrix(_p, pts, pdim);
  d = convert_to_gsl_matrix(_d, pts, pts);
  mds_data.p = p;
  mds_data.d = d;
  
  fminimizer = gsl_multimin_fminimizer_nmsimplex;
  mds_min.f = &mds_f;
  mds_min.n = p->size1 * p->size2;
  mds_min.params = &mds_data;

  x = gsl_vector_alloc_from_block(p->block, 0, mds_min.n, 1);
  step_size = gsl_vector_alloc (mds_min.n);
  
  for (i = 0; i < mds_min.n; i++)
    gsl_vector_set (step_size, i, _max_step);
  
  s = gsl_multimin_fminimizer_alloc(fminimizer, mds_min.n);
  gsl_multimin_fminimizer_set(s, &mds_min, x, step_size);

  do{
    iter++;
    status = gsl_multimin_fminimizer_iterate(s);
    if(verbose_mode){
      printf("%i: \n",iter);
      //    printf("x "); gsl_vector_fprintf (stdout, s->x, "%g"); 
      printf("f(x) %g\n", gsl_multimin_fminimizer_minimum (s));
      printf("size: %g\n", gsl_multimin_fminimizer_size (s));
      printf("\n");
    }
    status = gsl_multimin_test_size (gsl_multimin_fminimizer_minimum (s),
				       energy_limit);
  }
  while (iters > iter && status == GSL_CONTINUE);
  result = gsl_matrix_alloc_from_block(s->x->block, 0,
				       p->size1, p->size2,
				       p->tda);
  update_to_float(result, _p);
  print_matrix_2d(result,"result");
  printf("final loss after %d iters: %g, size of simplex: %g \n", iter, loss_function_simple(result, d, -1), gsl_multimin_fminimizer_size(s));
  return 1;
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
  //print_matrix_2d(g, "g");
  //printf(" %g\n ",loss);
  return loss;
}
