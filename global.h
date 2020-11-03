/** \file
 * This file contains the declarations for the global variables.
 */

#ifndef _GLOBAL_H
#define _GLOBAL_H

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>

/** \def
 * EPS defines a infinitely small real value 
 */ 
#define EPS 1.0e-14
/** \def
 * INF defines a infinitely large real value 
 */ 
#define INF 1.0e14
/** \def
 * PI defines the value of pi
 */
#define PI 3.14159265358979
/** \def
 * E define the value of exponential 'e'
 */
#define E 2.71828182845905


#define abss(a) (a<0 ? (-a) : a)
#define max(a, b) (a>b ? a : b)							  
#define min(a, b) (a>b ? b : a)

/* File: utility.c */
double mean(double *data, int size);
double stddev(double *data, int size);
int printusage();
int findoption(char *option, int argc, char *argv[]);
void sprint_cmdline(char *string, int argc, char *argv[]);
void print_cmdline(int argc, char *argv[]);
int file_linecount(FILE *file);
int file_columncount(FILE *file);

/* File: shellsort.c */
void shellsort1d(float *value, int n);
void shellsort2d(float **value, int ncols, int caxis, int nrows);
#endif
