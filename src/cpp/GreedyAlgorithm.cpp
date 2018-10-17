#include "GreedyAlgorithm.hpp"
#include "MathHelper.hpp"

#include <tuple>

using namespace std;

namespace maxcut {

void GreedyAlgorithm::run() {
  helper.setUniformRange(0, 1);
  //   while (!stop) {
  // for (int node = 0; node < _adj_list->node_count; ++node) {
  //   _part[node] = -1 + 2 * helper.getInt();
  // }
  // calculateChange();
  // _cut_weight = calculateCurrentCutSize().first;
  bool improved = false;
  do {
    improved = false;
    int best_node = -1, improvement = 0;
    for (int node = 0; node < _adj_list->node_count; ++node) {
      if (_change[node] > improvement) {
        best_node = node;
        improvement = _change[node];
      }
    }
    if (best_node > -1) {
      flipNode(best_node);
      improved = true;
    }
    evaluation_count++;
  } while (improved && !stop);
  //   }
}
} // namespace maxcut
