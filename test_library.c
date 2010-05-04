#include "mds.h"
#include "basic_sa.h"
#include "iterative_majorize.h"
#include "iterative_majorize_improved.h"
#include "mds_simplex.h"
#include "test_io.h"
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>

extern char *optarg;
extern int optind, optopt;
void run_basic_sa(int verbose_mode);
void run_iterative_majorize(int verbose_mode);
void run_iterative_majorize_improved(int verbose_mode);
void run_mds_simlex(int verbose_mode);

char *ifile, *ofile, *cfile, *lib, *err_str = "usage: test_basic_sa [-v] [-h] -i inputfile -c configfile -o outputfile -l library\n";
struct rusage start,end;


void print_matrix (float **p, int dim1, int dim2, char* str)
{
  int i,j;
  printf("printing matrix %s \n - - - - - - - - - - - - - - - - - - - - \n", str);
  for (i = 0; i < dim1; i++){
    for (j = 0; j < dim2; j++){
      printf(" %f ",p[i][j]);
    }
    printf("\n");
  }
  printf("\n - - - - - - - - - - - - - - - - - - - - \n");
}

float **new_float_matrix_2d(int dim1, int dim2)
/* Allocates and returns unitialized new matrix*/
{
  float **x;
  int i;

  x = (float **) malloc (dim1 * sizeof(float *));
  for (i = 0; i < dim1; i++){
    x[i] = (float *) malloc (dim2 * sizeof(float *));
  }
  return x;
}

int main(int argc, char *argv[]){
  char *basic_sa = "basic_sa";
  char *iterative_majorize = "iterative_majorize";
  char *iterative_majorize_improved = "iterative_majorize_improved";
  char *mds_simplex = "mds_simplex";
    
  int verbose_mode = 0;
  {
    int c;    

    while ((c = getopt(argc, argv, "hvi:c:o:l:")) != -1)
      switch (c)
	{
	case 'h':
	  fprintf(stderr, "%s", err_str);
	  exit(EXIT_SUCCESS);
	case 'v':
	  verbose_mode = 1;
	  break;
	case 'i':
	  ifile = optarg;
	  break;
	case 'c':
	  cfile = optarg;
	  break;
	case 'o':
	  ofile = optarg;
	  break;
	case 'l':
	  lib = optarg;
	  break;
	default:
	  fprintf(stderr, "test_basic_sa error: unknown option %c\n", c);
	  exit(EXIT_FAILURE);
	}
  }

  if (!(ifile && cfile && ofile && lib)){
    fprintf(stderr, "%s", err_str);
    exit(EXIT_SUCCESS);
  }
  printf("input:%s config:%s output:%s\n",ifile,cfile,ofile);

  if ( strcmp(basic_sa, lib) == 0 ){
    run_basic_sa(verbose_mode);
  }else if ( strcmp(iterative_majorize, lib) == 0 ){
    run_iterative_majorize(verbose_mode);
  }else if ( strcmp(iterative_majorize_improved, lib) == 0 ){
    run_iterative_majorize_improved(verbose_mode);
  }else if ( strcmp(mds_simplex, lib) == 0 ){
    run_mds_simlex(verbose_mode);
  }
  
  exit(EXIT_SUCCESS);
  return 0;
}

