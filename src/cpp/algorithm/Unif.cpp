#include "Unif.hpp"
#include "../MathHelper.hpp"

using namespace std;

namespace maxcut {

Unif::Unif(const AdjList &adj_list) : Algorithm(adj_list) {
  helper.setUniformRange(0, _node_count);
  helper.setRealRange(0, 1);
  _probability = 1.0 / _node_count;
}

void Unif::iteration() {
  vector<int> nodes_to_flip;
  for (int node = 0; node < _node_count; ++node) {
    if (helper.sampleProbability(_probability)) {
      nodes_to_flip.push_back(node);
    }
  }
  flipNodesIfBetterCut(nodes_to_flip);
}
} // namespace maxcut
