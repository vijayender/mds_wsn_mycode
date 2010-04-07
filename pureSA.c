#include <stdio.h>
#include <stdlib.h>
#include "lossFunctions.h"

float random_f(void);
float sum_d(float **, int);
void pureSA(float **, int, int, float**, int, float,  float (* loss_function)(float **, int , int , float**,float));

int main(int argc, char* argv[])
{
  // Testing loss function
  float x[][2] = {{1,2},{3,4},{5,6},{6,7},{7,8},{8,9},{2,5},{7,2},{3,6},{2,6}};
  float** d;
  // Computation of d
  int i,j,k;
  float psum,diff;
  float **x_p, **p_test;

  srand((unsigned)1);			//Start seed
  printf("hello world \n");

  d = (float **) malloc(10 * sizeof(float *));
  for (i = 0; i < 10; i++) {
    d[i] = (float *)malloc(10 * sizeof(float));
    for (j = 0; j < i; j++) {
      for (k = 0,psum = 0; k < 2; k++){
	diff = (x[i][k] - x[j][k]);
	psum += SQR(diff);
      }
      d[i][j] = sqrt(psum);
      printf(" %f ",d[i][j]);
    }
    printf("\n");
  }
  /* printf(" computed loss is %f ",loss_function((float **)x,2,10,(float **)d));
   * using the above call is problematic if x is of type float[][].
   * 
   * A solution could be by creating a new variable p which is **float as follows
   */
  x_p = (float **) malloc(10 * sizeof(float *));
  for (i = 0; i < 10; i++) {
    x_p[i] = x[i];
  }
  //TEST FUNCTIONS
  //Adding a random amount of error to d in [0,1]
  for (i = 0; i < 10; i++){
    for(j = 0; j < i; j++){
      d[i][j] += random_f()*0.1;
    }
  }
  printf(" computed loss is %f \n",loss_function_simple(x_p,2,10,d,-1.0f));
  printf(" hellow world \n");
  p_test = (float **)malloc(10 * sizeof(float *));
  for (i = 0; i < 10; i++){
    p_test[i] = (float *) malloc(2 * sizeof(float));
    for (k = 0; k < 2; k++){
      p_test[i][k] = random()%10;
      printf(" %f ",p_test[i][k]);
    }
    printf("\n");
  }

  pureSA(p_test, 2, 10, d, 10000, 0.01, loss_function_simple);

  //  p_test = x_p;
  for (i = 0; i < 10; i++){
    for (k = 0; k < 2; k++)
      printf(" %f ", p_test[i][k]);
    printf("\n");
  }
  return 0;
}

float random_f() //univariate random float
{
  return ((float)rand() / RAND_MAX ); //Assuming that the random numbers are uniform
}

float sum_d(float **d, int pts){
  int i,j;
  float sum=0;
  for (i = 0; i < pts; i++)
    for (j = 0; j < i; j++)
      sum += d[i][j];
  return sum;
}

void pureSA(float **p, int pdim, int pts, float** d, int maxIter, float tol, float (* loss_function)(float **, int , int , float**,float lim))
{
  int iter = 0;
  int i,j,k;
  float loss,loss_threshold,loss1;
  // float psum,dist_p,dist_d,diff,loss1;
  float t;
  float normalizer; // The denominator in the loss function.
  float **p1, **p_temp; // Temporary variable where new location is saved.
  
  t = 0.09;
  // Intialize p1
  p1 = (float **)malloc(sizeof(float *) * pts);
  for (i = 0; i < pts; i++)
    p1[i] = malloc(sizeof(float) * pdim);
  normalizer = sum_d(d, pts);
  printf("%f\n",normalizer);
  tol = tol * normalizer;
  loss = loss_function(p, pdim, pts, d, -1.0f);
  printf(" initial loss %f", loss);
  while((loss > tol) & (iter < maxIter)){
    iter++;
    // Brownian motion
    for (i = 0; i < pts; i++){
      for (k = 0; k < pdim; k++){ // t * tan (Random from -90 to 90)
	p1[i][k] = p[i][k] + t * tan((random_f() - 0.5) * 1.3659);
	//printf(" %f ",p1[i][k]);
      }
      //printf("|%f\n", tan(90));
    }
    loss_threshold = loss - t * log(random_f()) * normalizer;
    // Compute loss while comparing with loss_threshold
    loss1 = loss_function(p1, pdim, pts, d, -1);// loss_threshold);
    //printf("%f,%f\n", loss_threshold, loss1);
    if(loss1 > loss_threshold)
      continue;
    // Check if acceptable -- done in above ^ step
    // Update loss and p (by swapping p1)
    loss = loss1;
    p_temp = p ; p = p1 ; p1 = p_temp;
    // Update t
    t = t * 0.99;
    printf(" iter,%d, %f, %f\n ",iter,loss, t*normalizer);
  }
  return;
}
 
