#include "mds.h"


static	annealing_energy_fun_t	energy_function;
static	annealing_step_fun_t	step_function;
static	annealing_log_fun_t	log_function;
static	annealing_copy_fun_t	copy_function;

static void load_data (mds_data_t *config);
void mds_solve(float **p, int pts, int pdim, float**  d);

static int steps_count = 0;


#define print_p(x,y) print_matrix_2d(x->p, x->pts, x->pdim, y)
#define print_d(x,y) print_matrix_2d(x->d, x->pts, x->pts, y)

int main(int argc, char *argv[]){
  annealing_simple_workspace_t S;
  mds_data_t configuration;
  double max_step = 32.0; // Set it to half the largest distance in the given set.
  int verbose_mode = 0;
  mds_data_t *temp;
  
  
  {
    int c;

    while ((c = getopt(argc, argv, "hv")) != -1)
      switch (c)
	{
	case 'h':
	  fprintf(stderr, "usage: test_basic_nonsimplex [-v] [-h]\n");
	  exit(EXIT_SUCCESS);
	case 'v':
	  verbose_mode = 1;
	  break;
	default:
	  fprintf(stderr, "test_basic_nonsimplex error: unknown option %c\n", c);
	  exit(EXIT_FAILURE);
	}
  }

  load_data(&configuration); // Load default configuration data
  
  S.number_of_iterations_at_fixed_temperature = 10;
  S.max_step_value		= &max_step;

  S.temperature			= 10.0;
  S.minimum_temperature		= 1.0e-6;
  S.restart_temperature		= DBL_MIN; /* do not restart */
  S.boltzmann_constant		= 1.0;
  S.damping_factor		= 1.005;
  S.energy_limit = 0.001;
  S.energy_limit_set = 1;

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
  exit(EXIT_SUCCESS);

  
  return 0;
}

void mds_solve(float **p, int pts, int pdim, float**  d){
  
}

void load_data(mds_data_t *M){
  float val[][2] = {{1,2},{3,4},{5,6}};
  int i,j,k;
  float psum,diff;
  gsl_rng * number_generator = gsl_rng_alloc(gsl_rng_rand);

  M->pts = 3;
  M->pdim = 2;
  M->p = new_float_matrix_2d(M->pts, M->pdim);
  M->d = new_float_matrix_2d(M->pts, M->pts);

  for (i = 0; i < 3; i++)
    for (j = 0; j < 2; j++){
      M->p[i][j] = val[i][j];
    }

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++){
      for (k = 0, psum = 0; k < 2; k++){
	diff = M->p[i][k] - M->p[j][k];
	psum += SQR(diff);// + gsl_rng_uniform(number_generator) * 1 ;// Adding some noise
      }
      M->d[i][j] = psum;	/* d contains squared distances */
    }

  /*
   * Adding noise to the input `p' matrix.
   * This is easier to understand how close we are to the final answer
   * rather than adding noise to `d' matrix.
   * Random input:
   */
  for (i = 0; i < 3; i++)
    for (j = 0; j < 2; j++)
      M->p[i][j] = gsl_rng_uniform(number_generator) * 5;
  print_p(M,"p intially");
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
