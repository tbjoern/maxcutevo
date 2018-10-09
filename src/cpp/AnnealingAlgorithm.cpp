#include "AnnealingAlgorithm.hpp"
#include "types.hpp"
#include <iostream>
#include <random>

using namespace std;

namespace maxcut {

void AnnealingAlgorithm::run() {
  AdjList adj_list = *_adj_list;

  double heatmax = 10;
  for (double heat = 0; heat < heatmax; heat += 2e-6) {
    int k = rand() % _node_count;
    if (!(drand48() > exp(heat * _change[k] / _max_cut_weight))) {
      flipNode(k);
    }
  }
}

} // namespace maxcut
