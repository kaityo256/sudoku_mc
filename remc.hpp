#pragma once

#include "mc.hpp"
#include "smr/rater.hpp"
#include "sudoku_lib/grid.hpp"
#include <random>
#include <vector>

class REMC {
private:
  std::vector<MC> _mcs;
  std::vector<double> _beta;
  std::vector<int> _bindex;
  double _min_energy;
  void swap_replica(const int j1, const int j2);
  void swap_beta(void);
  int _step;
  std::mt19937 mt;
  std::uniform_real_distribution<double> ud;

public:
  REMC(void)
      : ud(0.0, 1.0) {
    _min_energy = 0.0;
    _step = 0;
  }

  void reset(const std::string &ans) {
    for (auto &m : _mcs) {
      m.reset(ans);
    }
  }
  void add(MC &m, double b) {
    _mcs.push_back(m);
    _bindex.push_back(_beta.size());
    _beta.push_back(b);
  }
  void onestep(void);
};
