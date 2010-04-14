#ifndef BASIC_SA_H
#define BASIC_SA_H

int basic_sa_solve(float **p, int pts, int pdim, float**  d, float _max_step, float energy_limit, float temperature, float damping_factor, int iters, float boltzman_k, float temp_min, int verbose_mode,
	      float *f_loss);


#endif
