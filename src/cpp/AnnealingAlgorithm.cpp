#include "AnnealingAlgorithm.hpp"
#include "MathHelper.hpp"
#include "types.hpp"
#include <iostream>
#include <random>

using namespace std;

namespace maxcut {

void AnnealingAlgorithm::run() {
  AdjList adj_list = *_adj_list;
  auto &helper = MathHelper::getInstance();
  helper.setRealRange(0, 1);

  for (double heat = 0; !stop; heat += 2e-6) {
    int k = rand() % _node_count;
    if (!(helper.getReal() > exp(heat * _change[k] / _max_cut_weight))) {
      flipNode(k);
    }
  }
}

} // namespace maxcut
