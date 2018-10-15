#include "ActivityAlgorithm.hpp"
#include "MathHelper.hpp"
#include "types.hpp"
#include <iostream>

using namespace std;

namespace {}

namespace maxcut {

void ActivityAlgorithm::run() {
  const auto &adj_list = *_adj_list;
  constexpr int START_ACTIVITY = 10;
  constexpr int ACT_INC = 5;
  constexpr int ACT_DEC = 1;
  vector<int> pop(_node_count), weights(_node_count, START_ACTIVITY);
  for (int node = 0; node < _node_count; ++node) {
    pop[node] = node;
  }

  helper.setPowerLawParam(2);
  helper.setUniformRange(0, _node_count);

  const int activity_mod = _reverse ? -1 : 1;

  while (!stop) {
    auto k = helper.getIntFromPowerLawDistribution(_node_count);
    auto nodes_to_flip = helper.chooseKUnique(pop, weights, k);

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
        if (weights[neighbour] < 1) {
          weights[neighbour] = 1;
        }
      }
      for (const auto &edge : adj_list.out_edges[node]) {
        auto &neighbour = edge.neighbour;
        if (_part[neighbour] == _part[node]) {
          weights[neighbour] -= ACT_DEC * activity_mod;
        } else {
          weights[neighbour] += ACT_INC * activity_mod;
        }
        if (weights[neighbour] < 1) {
          weights[neighbour] = 1;
        }
      }
    }

    flipNodesIfBetterCut(nodes_to_flip);
  }
}

} // namespace maxcut
