#include "GreedyActivity.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

namespace maxcut {

GreedyActivity::GreedyActivity(Algorithm::Parameters params,
                               ActivityAlgorithm::Parameters parameters)
    : ActivityAlgorithm(params, parameters) {}

void GreedyActivity::iteration() {
  int best_node = -1, improvement = 0;
  for (int node = 0; node < _adj_list.node_count; ++node) {
    if (_change[node] > improvement) {
      best_node = node;
      improvement = _change[node];
    }
  }
  if (best_node > -1) {
    flipNode(best_node);
    updateActivity({best_node});
    decayActivity();
  } else {
    std::vector<int> candidates;
    for (int node = 0; node < _adj_list.node_count; ++node) {
      if (_activity[node] > parameters.START_ACTIVITY) {
        candidates.push_back(node);
      }
    }
    sort(candidates.begin(), candidates.end(), [this](int a, int b) {
      return this->_activity[a] > this->_activity[b];
    });
    uint threshold = std::max(candidates.size() / 10, 10ul);
    threshold = std::min(threshold, static_cast<uint>(candidates.size()));
    std::vector<int> chosen_nodes;
    for (int i = 0; i < threshold; ++i) {
      chosen_nodes.push_back(candidates[i]);
    }
    flipNodes(chosen_nodes);
    updateActivity(chosen_nodes);
    decayActivity();
    _max_cut_weight = _cut_weight;
  }
}

} // namespace maxcut
