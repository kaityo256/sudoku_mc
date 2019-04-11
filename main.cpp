#include "mpistream/mpistream.hpp"
#include "remc.hpp"
#include "sudoku_lib/ansmaker.hpp"
#include <fstream>
#include <mpi.h>

//レプリカ交換モンテカルロ法による難問探索
void run(const int seed) {
  std::vector<MC> v;
  AnsMaker am(seed);
  REMC remc;
  double beta = 0.01;
  for (int i = 0; i < 10; i++) {
    std::string answer = am.make();
    MC m(answer, seed);
    remc.add(m, beta);
    beta *= 2.0;
  }
  while (true) {
    remc.onestep();
  }
}

void mctest(const double beta, const std::string filename) {
  AnsMaker am(1);
  MC mc(am.make());
  std::ofstream ofs(filename);
  ofs << "# beta = " << beta << std::endl;
  std::cout << filename << " beta = " << beta << std::endl;
  for (int i = 0; i < 1000; i++) {
    mc.onestep(beta);
    ofs << i << " " << mc.current_energy() << std::endl;
  }
}

// モンテカルロ法の温度の効果を確認するためのテストコード
void test() {
  mctest(0.01, "test0_01.dat");
  mctest(0.05, "test0_05.dat");
  mctest(5.0, "test5_00.dat");
}

int main(int argc, char **argv) {
  int rank = 0;
  int procs = 0;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  mout.init(rank, procs);
  //run(rank);
  test();
  MPI_Finalize();
}
