#pragma once
#include "smr/rater.hpp"
#include "sudoku_lib/grid.hpp"
#include "sudoku_lib/mbit.hpp"
#include <random>

__attribute__((weak)) Rater rater(100000);

class MC {
private:
  std::string answer;
  mbit _data;
  double energy;
  std::mt19937 mt;
  std::uniform_real_distribution<double> ud;
  unsigned int myrand(const unsigned int n) {
    return static_cast<unsigned int>(ud(mt) * n);
  }
  double myrand_real(void) {
    return ud(mt);
  }

  bool is_unique(mbit m) {
    std::string str = mbit2str(m, answer);
    return Grid::is_unique(str);
  }

  double calc_energy(mbit m) {
    std::string str = mbit2str(m, answer);
    return static_cast<double>(rater.rate(str.c_str()));
  }

public:
  MC(const std::string &a)
      : answer(a), mt(1) {
    _data = mask81;
    energy = 0.0;
    for (int i = 0; i < 10000; i++) {
      mbit ns = random_remove(_data);
      if (is_unique(ns)) {
        _data = ns;
      }
    }
    energy = calc_energy(_data);
  }

  mbit random_remove(const mbit &s) {
    mbit t = s;
    const unsigned int n = myrand(popcnt_u128(s));
    for (unsigned int i = 0; i < n; i++) {
      t = t ^ (t & -t);
    }
    return (s ^ (t & -t));
  }

  mbit random_add(const mbit &s) {
    static const mbit mask = (mbit(1) << 81) - 1;
    mbit t = (~s) & mask;
    const unsigned int n = myrand(popcnt_u128(t));
    for (unsigned int i = 0; i < n; i++) {
      t = t ^ (t & -t);
    }
    return (s | (t & -t));
  }

  double current_energy(void) {
    return energy;
  }

  const std::string current_data(void) {
    return mbit2str(_data, answer);
  }

  void onestep(const double beta) {
    mbit ns = random_remove(_data);
    if (is_unique(ns)) {
      _data = ns;
      energy = calc_energy(_data);
    }
    ns = random_add(_data);
    double n_energy = calc_energy(ns);
    if (myrand_real() < exp(beta * (n_energy - energy))) {
      _data = ns;
      energy = calc_energy(_data);
    }
  }
};