#include "UnifAlgorithm.hpp"
#include "MathHelper.hpp"

using namespace std;

namespace maxcut {

void UnifAlgorithm::run() {
  helper.setUniformRange(0, _node_count);

  while (evaluation_count < 3000) {
    vector<int> nodes_to_flip;
    for (int node = 0; node < _node_count; ++node) {
      if (helper.getInt() == 1) {
        nodes_to_flip.push_back(node);
      }
    }
    if (nodes_to_flip.size()) {
      flipNodesIfBetterCut(nodes_to_flip);
    }
    evaluation_count++;
  }
}
} // namespace maxcut
