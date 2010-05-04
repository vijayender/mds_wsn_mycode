#include "iterative_majorize_improved.h"
#include "mds.h"
#include "gsl/gsl_blas.h"
#include "commonFuncs.h"

mds_data_t configuration;
int steps_count=0;
int sa_initialized = 0;
void initialize_sa(gsl_matrix *p, gsl_matrix *p2,gsl_matrix *d, double max_step, int verbose_mode);
int sa_solve(double temperature, double temperature_min, double energy_limit, double damping_factor, int iters);
double lower_triangle2;
mds_data_t * copy_mds_data(mds_data_t *M);
annealing_simple_workspace_t S;

static	annealing_energy_fun_t	energy_function;
static	annealing_step_fun_t	step_function;
static	annealing_log_fun_t	log_function;
static	annealing_copy_fun_t	copy_function;

int iterative_majorize_improved_solve(float **_p, int pts, int pdim, float**  _d, int _iters, float energy_limit, int verbose_mode, float *final_loss)
{
  //Conversion from the given data
  gsl_matrix *p, *d;
  iterative_majorizer_improved_t *s;
  int iters = 0;
  double lower_triangle, limit, min_slope = 0.0004;

  s = iterative_majorizer_improved_alloc();
  p = convert_to_gsl_matrix(_p, pts, pdim);
  d = convert_to_gsl_matrix(_d, pts, pts);
  iterative_majorizer_improved_initialize(s, p, d);
  lower_triangle = sum_distance_matrix_sqr(d);
  limit = lower_triangle * energy_limit;
  min_slope *= lower_triangle;
  do{
    iters++;
    iterative_majorizer_improved_iterate(s);
    if(verbose_mode) {
      printf("%5d: loss %f, loss_temp %f, limit %f * %f \n", iters, s->loss/lower_triangle, ((s->loss_temp - s->loss)<min_slope?1.0:0.0), energy_limit, lower_triangle);
      printf("\n-----------------------------------\n");
    }
    if((s->loss_temp - s->loss) < min_slope){
      if(!sa_initialized)
	initialize_sa(s->x,s->x_temp,d,5.0, verbose_mode);
      sa_solve(1.0,
	       0.01,
	       energy_limit,
	       1.05,
	       60);
      exit(0);
      //temperature proportional to
      //min temperature for 3 iterations
      //energy limit so that difference is 0.001
    }
      
  }while(_iters > iters && s->loss > limit);
  *final_loss = s->loss / lower_triangle;
  update_to_float(s->x, _p);
  return iters;
}

iterative_majorizer_improved_t * iterative_majorizer_improved_alloc ()
{
  return (iterative_majorizer_improved_t *) malloc (sizeof(iterative_majorizer_improved_t));
}

void iterative_majorizer_improved_initialize (iterative_majorizer_improved_t *s, gsl_matrix *x, gsl_matrix *d)
{
  s->d = d;
  s->x = x;
  // Initialize the other variables
  s->x_temp = gsl_matrix_alloc(x->size1, x->size2);
  s->BZ = gsl_matrix_alloc(d->size1, d->size2);
  s->DZ = gsl_matrix_alloc(d->size1, d->size2);
  s->loss = loss_function_simple(s->x, s->d, -1);
}

void iterative_majorizer_improved_iterate (iterative_majorizer_improved_t *s)
{
  int i,j;
  gsl_matrix * x_temp;
  double d_temp,psum;
  // To Find X_new :
  // Find DZ
  compute_distance_matrix_lt(s->DZ, s->x);
  #ifdef DEBUG
  print_matrix_2d(s->x, "from x");
  print_matrix_2d(s->DZ, "DZ");
  #endif
  // Compute B(X) using d1/dz
  for (i = 0; i < s->d->size1; i++){
    for (j = 0,psum = 0; j < s->d->size2; j++){
      if (i == j || gsl_matrix_get(s->DZ, i, j) < s->epsilon)
	gsl_matrix_set(s->BZ, i, j, 0);
      else if(i < j)
	gsl_matrix_set(s->BZ, i, j, -gsl_matrix_get(s->d, j, i)/gsl_matrix_get(s->DZ, j, i)/s->d->size1); /* Symmetry */
      else
	gsl_matrix_set(s->BZ, i, j, -gsl_matrix_get(s->d, i, j)/gsl_matrix_get(s->DZ, i, j)/s->d->size1);
      psum += gsl_matrix_get(s->BZ,i,j);
    }
    gsl_matrix_set(s->BZ, i, i, -psum);
  }
  //Update the diagonal elements
  #ifdef DEBUG
  print_matrix_2d(s->BZ,"BZ");
  #endif
  // Multiply B(x)*x in DZ and store in x_temp
  gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,
		  1.0, s->BZ, s->x,
		  0.0, s->x_temp);
  #ifdef DEBUG
  print_matrix_2d(s->x_temp, "new x");
  #endif
  s->loss_temp = loss_function_simple_unsquared_d(s->x_temp, s->d, -1);
  // swap pointers x and x_temp
  x_temp = s->x_temp; s->x_temp = s->x; s->x = x_temp;
  d_temp = s->loss_temp; s->loss_temp = s->loss; s->loss = d_temp;
}

