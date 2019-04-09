#pragma once
#include "mbit.hpp"
#include <random>
class MC {
protected:
  std::string answer;
  mbit _data;
  void dig(void);
  double energy;
  std::mt19937 mt;
  std::uniform_real_distribution<double> ud;
  unsigned int myrand(const unsigned int n) {
    return static_cast<unsigned int>(ud(mt) * n);
  }
  double myrand_real(void) {
    return ud(mt);
  }

public:
  MC(const std::string &a)
      : answer(a), mt(1) {
    _data = mbit81mask;
    energy = 0.0;
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
  void reset(const std::string &a) {
    answer = a;
    energy = 0.0;
  }
  double current_energy(void) {
    return energy;
  }
  void seed(int s) {
    mt.seed(s);
  }
  const std::string current_data(void) {
    return mbit2str(_data, answer);
  }
  mbit random_remove(const mbit &s);
  mbit random_add(const mbit &s);
  virtual void onestep(const double beta) = 0;
};