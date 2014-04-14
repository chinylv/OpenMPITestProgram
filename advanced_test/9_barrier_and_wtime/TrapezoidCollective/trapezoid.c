#include <stdio.h>
#include <math.h>
#include <mpi.h>

double f(double x){
  return exp(x*x);
}

int main(int argc, char *argv[]){
  const int ROOT = 0;
  double integral;         /*definite integral result*/
  double a;                /*left end point*/
  double b;                /*right end point*/
  int    N;                /*number of subdivisions*/
  double w;                /*base width of subdivision*/
  double x;
  int i;
  int my_rank;
  int numprocs;
  
  /* we will need some local variables */
  double local_a, local_b;
  int local_N;
  double lcl_integral;
  
  /* MPI programming begins */
  MPI_Init(&argc, &argv);
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
 
  if(my_rank == ROOT) {
    a = 0.0;
    b = 1.0;
    N = 204800;
  }
  double start, finish;
  MPI_Barrier(MPI_COMM_WORLD);
  start = MPI_Wtime();

  MPI_Bcast(&a, 1, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
  MPI_Bcast(&b, 1, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
  MPI_Bcast(&N, 1, MPI_INT,    ROOT, MPI_COMM_WORLD);

  w = (b-a)/N;   /* we assume we use the same integration step on all processes */
  
  /* Find out what the local values are on each process */  
  local_N = N / numprocs;
  local_a = a + my_rank * local_N * w;
  local_b = local_a + local_N * w;
  
  /* begins local integration */
  x = local_a;
  lcl_integral = (f(local_a)+f(local_b))/2.0;

  for(i = 1; i <= local_N-1; i++) {
      x = local_a+i*w;
      lcl_integral = lcl_integral + f(x);
  }
  lcl_integral = lcl_integral*w;
 
  /* Reduce and send result to ROOT */
  MPI_Reduce(&lcl_integral, &integral, 1, MPI_DOUBLE, MPI_SUM, ROOT, MPI_COMM_WORLD);
  //MPI_Allreduce(&lcl_integral, &integral, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  printf("rank %d, local_N %d, local_a %lf , local_b %lf, lcl_integral %lf\n", my_rank, local_N, local_a, local_b, lcl_integral);
  if(my_rank == ROOT) {
    printf("integral %lf\n", integral);
  }
  /* MPI programming ends */

  MPI_Barrier(MPI_COMM_WORLD);
  finish = MPI_Wtime();
  
  if (my_rank==0) printf ("Elapsed time = %e seconds \n", finish-start);
  MPI_Finalize();

  return 0;
}

