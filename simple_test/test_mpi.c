#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_HOSTNAME_LENGTH 256

int mpi_solve(int argc, char* argv[])
{
    int pid;
    int nump, rank, rc;

    rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS) {
        fprintf(stderr, "MPI_Init failed!\n");
        return -1;
    }
    printf("This is the first MPI test program.\n");
    MPI_Finalize();
    return 0;
}

int main(int argc, char* argv[])
{
    return mpi_solve( argc, argv );
}
