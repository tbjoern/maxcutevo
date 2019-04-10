#include "PMUTActivity.hpp"

namespace maxcut {

PMUTActivity::PMUTActivity(const AdjList &adj_list, double power_law_param)
    : ActivityAlgorithm(adj_list) {
  helper.setPowerLawParam(power_law_param);
}

void PMUTActivity::iteration() {
  auto k = helper.getIntFromPowerLawDistribution(_node_count);
  auto nodes_to_flip = helper.chooseKUnique(_activity, k);
  bool flipped = flipNodesIfBetterCut(nodes_to_flip);

  if (flipped) {
    updateActivity(nodes_to_flip);
    decayActivity();
  }
}

} // namespace maxcut
