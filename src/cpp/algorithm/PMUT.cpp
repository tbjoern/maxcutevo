#include "PMUT.hpp"
#include "../MathHelper.hpp"

using namespace std;

namespace maxcut {

PMUT::PMUT(Algorithm::Parameters params, double power_law_beta)
    : Algorithm(params), power_law_beta(power_law_beta) {
  _weights = vector<int>(_node_count, 1);

  helper.setPowerLawParam(power_law_beta);
}

void PMUT::iteration() {
  auto k = helper.getIntFromPowerLawDistribution(_node_count);
  auto nodes_to_flip = helper.chooseKUnique(_weights, k);
  flipNodesIfBetterCut(nodes_to_flip);
}

} // namespace maxcut
