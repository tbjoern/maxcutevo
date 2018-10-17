#include "UnifActivityAlgorithm.hpp"
#include "MathHelper.hpp"

using namespace std;

namespace maxcut {

void UnifActivityAlgorithm::run() {
  constexpr int START_ACTIVITY = 5;
  constexpr int ACT_MIN = -10;
  constexpr int ACT_MAX = 10;
  constexpr int ACT_INC = 1;
  constexpr int ACT_DEC = 1;
  constexpr double DECAY_RATE = 0.95;
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
          switch (_part[node]) {
          case CUT_SET:
            for (const auto &edge : _adj_list->out_edges[node]) {
              switch (_part[edge.neighbour]) {
              case CUT_SET:
                weights[edge.neighbour] += ACT_INC * activity_mod;
                break;
              case NOT_CUT_SET:
                weights[edge.neighbour] -= ACT_DEC * activity_mod;
                break;
              }
              if (weights[edge.neighbour] > ACT_MAX) {
                weights[edge.neighbour] = ACT_MAX;
              }
              if (weights[edge.neighbour] < ACT_MIN) {
                weights[edge.neighbour] = ACT_MIN;
              }
            }
            break;
          case NOT_CUT_SET:
            for (const auto &edge : _adj_list->in_edges[node]) {
              switch (_part[edge.neighbour]) {
              case CUT_SET:
                weights[edge.neighbour] -= ACT_DEC * activity_mod;
                break;
              case NOT_CUT_SET:
                weights[edge.neighbour] += ACT_INC * activity_mod;
                break;
              }
              if (weights[edge.neighbour] > ACT_MAX) {
                weights[edge.neighbour] = ACT_MAX;
              }
              if (weights[edge.neighbour] < ACT_MIN) {
                weights[edge.neighbour] = ACT_MIN;
              }
            }
            break;
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