void run_basic_sa(int verbose_mode)
{  
  float **p, **D, loss;
  int pts,pdim, iters;
  int m,i;
  float e,T,d,b,t;

  read_from_file(ifile,'x',&p,&pts,&pdim);
  read_from_file(ifile,'d',&D,&pts,&pts);
  m = read_int_from_file(cfile,'m');
  e = read_float_from_file(cfile,'e');
  T = read_float_from_file(cfile,'T');
  d = read_float_from_file(cfile,'d');
  i = read_int_from_file(cfile,'i');
  b = read_float_from_file(cfile,'b');
  t = read_float_from_file(cfile,'t');

  
  getrusage(RUSAGE_SELF, &start);
  iters = basic_sa_solve(p, pts, pdim, D,m,e,T,d,i,b,t,
	    verbose_mode,
	    &loss);
  getrusage(RUSAGE_SELF, &end);

  //mds_solve(p, pts, pdim, d, 16, .001, 1, 1.005, 10, 1, 1e-5, verbose_mode);
  write_char_to_file(ofile,' ');
  write_floatarr_to_file(ofile, p, 'p', pts, pdim);
  write_float_to_file(ofile, 'L', loss);
  write_int_to_file(ofile, 'I', iters);
  /* Log both system time and user time though only user time will be used */
  write_int_to_file(ofile,'u', (end.ru_utime.tv_usec - start.ru_utime.tv_usec)+1000000 * ( end.ru_utime.tv_sec - start.ru_utime.tv_sec));
  write_int_to_file(ofile,'S', ( end.ru_stime.tv_usec - start.ru_stime.tv_usec)+1000000 * ( end.ru_stime.tv_sec - start.ru_stime.tv_sec));
  /* iters it took 'i'
   * final loss 'l'
   */
  printf(" user time %ld\n", (end.ru_utime.tv_usec - start.ru_utime.tv_usec)+1000000 * ( end.ru_utime.tv_sec - start.ru_utime.tv_sec));
  printf(" system time %ld\n", ( end.ru_stime.tv_usec - start.ru_stime.tv_usec)+1000000 * ( end.ru_stime.tv_sec - start.ru_stime.tv_sec));    
  printf(" final loss after %d iters: %f\n", iters, loss);
}


void run_iterative_majorize(int verbose_mode)
{  
  float **p, **D, loss;
  int pts,pdim, iters;
  int i,_i,_j;
  float e;

  read_from_file(ifile,'x',&p,&pts,&pdim);
  read_from_file(ifile,'d',&D,&pts,&pts);
  for ( _i = 0; _i < pts; _i++)
    for (_j = 0; _j < _i; _j++)
      D[_i][_j] = sqrt(D[_i][_j]); /* 'coz iterative majorize uses unsquared distances */
  e = read_float_from_file(cfile,'e'); /* Loss */
  i = read_int_from_file(cfile,'i');   /* iters */
  
  getrusage(RUSAGE_SELF, &start);
  iters = iterative_majorize_solve(p, pts, pdim, D, i, e, verbose_mode, &loss);
  getrusage(RUSAGE_SELF, &end);

  write_char_to_file(ofile,' ');
  write_floatarr_to_file(ofile, p, 'p', pts, pdim);
  write_float_to_file(ofile, 'L', loss);
  write_int_to_file(ofile, 'I', iters);
  /* Log both system time and user time though only user time will be used */
  write_int_to_file(ofile,'u', (end.ru_utime.tv_usec - start.ru_utime.tv_usec)+1000000 * ( end.ru_utime.tv_sec - start.ru_utime.tv_sec));
  write_int_to_file(ofile,'S', ( end.ru_stime.tv_usec - start.ru_stime.tv_usec)+1000000 * ( end.ru_stime.tv_sec - start.ru_stime.tv_sec));
  /* iters it took 'i'
   * final loss 'l'
   */
  printf(" user time %ld\n", (end.ru_utime.tv_usec - start.ru_utime.tv_usec)+1000000 * ( end.ru_utime.tv_sec - start.ru_utime.tv_sec));
  printf(" system time %ld\n", ( end.ru_stime.tv_usec - start.ru_stime.tv_usec)+1000000 * ( end.ru_stime.tv_sec - start.ru_stime.tv_sec));
  printf(" final loss after %d iters: %f\n", iters, loss);
}

