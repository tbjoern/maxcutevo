#include "ActivityAlgorithm.hpp"
#include "Algorithm.hpp"
#include "MathHelper.hpp"
#include "types.hpp"
#include <iostream>

using namespace std;

namespace {}

namespace maxcut {

void ActivityAlgorithm::run() {
  constexpr int START_ACTIVITY = 10;
  constexpr int ACT_INC = 1;
  constexpr int ACT_DEC = 1;
  constexpr int ACT_MAX = 20;
  constexpr int ACT_MIN = 1;
  // constexpr int DECAY_TIME = 10000;
  constexpr double DECAY_RATE = 0.95;
  vector<int> pop(_node_count), weights(_node_count, START_ACTIVITY);
  for (int node = 0; node < _node_count; ++node) {
    pop[node] = node;
  }

  helper.setPowerLawParam(2);
  helper.setUniformRange(0, _node_count);

  const int activity_mod = _reverse ? -1 : 1;

  // int decay_timer = 0;

  while (!stop) {
    auto k = helper.getIntFromPowerLawDistribution(_node_count);
    auto nodes_to_flip = helper.chooseKUnique(pop, weights, k);
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

    for (auto &weight : weights) {
      weight = ceil(weight * DECAY_RATE);
    }
    // ++decay_timer;
    // if (decay_timer == DECAY_TIME) {
    // }
  }
}

} // namespace maxcut
