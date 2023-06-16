[![DOI](https://zenodo.org/badge/309788667.svg)](https://zenodo.org/badge/latestdoi/309788667)

**Cite this code as**: Kumar, J. (2023). Multivariate Quantitative Representativeness and Constituency Analysis of Ecological Observation Networks (Version 1.0) [Computer software]. https://doi.org/10.5281/zenodo.8048530

# Multivariate Quantitative Representativeness and Constituency Analysis of Ecological Observation Networks
## Author: Jitendra (Jitu) Kumar (kumarj@ornl.gov), Oak Ridge National Laboratory

Regional and global ecological research networks, representing coordinated and standardized as well as adhoc networks of observation sites, provide valuable observations necessary for ecological modeling and synthesis studies. Studies conducted across observational networks strive to scale up their results to larger areas, trying to reach conclusions that are valid throughout regional, continental, and even global scales. Network representativeness and constituency can show how well conditions at those locations represent conditions elsewhere within a larger area containing the network and can be used to help scale-up results over larger regions.

**Representativeness**: Euclidean distance between two sites plotted in multivariate environmental space can be used as an inverse measure of multivariate similarity to quantify representativeness. Close sites in environmental space have a similar combination of environmental factors, and therefore are highly representative of each other.

**Constituency**: For any site in the network, its Constituency represent all locations that are best represented by the multivariate environmental drivers at that site.

**Code Compilation**: 
```bash
make 
```
Edit the ```makefile``` as needed for your platform.
```bash
CC=gcc
CFLAGS= -O3

hpea: network_representativeness.o\
	  utility.o
	  $(CC) $(CFLAGS) *.o -lm -o network_representativeness 

.o: 
	  $(CC) $(CFLAGS) -c  $<

clean:
	  \rm *.o network_representativeness
```

**Running the representativeness analysis**:
```bash
Usage: network_representativeness -infile input data file [ASCII]
       -coordsfile coordinate file name
       -clustfile coordinate file name [OPTIONAL -- must be used with -siteclustfile]
       -sitefile site data file name
       -siteclustfile site data file name [OPTIONAL -- must be used with -clustfile]
       -nsites No. of sites
       -minmaxfile minmax file name
       -outfile output file name
       -nrows No. of rows in input data
       -ncols No. of variables
       -details [OPTIONAL -- turn on output representativeness for each site, default is to write network representativeness and constituency only.]
       -help program usage help.
```

**Publications using ```network_reprentatiness``` code**:
- Kumar, J., Coffin, A. W., Baffaut, C., Ponce-Campos, G., Witthaus, L., and Hargrove, W. W. (2023) "Quantitative Representativeness and Constituency of the Long-Term Agroecosystem Research Network, and Analysis of Complementarity with Other Existing Ecological Networks", Environmental Management (in press)
- M. M. T. A. Pallandt, J. Kumar, M. Mauritz, E. A. G. Schuur, A.-M. Virkkala, G. Celis, F. M. Hoffman, and M. Göckede. Representativeness assessment of the pan-arctic eddy covariance site network and optimized future enhancements. Biogeosciences, 19(3):559--583, 2022. https://doi.org/10.5194/bg-19-559-2022
- J. Kumar, F. M. Hoffman, W. W. Hargrove, and N. Collier. Understanding the representativeness of FLUXNET for upscaling carbon flux from eddy covariance measurements. Earth System Science Data Discussion, 2016:1--25, August 2016. https://doi.org/10.5194/essd-2016-36.
