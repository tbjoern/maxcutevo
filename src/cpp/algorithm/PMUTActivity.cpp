#include "PMUTActivity.hpp"

namespace maxcut {

PMUTActivity::PMUTActivity(Algorithm::Parameters params,
                           ActivityAlgorithm::Parameters parameters,
                           double power_law_param)
    : ActivityAlgorithm(params, parameters),
      plaw_gen(_node_count, power_law_param, helper.getEngine()) {
  helper.setPowerLawParam(power_law_param);
}

void PMUTActivity::iteration() {
  auto k = plaw_gen.get() + 1;
  auto nodes_to_flip = helper.chooseKUnique(_activity, k);
  bool flipped = flipNodesIfBetterCut(nodes_to_flip);

  if (flipped) {
    updateActivity(nodes_to_flip);
    decayActivity();
  }
}

} // namespace maxcut
