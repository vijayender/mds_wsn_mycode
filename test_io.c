#include "test_io.h"
#include <stdio.h>
#include <stdlib.h>

void skip_line(FILE *fp);
void read_float_array(FILE *fp, float ***p, size_t dim1, size_t dim2);

void read_from_file(char* fname, char elem, float ***p, int *dim1, int *dim2 )
/* 'elem' is the element that will be searched for */
{
  FILE *fp;
  int c=0;
  size_t len1, len2;
  
  fp = fopen(fname, "r");
  
  while(c != EOF){
    c = fgetc(fp);
    if (c == elem) {
      skip_line(fp);
      c = fscanf(fp, "\nl %zd %zd", &len1, &len2);
      read_float_array(fp, p, len1, len2);
    }
    skip_line(fp);
  }
  *dim1 = len1;
  *dim2 = len2;
  fclose(fp);
}

float read_float_from_file(char *fname, char elem)
{
  FILE *fp;
  int c=0;
  float res;
  
  fp = fopen(fname, "r");
  
  while(c != EOF){
    c = fgetc(fp);
    if (c == elem) {
      fscanf(fp, "%f", &res);
      return res;
    }
    skip_line(fp);
  }
  fclose(fp);
  return -1;
}

int read_int_from_file(char *fname, char elem)
{
  FILE *fp;
  int c=0;
  int res;
  
  fp = fopen(fname, "r");
  
  while(c != EOF){
    c = fgetc(fp);
    if (c == elem) {
      fscanf(fp, "%d", &res);
      return res;
    }
    skip_line(fp);
  }
  fclose(fp);
  return -1;
}

void skip_line(FILE *fp)
{
  int c;
  do{
    c = fgetc(fp);
  } while (c!=EOF && c!='\n');
}

void read_float_array(FILE *fp, float ***p, size_t dim1, size_t dim2)
/* Allocate memory and load the contents from file */
{
  int i,j;
  *p = (float **)malloc(dim1*sizeof(float*));
  for (i = 0; i < dim1; i++){
    (*p)[i] = (float *)malloc(dim2*sizeof(float));
    for (j = 0; j < dim2; j++){
      fscanf(fp,"%f", &(*p)[i][j]);
      fgetc(fp); /* Flush out the next character which might be ',' or
		    '\n' */      
    }
  }
}

void write_floatarr_to_file(char *fname, float **p, char elem, int dim1, int  dim2)
{
  // Open in append mode
  FILE *fp;
  int i,j;
  fp = fopen(fname, "a");

  fputc(elem,fp);
  fputc('\n',fp);
  fprintf(fp,"l %d %d\n",dim1, dim2);
  for (i = 0; i < dim1; i++){
    for (j = 0; j < dim2; j++){
      fprintf(fp,"%f",p[i][j]);
      if(j < dim2 - 1)
	fputc(',',fp);
      else
	fputc('\n',fp);
    }
  }

  fclose(fp);
  return;
}

void write_float_to_file(char *fname, char elem, float f)
{
  FILE *fp;
  fp = fopen(fname,"a");

  fprintf(fp,"%c %f\n",elem,f);
  fclose(fp);
}

void write_int_to_file(char *fname, char elem, int i)
{
  FILE *fp;
  fp = fopen(fname,"a");

  fprintf(fp,"%1c %d\n",elem,i);
  fclose(fp);
}


void write_char_to_file(char *fname, char elem)
{
  FILE *fp;
  fp = fopen(fname,"a");

  fprintf(fp,"%1c\n",elem);
  fclose(fp);
}
