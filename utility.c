#include"global.h"

/** \file
 * This file contains some uitlity functions. not a part of the algorithm
 */

/** \fn
 * mean: Function to calculate mean of an array
 *
 * Input:
 * *data: pointer to the array of double values
 * size: size of the array
 *
 * Output:
 * mean of the array
 *
 */

double mean(double *data, int size)
{
	int i;
    double sum, avg;
	sum = avg = 0;
	for(i=0; i<size; i++)
	{
//		printf("%f ", data[i]);
		sum = sum + data[i];
	}
	avg = sum/size;
//	printf("avg %f\n", avg);
	return (avg);
}

/**
 * stddev: Function to calculate standard deviation of an data array 
 *
 * Input: 
 * *data: pointer to the data array
 * size: size of the data array
 *
 * Output:
 * standard deviation of the data
 *
 */

double stddev(double *data, int size)
{
	int i;
	double sum, avg, std;
	avg = mean(data, size);
	sum = 0;
	for(i=0; i<size; i++)
	{
		sum = sum + (data[i] - avg)*(data[i] - avg);
	}
	std = sqrt(sum/size);

	return (std);
}

/**
 * printusage(): Function print the usage of the program
 *
 */
int printusage() 
{
	printf("Usage: network_representativeness -infile input data file [ASCII]\n");
	printf("       -coordsfile coordinate file name\n");
	printf("       -sitefile site data file name\n");
	printf("       -nsites No. of sites\n");
	printf("       -minmaxfile minmax file name\n");
	printf("       -outfile output file name\n");
	printf("       -nrows No. of rows in input data\n");
	printf("       -ncols No. of variables\n");
	printf("       -help program usage help.\n");
	exit(1);
}


/**
 * findoption(): 
 * This function searches argv[] for a match to the string option[].  If
 * a match is found in the nth string in argv, then n is returned.  Otherwise,
 * findoption() returns zero.
 * It return the location of the option in argv[] so that if the option has an
 * argument associated with it, it will be simple to read that argument. 
 */
int findoption(char *option, int argc, char *argv[]) 
{
   int i;

   for(i=1; i<argc; i++) {
      if( strcmp(option, argv[i]) == 0) {
         /* The option has been found on the command line. */
         return i;
      }
   }

   return 0;
}


/** sprint_cmdline(): Print out the entire command line to the argument 'string'.  The user 
 * must take care to ensure that enough memory has been allocated for
 * 'string' for the command line to fit! 
 */
void sprint_cmdline(char *string, int argc, char *argv[]) 
{
   int i;

   sprintf(string, "");
   for(i=0; i<(argc-1); i++) {
      strcat(string, argv[i]);
      strcat(string, " ");
   }
   strcat(string, argv[argc-1]);
}

/** print_cmdline(): Print out the entire commandline argument to the screen
 */
void print_cmdline(int argc, char *argv[]) 
{
   int i;

   printf("\n*****************************************\n");
   printf("Commandline argument issued to the program\n");
   printf("*****************************************\n");
   for(i=0; i<argc; i++) {
	   printf("%s ", argv[i]);
   }
   printf("\n*****************************************\n");
}

/** file_linecount():
 *  Count lines in the file 
 */

int file_linecount(FILE *file)
{
	int c;
	int countLines = 0;
	while((c=fgetc(file))!=EOF)
	{
		if (c == '\n')
		countLines++;
	}

	return countLines;
}


/** file_columncount():
 *  Count number of columns in the file. Will be checking
 *  only first line assuming the file has same number of values 
 *  on each row
 */

int file_columncount(FILE *file)
{
	int c, 
		countColumns=0;
	char buf[1024],
		 word[64];

	while(1)
	{
		c=fscanf(file, "%s", word);
		if(c!=1)
			break;
		countColumns++;
		printf("Read %s, numcolumn %d\n", word, countColumns);
	}

	return countColumns;
}