mds_data_t * copy_mds_data_with_p(mds_data_t *M,gsl_matrix *p)
{
  mds_data_t * new_mds;

  new_mds = (mds_data_t *) malloc(sizeof(mds_data_t));
  new_mds->p = p;
  new_mds->d = M->d;

  return new_mds;
}

mds_data_t * copy_mds_data(mds_data_t *M)
{
  mds_data_t * new_mds;

  new_mds = (mds_data_t *) malloc(sizeof(mds_data_t));
  new_mds->p = gsl_matrix_alloc(M->p->size1, M->p->size2);
  gsl_matrix_memcpy(new_mds->p, M->p);
  new_mds->d = M->d;

  return new_mds;
}

void initialize_sa(gsl_matrix *p, gsl_matrix *p2, gsl_matrix *d, double max_step, int verbose_mode)
{
  //double max_step; // Set it to half the largest distance in the given set.
  gsl_matrix *d2;
  d2 = gsl_matrix_alloc(d->size1, d->size2);
  int i,j;
  for (i = 0; i < d->size1; i++)
    for (j = 0; j < i; j++)
      gsl_matrix_set(d2, i, j, SQR(gsl_matrix_get(d, i, j)));
  //  max_step = _max_step;
  configuration.p = p;
  configuration.d = d2;
  lower_triangle2 = sum_distance_matrix(configuration.d);
  //load_data(&configuration); // Load default configuration data
  
  //S.number_of_iterations_at_fixed_temperature = iters;
  S.max_step_value		= &max_step;
  
  //S.temperature			= temperature;
  //S.minimum_temperature		= temp_min;
  S.restart_temperature		= DBL_MIN; /* do not restart */
  S.boltzmann_constant		= 1.0;
  //S.damping_factor		= damping_factor;
  //S.energy_limit		= energy_limit * lower_triangle;
  S.energy_limit_set		= 1;

  S.energy_function		= energy_function;
  S.step_function		= step_function;
  S.copy_function		= copy_function;
  S.log_function		= (verbose_mode)? log_function : NULL;
  S.cooling_function		= NULL;

  S.numbers_generator		= gsl_rng_alloc(gsl_rng_rand);
  gsl_rng_set(S.numbers_generator, 15);

  S.best_configuration.data	= &configuration;
  // SO THAT THERE WILL BE NO NEED OF UPDATING P
  S.current_configuration.data	= copy_mds_data_with_p(&configuration,p2);
  S.new_configuration.data	= copy_mds_data(&configuration);

  //annealing_simple_solve(&S);

  //temp = (mds_data_t *) S.best_configuration.data;
  //*f_loss = S.best_configuration.energy / lower_triangle2 ;
  //gsl_rng_free(S.numbers_generator);

}
int sa_solve(double temperature, double temperature_min, double energy_limit, double damping_factor, int iters)
{
  mds_data_t *t;
  printf("%f %f\n",temperature, temperature_min);
  steps_count=0;
  S.temperature			= temperature;
  S.minimum_temperature		= temperature_min;
  S.damping_factor		= damping_factor;
  S.energy_limit		= energy_limit * lower_triangle2;
  S.number_of_iterations_at_fixed_temperature = iters;
  annealing_simple_solve(&S);
  t = (mds_data_t *) S.best_configuration.data;

  return steps_count;
}

double
energy_function (void * dummy ANNEALING_UNUSED, void * configuration)
{
  mds_data_t	*M = (mds_data_t *)configuration;

  return loss_function_simple(M->p, M->d, -1);
}
void
step_function (void * W, void * configuration)
{
  steps_count++;
  annealing_simple_workspace_t * S = W;
  mds_data_t *M = (mds_data_t *)configuration;
  step_function_internal(M->p ,S->temperature * (*(double *)S->max_step_value), S->numbers_generator);
}
void
log_function (void * W)
{
  annealing_simple_workspace_t * S = W;
  double	current = *((double *)S->current_configuration.data);
  double	best    = *((double *)S->best_configuration.data);

   printf("current %f (energy %f) %f, best %f (energy %f), temperature %f, limit %f\n",
	  current, S->current_configuration.energy/lower_triangle2,
	  S->current_configuration.energy,
	  best,    S->best_configuration.energy, S->temperature,
	  S->energy_limit);
}
void
copy_function (void * dummy ANNEALING_UNUSED, void * dst_configuration, void * src_configuration)
{
  mds_data_t * dst = dst_configuration;
  mds_data_t *	src = src_configuration;

  gsl_matrix_memcpy(dst->p, src->p);
}
