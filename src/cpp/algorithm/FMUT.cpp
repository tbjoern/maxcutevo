#include "FMUT.hpp"
#include "../MathHelper.hpp"

using namespace std;

namespace maxcut {

FMUT::FMUT(Algorithm::Parameters params, double power_law_beta)
    : Algorithm(params), power_law_beta(power_law_beta),
      plaw_gen(_node_count / 2, power_law_beta, helper.getEngine()) {
  helper.setPowerLawParam(power_law_beta);
}

void FMUT::iteration() {
  vector<int> nodes_to_flip;
  auto k = plaw_gen.get();
  auto sampler =
      helper.probabilitySampler(static_cast<double>(k) / _node_count);
  for (int node = 0; node < _node_count; ++node) {
    if (sampler->get()) {
      nodes_to_flip.push_back(node);
    }
  }
  flipNodesIfBetterCut(nodes_to_flip);
}
} // namespace maxcut
