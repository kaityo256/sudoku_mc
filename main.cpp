#include "mpistream/mpistream.hpp"
#include "remc.hpp"
#include "sudoku_lib/ansmaker.hpp"
#include <mpi.h>

int main(int argc, char **argv) {
  int rank = 0;
  int procs = 0;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  mout.init(rank, procs);
  std::vector<MC> v;
  const int seed = rank;
  AnsMaker am(seed);
  REMC remc;
  double beta = 0.01;
  for (int i = 0; i < 10; i++) {
    std::string answer = am.make();
    MC m(answer, seed);
    remc.add(m, beta);
    beta *= 2.0;
  }
  for (int i = 0; i < 10000; i++) {
    remc.onestep();
  }
  MPI_Finalize();
}
