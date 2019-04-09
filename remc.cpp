#include "remc.hpp"
#include "mpistream.hpp"
#include "rater.hpp"
#include <assert.h>

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
