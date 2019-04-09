#include "remc.hpp"
#include <mpi.h>

int main(int argc, char **argv) {
  int rank = 0;
  int procs = 0;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  std::vector<MC> v;
  std::string answer = "test";
  REMC remc;
  double beta = 1.00;
  for (int i = 0; i < 10; i++) {
    MC m(answer);
    remc.add(m, beta);
    beta *= 0.5;
  }
  MPI_Finalize();
}
