#include "GreedyActivityAlgorithm.hpp"
#include "MathHelper.hpp"
using namespace std;

namespace maxcut {

bool GreedyActivityAlgorithm::greedyIteration() {
  bool improved = false;
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
    updateActivity({best_node});
    improved = true;
  }
  evaluation_count++;
  return improved;
}

bool GreedyActivityAlgorithm::activityIteration() {
  bool improved = false;
  int most_active_node = -1, best_activity = ActivityAlgorithm::ACT_MIN;
  for (int node = 0; node < _adj_list->node_count; ++node) {
    if (_activity[node] > best_activity) {
      best_activity = _activity[node];
      most_active_node = node;
    }
  }
  if (most_active_node > 1) {
    flipNode(most_active_node);
    updateActivity({most_active_node});
    decayActivity();
    improved = true;
  }
  evaluation_count++;
  return improved;
}

void GreedyActivityAlgorithm::run() {
  while (!stop) {
    while (greedyIteration() && !stop)
      ;

    for (int activity_duration = _adj_list->node_count / 5;
         !stop && activity_duration && activityIteration(); --activity_duration)
      ;
  }
}

} // namespace maxcut
