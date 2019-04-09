#include "remc.hpp"
#include <mpi.h>

int main(int argc, char **argv) {
  int rank = 0;
  int procs = 0;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  MPI_Finalize();
}
