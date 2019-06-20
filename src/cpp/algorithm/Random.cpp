#include "Random.hpp"
#include "../MathHelper.hpp"

using namespace std;

namespace maxcut {

Random::Random(Algorithm::Parameters params) : Algorithm(params) {
  gen = helper.probabilitySampler(0.5);
}

void Random::iteration() {
  vector<int> nodes_to_flip;
  for (int node = 0; node < _node_count; ++node) {
    if (gen.get()) {
      nodes_to_flip.push_back(node);
    }
  }
  flipNodes(nodes_to_flip);
  _max_cut_weight = _cut_weight;
}

} // namespace maxcut
