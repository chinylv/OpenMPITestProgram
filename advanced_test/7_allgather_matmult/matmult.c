#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

void printMat(int R, int C, double *A) {
  int i,j;
  for (i = 0; i < R; i++) {
    for (j = 0; j < C; j++) {
      printf("%8.1f ",A[i*C+j]);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  int N = 32;
  int i,j,k;
  int my_rank;
  int nprocs;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  int local_nrows = N / nprocs;
  if (N % nprocs != 0) {
    printf("Matrix size is not divisible by the number of processes\n");
    MPI_Abort(MPI_COMM_WORLD,10);
  }

  // local_vsize is prepared for vector v
  int local_vsize = local_nrows;

  double *localA, *localB, *localC;
  double *local_v, *v;
  double *globalC;

  localA = (double *) malloc(local_nrows * N * sizeof(double));
  localB = (double *) malloc(local_nrows * N * sizeof(double));
  localC = (double *) malloc(local_nrows * N * sizeof(double));
  
  globalC = (double *) malloc(N * N * sizeof(double));

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      globalC[i*N+j] = 0.0;
   }
  }

  local_v = (double *) malloc(local_vsize * sizeof(double));
  // v is the j-th column of globalB
  v       = (double *) malloc(N * sizeof(double));

  for (i = 0; i < local_nrows; i++) {
    for (j = 0; j < N; j++) {
      localA[i*N+j] = (i + my_rank * local_nrows) + j;
      localB[i*N+j] = (i + my_rank * local_nrows) - j;
    }
  }


  for (j = 0; j < N; j++) {
    for (i = 0; i < local_vsize; i++) {
      local_v[i] = localB[i*N+j]; // the j-th column
    }

    // make v the j-th column of globalB
    // when use MPI_Alltogether, we must make sure that each process using/fulfilling/operating the same array
    // so that they can combines to be one unit
    MPI_Allgather(local_v, local_vsize, MPI_DOUBLE, 
                  v,       local_vsize, MPI_DOUBLE, 
                  MPI_COMM_WORLD);
    // caculate the j-th column of localC
    for (i = 0; i < local_nrows; i++) {
      localC[i*N+j] = 0.0;
      for (k = 0; k < N; k++) {
        localC[i*N+j] += localA[i*N+k] * v[k]; // v[k] is the globalB[j][k]
      }
    }
  } // end outer loop

  if (my_rank == 0) {
    printf("localA=\n"); printMat(local_nrows, N, localA);
    printf("localB=\n"); printMat(local_nrows, N, localB);
  }
  
  // you will understand the program when you see the output here
  // for every process, local_nrows lines of data in globalC is calculated
  if (my_rank==0) {
    printf("localC=\n"); printMat(local_nrows,N,localC);  
  }
  
  MPI_Gather(localC , N*local_nrows, MPI_DOUBLE, 
             globalC, N*local_nrows, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  if (my_rank == 0) {
    printf("globalC = \n"); printMat(N, N, globalC);
  } 

  free(v);
  free(local_v);
  free(globalC);
  free(localC);
  free(localB);
  free(localA);
  MPI_Finalize();
}

