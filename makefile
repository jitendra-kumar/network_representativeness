CC=gcc
CFLAGS= -O3

hpea: network_representativeness.o\
	  utility.o
	  $(CC) $(CFLAGS) *.o -lm -o network_representativeness 

.o: 
	  $(CC) $(CFLAGS) -c  $<

clean:
	  \rm *.o network_representativeness 
