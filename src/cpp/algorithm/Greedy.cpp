#include "Greedy.hpp"
#include "../MathHelper.hpp"

#include <tuple>

using namespace std;

namespace maxcut {

void Greedy::iteration() {
  _improved = false;
  int best_node = -1, improvement = 0;
  for (int node = 0; node < _adj_list->node_count; ++node) {
    if (_change[node] > improvement) {
      best_node = node;
      improvement = _change[node];
    }
  }
  if (best_node > -1) {
    flipNode(best_node);
    _improved = true;
  }
}
} // namespace maxcut
