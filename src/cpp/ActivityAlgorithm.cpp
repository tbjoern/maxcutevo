#include "ActivityAlgorithm.hpp"
#include "MathHelper.hpp"
#include <iostream>

using namespace std;

namespace maxcut {

void ActivityAlgorithm::run() {
  const auto &adj_list = *_adj_list;
  vector<int> pop(_node_count), weights(_node_count, 1);
  for (int node = 0; node < _node_count; ++node) {
    pop[node] = node;
  }

  helper.setPowerLawParam(2);
  helper.setUniformRange(0, _node_count);

  const int activity_weight = _reverse ? -1 : 1;

  while (!stop) {
    auto k = helper.getIntFromPowerLawDistribution(_node_count);
    auto nodes_to_flip = helper.chooseKUnique(pop, weights, k);

    for (const auto node : nodes_to_flip) {
      weights[node] = 1;
    }

    for (const auto node : nodes_to_flip) {
      const auto &neighbours = adj_list[node];
      const auto node_part = _part[node];
      for (const auto edge : neighbours) {
        auto &neighbour = edge.first;
        if (_part[neighbour] == node_part) {
          weights[neighbour] -= activity_weight;
        } else {
          weights[neighbour] += activity_weight;
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
