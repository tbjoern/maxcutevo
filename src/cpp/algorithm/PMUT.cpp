#include "PMUT.hpp"
#include "../MathHelper.hpp"

using namespace std;

namespace maxcut {

PMUT::PMUT(Algorithm::Parameters params, double power_law_beta)
    : Algorithm(params), power_law_beta(power_law_beta),
      plaw_gen(_node_count, power_law_beta, helper.getEngine()) {
  _weights = vector<int>(_node_count, 1);

  helper.setPowerLawParam(power_law_beta);
}

void PMUT::iteration() {
  auto k = plaw_gen.get() + 1;
  auto nodes_to_flip = helper.chooseKUnique(_weights, k);
  flipNodesIfBetterCut(nodes_to_flip);
}

} // namespace maxcut
