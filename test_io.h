#ifndef TEST_IO_H
#define TEST_IO_H

void read_from_file(char *fname, char elem, float ***p, int *dim1, int *dim2);
float read_float_from_file(char *fname, char elem);
int read_int_from_file(char *fname, char elem);
void write_floatarr_to_file(char *fname, float **p, char elem, int dim1, int dim2);
void write_float_to_file(char *fname, char elem, float f);
void write_int_to_file(char *fname, char elem, int i);
void write_char_to_file(char *fname, char elem);
#endif
