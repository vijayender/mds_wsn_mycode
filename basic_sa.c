#include "mds.h"
#include "basic_sa.h"


#define print_p(x,y) print_matrix_2d(x, y)


static	annealing_energy_fun_t	energy_function;
static	annealing_step_fun_t	step_function;
static	annealing_log_fun_t	log_function;
static	annealing_copy_fun_t	copy_function;

static int steps_count = 0;
double lower_triangle;

typedef struct mds_data_t{
  gsl_matrix *p;
  gsl_matrix *d;
} mds_data_t;

gsl_matrix * convert_to_gsl_matrix (float **_p, int dim1, int dim2);
void update_to_float (gsl_matrix *, float **);
mds_data_t * copy_mds_data(mds_data_t *M);

int basic_sa_solve(float **p, int pts, int pdim, float**  d, float _max_step, float energy_limit, float temperature, float damping_factor, int iters, float boltzman_k, float temp_min, int verbose_mode, float *f_loss){
  annealing_simple_workspace_t S;
  mds_data_t configuration;
  double max_step; // Set it to half the largest distance in the given set.
  mds_data_t *temp;

  max_step = _max_step;
  configuration.p = convert_to_gsl_matrix(p, pts, pdim);
  configuration.d = convert_to_gsl_matrix(d, pts, pts);
  lower_triangle = sum_distance_matrix(configuration.d);
  //load_data(&configuration); // Load default configuration data
  
  S.number_of_iterations_at_fixed_temperature = iters;
  S.max_step_value		= &max_step;
  
  S.temperature			= temperature;
  S.minimum_temperature		= temp_min;
  S.restart_temperature		= DBL_MIN; /* do not restart */
  S.boltzmann_constant		= boltzman_k;
  S.damping_factor		= damping_factor;
  S.energy_limit		= energy_limit * lower_triangle;
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

  temp = (mds_data_t *) S.best_configuration.data;
  *f_loss = S.best_configuration.energy / lower_triangle ;
  gsl_rng_free(S.numbers_generator);
  update_to_float(temp->p, p);
  return steps_count;
}

/* ------------------------------------------------------------ */

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

   printf("current %f (energy %f), best %f (energy %f)\n",
	 current, S->current_configuration.energy/lower_triangle,
	 best,    S->best_configuration.energy);
}
void
copy_function (void * dummy ANNEALING_UNUSED, void * dst_configuration, void * src_configuration)
{
  mds_data_t * dst = dst_configuration;
  mds_data_t *	src = src_configuration;

  gsl_matrix_memcpy(dst->p, src->p);
}

gsl_matrix * convert_to_gsl_matrix (float **_p, int dim1, int dim2)
{
  gsl_matrix * p;
  int i,j;

  p = gsl_matrix_alloc(dim1, dim2);
  for (i = 0; i < dim1; i++)
    for (j = 0; j < dim2; j++)
      gsl_matrix_set(p,i,j,_p[i][j]);
  return p;
}

void update_to_float (gsl_matrix *p,float **_p)
{
  int i,j;
  
  for (i = 0; i < p->size1; i++)
    for (j = 0; j < p->size2; j++)
      _p[i][j] = gsl_matrix_get(p,i,j);
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

/* end of file */
