#!/bin/bash

CALCULATE_REPRESENTATIVENESS=1

if [ $CALCULATE_REPRESENTATIVENESS -eq 1 ]
then

 NETWORK_REPRESENTATIVENESS=../network_representativeness

 indatafilename=obs.ascii.grid
 sitefilename=site.vect.out.std
 coordsfilename=coords.grid
 outfilename=grid.representativeness
 MINMAXFILE=obs.ascii.grid.minmax

nrows=`wc -l ${indatafilename} | awk '{print $1}'`
ncols=`head -n 1 ${indatafilename}| wc -w`

nsites=`wc -l ${sitefilename} | awk '{print $1}'`

${NETWORK_REPRESENTATIVENESS} -infile ${indatafilename} \
							  -nrows ${nrows} \
							  -ncols ${ncols} \
							  -sitefile ${sitefilename} \
							  -nsites ${nsites} \
							  -coordsfile ${coordsfilename} \
							  -outfile ${outfilename} \
							  -allsitesrep \
							  -details
#							  -minmaxfile ${MINMAXFILE} \

fi


