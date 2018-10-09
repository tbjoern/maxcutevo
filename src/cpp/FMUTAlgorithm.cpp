#include "FMUTAlgorithm.hpp"
#include "MathHelper.hpp"

using namespace std;

namespace maxcut {

void FMUTAlgorithm::run() {
  auto &helper = MathHelper::getInstance();
  helper.setPowerLawParam(2);
  helper.setUniformRange(0, _node_count);

  while (!stop) {
    auto k = helper.getIntFromPowerLawDistribution(_node_count);
    vector<int> nodes_to_flip;
    for (int node = 0; node < _node_count; ++node) {
      if (helper.getInt() <= k) {
        nodes_to_flip.push_back(node);
      }
    }
    if (nodes_to_flip.size()) {
      flipNodesIfBetterCut(nodes_to_flip);
    }
  }
}

} // namespace maxcut