void run_mds_simlex(int verbose_mode)
{  
  float **p, **D, loss;
  int pts,pdim, iters;
  int m,i;
  float e;

  read_from_file(ifile,'x',&p,&pts,&pdim);
  read_from_file(ifile,'d',&D,&pts,&pts);
  m = read_int_from_file(cfile,'m');
  e = read_float_from_file(cfile,'e');
  i = read_int_from_file(cfile,'i');

  
  getrusage(RUSAGE_SELF, &start);
  iters = mds_simplex_solve(p, pts, pdim, D, m, e, i, verbose_mode, &loss);
  getrusage(RUSAGE_SELF, &end);

  //mds_solve(p, pts, pdim, d, 16, .001, 1, 1.005, 10, 1, 1e-5, verbose_mode);
  write_char_to_file(ofile,' ');
  write_floatarr_to_file(ofile, p, 'p', pts, pdim);
  write_float_to_file(ofile, 'L', loss);
  write_int_to_file(ofile, 'I', iters);
  /* Log both system time and user time though only user time will be used */
  write_int_to_file(ofile,'u', (end.ru_utime.tv_usec - start.ru_utime.tv_usec)+1000000 * ( end.ru_utime.tv_sec - start.ru_utime.tv_sec));
  write_int_to_file(ofile,'S', ( end.ru_stime.tv_usec - start.ru_stime.tv_usec)+1000000 * ( end.ru_stime.tv_sec - start.ru_stime.tv_sec));
  /* iters it took 'i'
   * final loss 'l'
   */
  printf(" user time %ld\n", (end.ru_utime.tv_usec - start.ru_utime.tv_usec)+1000000 * ( end.ru_utime.tv_sec - start.ru_utime.tv_sec));
  printf(" system time %ld\n", ( end.ru_stime.tv_usec - start.ru_stime.tv_usec)+1000000 * ( end.ru_stime.tv_sec - start.ru_stime.tv_sec));
  printf(" final loss after %d iters: %f\n", iters, loss);
}


void run_iterative_majorize_improved(int verbose_mode)
{  
  float **p, **D, loss;
  int pts,pdim, iters;
  int i,_i,_j;
  float e;

  read_from_file(ifile,'x',&p,&pts,&pdim);
  read_from_file(ifile,'d',&D,&pts,&pts);
  for ( _i = 0; _i < pts; _i++)
    for (_j = 0; _j < _i; _j++)
      D[_i][_j] = sqrt(D[_i][_j]); /* 'coz iterative majorize uses unsquared distances */
  e = read_float_from_file(cfile,'e'); /* Loss */
  i = read_int_from_file(cfile,'i');   /* iters */
  
  getrusage(RUSAGE_SELF, &start);
  iters = iterative_majorize_improved_solve(p, pts, pdim, D, i, e, verbose_mode, &loss);
  getrusage(RUSAGE_SELF, &end);

  write_char_to_file(ofile,' ');
  write_floatarr_to_file(ofile, p, 'p', pts, pdim);
  write_float_to_file(ofile, 'L', loss);
  write_int_to_file(ofile, 'I', iters);
  /* Log both system time and user time though only user time will be used */
  write_int_to_file(ofile,'u', (end.ru_utime.tv_usec - start.ru_utime.tv_usec)+1000000 * ( end.ru_utime.tv_sec - start.ru_utime.tv_sec));
  write_int_to_file(ofile,'S', ( end.ru_stime.tv_usec - start.ru_stime.tv_usec)+1000000 * ( end.ru_stime.tv_sec - start.ru_stime.tv_sec));
  /* iters it took 'i'
   * final loss 'l'
   */
  printf(" user time %ld\n", (end.ru_utime.tv_usec - start.ru_utime.tv_usec)+1000000 * ( end.ru_utime.tv_sec - start.ru_utime.tv_sec));
  printf(" system time %ld\n", ( end.ru_stime.tv_usec - start.ru_stime.tv_usec)+1000000 * ( end.ru_stime.tv_sec - start.ru_stime.tv_sec));
  printf(" final loss after %d iters: %f\n", iters, loss);
}
