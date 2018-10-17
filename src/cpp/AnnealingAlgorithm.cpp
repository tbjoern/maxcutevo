#include "AnnealingAlgorithm.hpp"
#include "MathHelper.hpp"
#include "types.hpp"
#include <iostream>
#include <random>

using namespace std;

namespace maxcut {

void AnnealingAlgorithm::run() {
  AdjList adj_list = *_adj_list;
  helper.setRealRange(0, 1);
  helper.setUniformRange(0, _adj_list->node_count - 1);

  for (double heat = 0; !stop; heat += 2e-6) {
    int k = helper.getInt() % _node_count;
    if (!(helper.getReal() > exp(heat * changeByFlip(k) / _max_cut_weight))) {
      flipNode(k);
    }
    evaluation_count++;
  }
}

} // namespace maxcut
