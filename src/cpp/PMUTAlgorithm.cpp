#include "PMUTAlgorithm.hpp"
#include "MathHelper.hpp"
#include <random>

using namespace std;

namespace maxcut {

void PMUTAlgorithm::run() {
  vector<int> pop(_node_count), weights(_node_count, 1);
  for (int node = 0; node < _node_count; ++node) {
    pop[node] = node;
  }

  helper.setPowerLawParam(2);
  helper.setUniformRange(0, _node_count);

  while (!stop) {
    auto k = helper.getIntFromPowerLawDistribution(_node_count);
    auto nodes_to_flip = helper.chooseKUnique(pop, weights, k);
    flipNodesIfBetterCut(nodes_to_flip);
    evaluation_count++;
  }
}

} // namespace maxcut
