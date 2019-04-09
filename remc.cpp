#include "remc.hpp"
#include "mpistream.hpp"
#include "rater.hpp"
#include <assert.h>

REMC::REMC(void)
    : ud(0.0, 1.0) {
  _min_energy = 0.0;
  _step = 0;
}

REMC::~REMC(void) {
  for (auto &m : _mcs) {
    delete m;
  }
}

bool REMC::search_filledgrid(SGrid &g2, const int index, std::string &ans) {
  if (index == 81) {
    ans = g2.data;
    return true;
  }
  int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::shuffle(a, a + 9, mt);
  for (unsigned int i = 0; i < 9; i++) {
    SGrid g(g2.data);
    if (!g.can_put(index, a[i])) continue;
    g.put(index, a[i]);
    if (search_filledgrid(g, index + 1, ans)) return true;
  }
  return false;
}

std::string REMC::make_filledgrid(void) {
  SGrid g;
  std::string ans;
  search_filledgrid(g, 0, ans);
  return ans;
}

void REMC::reset(const std::string &ans) {
  for (auto &m : _mcs) {
    m->reset(ans);
  }
}

void REMC::swap_replica(const int j1, const int j2) {
  const int i1 = _bindex[j1];
  const int i2 = _bindex[j2];
  const double b1 = _beta[j1];
  const double b2 = _beta[j2];
  const double e1 = _mcs[i1]->current_energy();
  const double e2 = _mcs[i2]->current_energy();
  const double w = (b1 - b2) * (e1 - e2);
  assert(b1 < b2);
  assert((b2 - b1) > 0.00000001);
  if (w > 0 || exp(w) > ud(mt)) {
    std::swap(_bindex[j1], _bindex[j2]);
  }
}

void REMC::swap_beta(void) {
  const int n = _mcs.size();
  for (int j = 0; j < n / 2; j++) {
    swap_replica(j * 2, j * 2 + 1);
  }
  for (int j = 0; j < n / 2 - 1; j++) {
    swap_replica(j * 2 + 1, j * 2 + 2);
  }
}

void REMC::onestep(void) {
  static Rater rater(10000);
  _step++;
  const int n = _mcs.size();
  for (int i = 0; i < n; i++) {
    MC *m = _mcs[i];
    m->onestep(_beta[_bindex[i]]);
    const double e = m->current_energy();
    if (_min_energy > e) {
      _min_energy = e;
      mout << m->current_data() << " ";
      mout << rater.rate(m->current_data().c_str()) << " #remc " << std::endl;
    }
  }
  swap_beta();
}

