#include "UnifActivityAlgorithm.hpp"
#include "MathHelper.hpp"

using namespace std;

namespace maxcut {

void UnifActivityAlgorithm::run() {
  constexpr int START_ACTIVITY = 5;
  constexpr int MIN_ACTIVITY = -10;
  constexpr int MAX_ACTIVITY = 10;
  constexpr int ACT_INC = 1;
  constexpr int ACT_DEC = 1;
  constexpr double DECAY_RATE = 0.95;
  auto &adj_list = *_adj_list;
  helper.setUniformRange(0, _node_count);

  vector<double> weights(_node_count, START_ACTIVITY);

  const int activity_mod = _reverse ? -1 : 1;

  while (!stop) {
    vector<int> nodes_to_flip;
    for (int node = 0; node < _node_count; ++node) {
      if (helper.sampleProbability(helper.sigmoid(weights[node]))) {
        nodes_to_flip.push_back(node);
      }
    }
    if (nodes_to_flip.size()) {
      bool flipped = flipNodesIfBetterCut(nodes_to_flip);

      if (flipped) {
        for (const auto node : nodes_to_flip) {
          weights[node] = START_ACTIVITY;
        }

        for (const auto node : nodes_to_flip) {
          for (const auto &edge : adj_list.in_edges[node]) {
            auto &neighbour = edge.neighbour;
            if (_part[neighbour] == _part[node]) {
              weights[neighbour] -= ACT_DEC * activity_mod;
            } else {
              weights[neighbour] += ACT_INC * activity_mod;
            }
            if (weights[neighbour] < MIN_ACTIVITY) {
              weights[neighbour] = MIN_ACTIVITY;
            }
            if (weights[neighbour] > MAX_ACTIVITY) {
              weights[neighbour] = MAX_ACTIVITY;
            }
          }
          for (const auto &edge : adj_list.out_edges[node]) {
            auto &neighbour = edge.neighbour;
            if (_part[neighbour] == _part[node]) {
              weights[neighbour] -= ACT_DEC * activity_mod;
            } else {
              weights[neighbour] += ACT_INC * activity_mod;
            }
            if (weights[neighbour] < MIN_ACTIVITY) {
              weights[neighbour] = MIN_ACTIVITY;
            }
            if (weights[neighbour] > MAX_ACTIVITY) {
              weights[neighbour] = MAX_ACTIVITY;
            }
          }
        }
      }
    }
    for (auto &weight : weights) {
      weight = weight * DECAY_RATE;
    }
  }
}
} // namespace maxcut
