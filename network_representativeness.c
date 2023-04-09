/**
 * Calculate Network Representativeness 
 *
 * Author: Jitendra (Jitu) Kumar 
 * Date: November 10, 2015
 *
 * Rewrite of the original python version for improved computational 
 * speed
 */

#define MAXNAMESIZE 1024
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>
#include"global.h"

void main(int argc, char *argv[])
{
	/* utility counters */
	int i, j, k, r, c, s, position;

	/* strings for filenames */
	char *indatafilename,
		 *clustfilename,
		 *sitefilename,
		 *siteclustfilename,
		 *coordsfilename,
		 *outdatafilename,
		 *minmaxfilename;
	/* File pointers */
	FILE *infile,
		 *clustfile,
		 *sitefile,
		 *siteclustfile,
		 *coordsfile,
		 *outfile,
		 *minmaxfile;
	/* Data arrays */
	double  *indata,
		   **sitedata,
		   **minmax,
		   *outdata;
	/* Coordinates */
	double xcoord, ycoord;
	/* Distanc variables */
	double dist, mindist, maxdist, percent;
	int minsite;
	/* rows/cols etc info */
	int nrows,
		ncols,
		nsites,
		clust_file_provided,
		siteclust_file_provided,
		calculate,
		*siteclustdata,
		myclust,
		write_allsitesrep,
		write_details;
	/* Timers */
	time_t startt, endt, currentt;

	
	/* Default to doing full point by point calculation and deactivate
	 * cluster based masking 
	 */
	clust_file_provided = 0 ;
	siteclust_file_provided = 0 ;
	write_allsitesrep = 0;
	write_details=0;

	fprintf(stdout, "Check 1 \n");
	time(&startt);

	/* Allocate memory for the filename strings */
	indatafilename = (char *)malloc(MAXNAMESIZE*sizeof(char));
	clustfilename = (char *)malloc(MAXNAMESIZE*sizeof(char));
	sitefilename = (char *)malloc(MAXNAMESIZE*sizeof(char));
	siteclustfilename = (char *)malloc(MAXNAMESIZE*sizeof(char));
	coordsfilename = (char *)malloc(MAXNAMESIZE*sizeof(char));
	outdatafilename = (char *)malloc(MAXNAMESIZE*sizeof(char));
	minmaxfilename = (char *)malloc(MAXNAMESIZE*sizeof(char));
	
	fprintf(stdout, "Check 2 \n");

	/* START: Parse the command line to read the options */
	if(argc < 7)
	{
		printusage();
		exit(1);
	}
	/* Parse/Read the commandline option */
	position = findoption("-help", argc, argv);
	if(position) printusage();

	/* Parse/Read the commandline option */
	position = findoption("-infile", argc, argv);
	if(position)
	{
		if(sscanf(argv[position+1], "%s", indatafilename) != 1)
		{
			fprintf(stderr, "Please provide the input file name after '-infile' flag\n");
			exit(1);
		}
	}
	else
	{
			fprintf(stderr, "Please provide the input file name after '-infile' flag\n");
			exit(1);
	}

	/* Parse/Read the commandline option */
	position = findoption("-clustfile", argc, argv);
	if(position)
	{
		if(sscanf(argv[position+1], "%s", clustfilename) != 1)
		{
			fprintf(stderr, "Please provide the clust file name after '-clustfile' flag to restrict calculations within cluster\n");
			exit(1);
		}
		else
		{
			clust_file_provided = 1;
		}
	}

	/* Parse/Read the commandline option */
	position = findoption("-sitefile", argc, argv);
	if(position)
	{
		if(sscanf(argv[position+1], "%s", sitefilename) != 1)
		{
			fprintf(stderr, "Please provide the site file name after '-sitefile' flag\n");
			exit(1);
		}
	}
	else
	{
			fprintf(stderr, "Please provide the site file name after '-sitefile' flag\n");
			exit(1);
	}

	/* Parse/Read the commandline option */
	position = findoption("-siteclustfile", argc, argv);
	if(position)
	{
		if(sscanf(argv[position+1], "%s", siteclustfilename) != 1)
		{
			fprintf(stderr, "Please provide the site clust file name after '-siteclustfile' flag to restrict calculations within cluster\n");
			exit(1);
		}
		else
		{
			siteclust_file_provided = 1;
		}
	}

	/* Parse/Read the commandline option */
	position = findoption("-coordsfile", argc, argv);
	if(position)
	{
		if(sscanf(argv[position+1], "%s", coordsfilename) != 1)
		{
			fprintf(stderr, "Please provide the coordinate file name after '-coordsfile' flag\n");
			exit(1);
		}
	}
	else
	{
			fprintf(stderr, "Please provide the coordinates file name after '-coordsfile' flag\n");
			exit(1);
	}


	/* Parse/Read the commandline option */
	position = findoption("-outfile", argc, argv);
	if(position)
	{
		if(sscanf(argv[position+1], "%s", outdatafilename) != 1)
		{
			fprintf(stderr, "Please provide the output file name after '-outfile' flag\n");
			exit(1);
		}
	}
	else
	{
			fprintf(stderr, "Please provide the output file name after '-outfile' flag\n");
			exit(1);
	}

	/* Parse/Read the commandline option */
	position = findoption("-minmaxfile", argc, argv);
	if(position)
	{
		if(sscanf(argv[position+1], "%s", minmaxfilename) != 1)
		{
			fprintf(stderr, "Please provide the minmax file name after '-minmaxfile [will be used to normalize the rep values' flag\n");
			fprintf(stderr, "First row: Min values for all variables\n");
			fprintf(stderr, "Second row: Max values for all variables\n");
			fprintf(stderr, "Max distance possible in the data [usually calculated using the min/max values]\n");
			exit(1);
		}
	}
	else
	{
			fprintf(stderr, "Please provide the minmax file name after '-minmaxfile' flag\n");
			exit(1);
	}


	/* Parse/Read the commandline option */
	position = findoption("-nrows", argc, argv);
	if(position)
	{
		if(sscanf(argv[position+1], "%d", &nrows) != 1)
		{
			fprintf(stderr, "Please provide the no. of rows/entries in input file after '-nrows' flag\n");
			exit(1);
		}
	}
	else
	{
			fprintf(stderr, "Please provide the no. of rows/entries in input file after '-nrows' flag\n");
			exit(1);
	}

	/* Parse/Read the commandline option */
	position = findoption("-ncols", argc, argv);
	if(position)
	{
		if(sscanf(argv[position+1], "%d", &ncols) != 1)
		{
			fprintf(stderr, "Please provide the no. of columns/variables in input file after '-ncols' flag\n");
			exit(1);
		}
	}
	else
	{
			fprintf(stderr, "Please provide the no. of columns/variables in input file after '-ncols' flag\n");
			exit(1);
	}

	/* Parse/Read the commandline option */
	position = findoption("-nsites", argc, argv);
	if(position)
	{
		if(sscanf(argv[position+1], "%d", &nsites) != 1)
		{
			fprintf(stderr, "Please provide the no. of sites in network after '-nsites' flag\n");
			exit(1);
		}
	}
	else
	{
			fprintf(stderr, "Please provide the no. of sites in network after '-nsites' flag\n");
			exit(1);
	}

	/* Parse/Read the commandline option */
	position = findoption("-allsitesrep", argc, argv);
	if(position)
	{
		write_allsitesrep = 1;
		fprintf(stdout, "Flag '-allsitesrep' provided. Representativeness of all sites will be written to file.\n");
	}
	else
	{
			fprintf(stdout, "Flag '-allsitesrep' not provided. Output will be limited to network representativeness and constituency.\n");
	}

	/* Parse/Read the commandline option */
	position = findoption("-details", argc, argv);
	if(position)
	{
		write_details = 1;
		fprintf(stdout, "Flag '-details' provided. Contribution of each dimension for best representing site will be written to file.\n");
	}
	else
	{
			fprintf(stdout, "Flag '-details' not provided. Contribution of each dimension for best representing site will NOT be written to file.\n");
	}

	/* Make sure clust data is provided for the full data as well as
	 * sites 
	 */
	if (clust_file_provided != siteclust_file_provided)
	{
		if(clust_file_provided == 0)
		{
			fprintf(stderr, "ERROR: Site data points cluster assignments were provide. Must also provide cluster assignement file for all data points with -clustfile flag.\n");
			exit(1);
		}
		if(siteclust_file_provided == 0)
		{
			fprintf(stderr, "ERROR: Cluster assignments for all data points were provide. Must also provide cluster assignement file for site data points with -siteclustfile flag.\n");
			exit(1);
		}
	}

	/* END: Parse the command line to read the options */
	
	fprintf(stdout,"Finished parsing command line options.\n");
	time(&currentt);
	fprintf(stdout, "Elapsed Time: %f second\n", difftime(currentt, startt));
	/*****************************************/
	/* Read the site data in memory */
	/* We assume this not too large to hold in memory */
	siteclustdata = (int *)malloc(nsites*sizeof(int));
	sitedata = (double **)malloc(nsites*sizeof(double *));
	sitedata[0] = (double *)malloc(nsites*ncols*sizeof(double));
	for(i=1; i<nsites; i++)
	{
		sitedata[i] = sitedata[i-1] + ncols;
	}

	/*****************************************/
	/* Open site file and read the data */
	sitefile = fopen(sitefilename, "r");
	if (siteclust_file_provided == 1)
	{
		siteclustfile = fopen(siteclustfilename, "r");
	}
	for(i=0; i<nsites; i++)
	{
		for(j=0; j<ncols; j++)
		{
			fscanf(sitefile, "%lf", &sitedata[i][j]);
		}
		if (siteclust_file_provided == 1)
		{
			/* Read cluster membership of this site */
			fscanf(siteclustfile, "%d", &siteclustdata[i]);
		}
	}
	fclose(sitefile);
	if (siteclust_file_provided == 1)
	{
		fclose(siteclustfile);
	}
	

	fprintf(stdout,"Finished reading site data.\n");
	time(&currentt);
	fprintf(stdout, "Elapsed Time: %f second\n", difftime(currentt, startt));

	/*****************************************/
	/* Read the min max values from the file */
	/* Allocate array to hold the min/max data */
	minmax = (double **)malloc(2*sizeof(double *));
	minmax[0] = (double *)malloc(2*ncols*sizeof(double));
	for(i=1; i<2; i++)
	{ 
		minmax[i] = minmax[i-1] + ncols;
	}
	/* Open and read the minmax filename */
	minmaxfile = fopen(minmaxfilename, "r");
	for(i=0; i<2; i++)
	{
		for(j=0; j<ncols; j++)
		{
			fscanf(minmaxfile, "%lf", &minmax[i][j]);
		}
	}
	/* Max dist value to be read from the last line in the file */
	fscanf(minmaxfile, "%lf", &maxdist);
	fclose(minmaxfile);


	/*****************************************/
	/* Allocate memory for reading the input data, saving outputs and
	 * coordinates 
	 * We will read one line at a time from file to reduce memory
	 * footprint */
	indata = (double *)malloc(ncols*sizeof(double));	
	outdata = (double *)malloc((nsites+2)*sizeof(double));	
	
	infile = fopen(indatafilename, "r");
	coordsfile = fopen(coordsfilename, "r");
	if (clust_file_provided == 1)
	{
		clustfile = fopen(clustfilename, "r");
	}
	outfile = fopen(outdatafilename, "w");

	for(r=0; r<nrows; r++)
	{
		if(r%10000 == 0)
		{
			percent = (double)(r+1)*100.0/(double)nrows;
			fprintf(stdout, "Processing %d of %d [%f%]\n", r, nrows,percent);
			time(&currentt);
			fprintf(stdout, "Elapsed Time: %f second\n", difftime(currentt, startt));
		}

		for(c=0; c<ncols; c++)
		{
			fscanf(infile, "%lf", &indata[c]);
		}
		//for(c=0; c<ncols; c++)
		//{
		//	fprintf(stdout, "%lf ", indata[c]);
		//}
		//fprintf(stdout,"\n");
		fscanf(coordsfile, "%lf %lf", &xcoord, &ycoord);
		if (clust_file_provided == 1)
		{
			fscanf(clustfile, "%d", &myclust);
		}

		/*****************************************/
		/* Calculate the Euclidean distance of this point from each site
		 * */
		mindist = 1.0E30;
		minsite = 0;
		for(s=0; s<nsites; s++)
		{
			dist = -9999.9;
			calculate=1;
			if( (clust_file_provided == 1) && (siteclust_file_provided == 1))
			{
				if(siteclustdata[s] == myclust)
				{
					calculate=1;
				}
				else
				{
					calculate=0;
				}
			}
			if((clust_file_provided != 1) || (siteclust_file_provided != 1))	
			{
				calculate=1;
				//fprintf(stderr, "SKIPPING: %d vs %d\n", siteclustdata[s], myclust);
			}
			if (calculate == 1)
			{
				dist = 0.0;
				//printf("start Site %d: dist = %f\n", s, dist);
				for(c=0; c<ncols; c++)
				{
					dist = dist + (sitedata[s][c]-indata[c])*(sitedata[s][c]-indata[c]);
				}
				//dist = dist / 3642.821416;
				dist = dist / maxdist;
				outdata[s] = dist;
				if(s==0)
				{
					mindist = dist;
					minsite = s+1;
				}
				else
				{
					if(dist < mindist)
					{
						mindist = dist;
						minsite = s+1;
					}
				}
			}
			//printf("end Site %d: dist = %f\n", s, outdata[s]);
		}
		/*****************************************/
		/* save the network min dist */
		outdata[nsites] = mindist;
		/* Save the index of closest site */
		outdata[nsites+1] = (double)minsite;

		/*****************************************/
		/* Write it out to the output data file */
		fprintf(outfile, "%lf %lf ", xcoord, ycoord);
		/* Turn ON next line to save representativness values for all
		 * sites */
		/* Save representativeness of all sites and  
		 * save the network minimum representativeness and constituency*/
		if (write_allsitesrep == 1)
		{
			for(s=0; s<(nsites+2); s++)
			{
				if(s==(nsites+1))
				{
					fprintf(outfile, "%d", (int) outdata[s]);
				}
				else
				{
					fprintf(outfile, "%.8lf ", outdata[s]);
				}
			}
			/* Write contributions along each dimension for best rep * site */
			if (write_details == 1)
			{
				for(c=0; c<ncols; c++)
				{
					fprintf(outfile, "%.8lf ", (sitedata[minsite-1][c]-indata[c])*(sitedata[minsite-1][c]-indata[c])/maxdist);
				}
			}
			fprintf(outfile, "\n");
		}
		else
		{
		 	/* save the network minimum representativeness and constituency*/
			for(s=nsites; s<(nsites+2); s++)
			{
				if(s==(nsites+1))
				{
					fprintf(outfile, "%d", (int) outdata[s]);
				}
				else
				{
					fprintf(outfile, "%.8lf ", outdata[s]);
				}
			}
			/* Write contributions along each dimension for best rep * site */
			if (write_details == 1)
			{
				for(c=0; c<ncols; c++)
				{
					fprintf(outfile, " %.8lf", (sitedata[minsite-1][c]-indata[c])*(sitedata[minsite-1][c]-indata[c])/maxdist);
				}
			}
			fprintf(outfile, "\n");
		}
	}
	fclose(coordsfile);
	fclose(infile);
	if (clust_file_provided == 1)
	{
		fclose(clustfile);
	}
	fclose(outfile);

	time(&endt);
	fprintf(stdout, "Total Elapsed Time: %f second\n", difftime(endt, startt));
}
