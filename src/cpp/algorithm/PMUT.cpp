#include "PMUT.hpp"
#include "../MathHelper.hpp"

using namespace std;

namespace maxcut {

void PMUT::init() {
  _pop = vector<int>(_node_count);
  _weights = vector<int>(_node_count, 1);
  for (int node = 0; node < _node_count; ++node) {
    _pop[node] = node;
  }

  helper.setPowerLawParam(2);
  helper.setUniformRange(0, _node_count);
}

void PMUT::iteration() {
  auto k = helper.getIntFromPowerLawDistribution(_node_count);
  auto nodes_to_flip = helper.chooseKUnique(_pop, _weights, k);
  flipNodesIfBetterCut(nodes_to_flip);
}

} // namespace maxcut
