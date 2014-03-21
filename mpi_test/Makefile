LOAD	= module purge && module load openmpi/gcc/1.6.5
CC		= $(LOAD) && mpicc
RUN		= $(LOAD) && mpirun

CFLAGS	=
LFLAGS	=
DEFNUM	= -np 4
HOSTS	= -machinefile hosts

default	= test_mpi
objects	= $(default).o

all: $(default)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

$(default) : $(default).o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)
	-@rm -f *.o 2>&1 > /dev/null

run : $(default)
	$(RUN) $(DEFNUM) $(NUM) $(HOSTS) $^


.PHONY: clean all

clean :
	-@rm -f *.o $(default) 2>&1 > /dev/null
