#include "mds.h"
#include "basic_nonsimplex.h"

static	annealing_energy_fun_t	energy_function;
static	annealing_step_fun_t	step_function;
static	annealing_log_fun_t	log_function;
static	annealing_copy_fun_t	copy_function;

static int steps_count = 0;


#define print_p(x,y) print_matrix_2d(x->p, x->pts, x->pdim, y)
#define print_d(x,y) print_matrix_2d(x->d, x->pts, x->pts, y)

void mds_solve(float **p, int pts, int pdim, float**  d, float _max_step, float energy_limit, float temperature, float damping_factor, int iters, float boltzman_k, float temp_min, int verbose_mode){
  annealing_simple_workspace_t S;
  mds_data_t configuration;
  double max_step; // Set it to half the largest distance in the given set.
  mds_data_t *temp;

  max_step = _max_step;
  configuration.pts = 3;
  configuration.pdim = 2;
  configuration.p = p;
  configuration.d = d;

  //load_data(&configuration); // Load default configuration data
  
  S.number_of_iterations_at_fixed_temperature = 10;
  S.max_step_value		= &max_step;

  S.temperature			= temperature;
  S.minimum_temperature		= temp_min;
  S.restart_temperature		= DBL_MIN; /* do not restart */
  S.boltzmann_constant		= boltzman_k;
  S.damping_factor		= damping_factor;
  S.energy_limit		= energy_limit;
  S.energy_limit_set		= 1;

  S.energy_function		= energy_function;
  S.step_function		= step_function;
  S.copy_function		= copy_function;
  S.log_function		= (verbose_mode)? log_function : NULL;
  S.cooling_function		= NULL;

  S.numbers_generator		= gsl_rng_alloc(gsl_rng_rand);
  gsl_rng_set(S.numbers_generator, 15);

  S.current_configuration.data	= &configuration;
  S.best_configuration.data	= copy_mds_data(&configuration);
  S.new_configuration.data	= copy_mds_data(&configuration);

  annealing_simple_solve(&S);

  //  printf("test_sinc: final best solution: %f, global 0.0\n", configurations[1]);
  printf("test_sinc: final best solution: %f, after %d steps\n", S.best_configuration.energy, steps_count);
  printf("------------------------------------------------------------\n\n");

  temp = (mds_data_t *) S.best_configuration.data;
  print_p(temp,"p");
  print_d(temp,"d");
  gsl_rng_free(S.numbers_generator);

  //  return 0;
}

/* ------------------------------------------------------------ */

double
energy_function (void * dummy ANNEALING_UNUSED, void * configuration)
{
  mds_data_t	*M = (mds_data_t *)configuration;

  return loss_function_simple(M,-1);
}
void
step_function (void * W, void * configuration)
{
  steps_count++;
  annealing_simple_workspace_t * S = W;
  mds_data_t *M = (mds_data_t *)configuration;
  step_function_internal(M,S->temperature * (*(double *)S->max_step_value), S->numbers_generator);
}
void
log_function (void * W)
{
  annealing_simple_workspace_t * S = W;
  double	current = *((double *)S->current_configuration.data);
  double	best    = *((double *)S->best_configuration.data);

   printf("current %f (energy %f), best %f (energy %f)\n",
	 current, S->current_configuration.energy,
	 best,    S->best_configuration.energy);
}
void
copy_function (void * dummy ANNEALING_UNUSED, void * dst_configuration, void * src_configuration)
{
  mds_data_t * dst = dst_configuration;
  mds_data_t *	src = src_configuration;

  copy_only_float_matrix_2d(src->p, dst->p, dst->pts, dst->pdim);
}


/* end of file */
