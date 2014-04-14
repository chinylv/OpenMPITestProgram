LOAD	= module purge && module load openmpi/gcc/1.6.5
CC		= $(LOAD) && mpicc

CFLAGS	=
LFLAGS	=

DEFAULT	= test_mpi
OBJECTS	= $(DEFAULT).o
LSF		= $(DEFAULT).lsf

all: $(DEFAULT)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

$(DEFAULT) : $(DEFAULT).o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)
	-@rm -f *.o 2>&1 > /dev/null

run : $(DEFAULT)
	bsub < $(LSF) && bjobs 


.PHONY: clean clear all

clean :
	-@rm -f *.o $(DEFAULT) 2>&1 > /dev/null

clear :
	-@rm -f *.o $(DEFAULT) [0-9]*.err [0-9]*.out 2>&1 > /dev/null
