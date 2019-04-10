#include "ActivityAlgorithm.hpp"
#include "../MathHelper.hpp"
#include "../types.hpp"
#include "Algorithm.hpp"
#include <iostream>

using namespace std;

namespace {}

namespace maxcut {

void ActivityAlgorithm::updateActivity(const std::vector<int> &flipped_nodes) {
  for (const auto node : flipped_nodes) {
    _activity[node] = START_ACTIVITY;
  }
  for (const auto node : flipped_nodes) {
    switch (_part[node]) {
    case CUT_SET:
      for (const auto &edge : _adj_list.out_edges[node]) {
        switch (_part[edge.neighbour]) {
        case CUT_SET:
          _activity[edge.neighbour] += ACT_INC;
          break;
        case NOT_CUT_SET:
          _activity[edge.neighbour] -= ACT_DEC;
          break;
        }
        if (_activity[edge.neighbour] > ACT_MAX) {
          _activity[edge.neighbour] = ACT_MAX;
        }
        if (_activity[edge.neighbour] < ACT_MIN) {
          _activity[edge.neighbour] = ACT_MIN;
        }
      }
      break;
    case NOT_CUT_SET:
      for (const auto &edge : _adj_list.in_edges[node]) {
        switch (_part[edge.neighbour]) {
        case CUT_SET:
          _activity[edge.neighbour] -= ACT_DEC;
          break;
        case NOT_CUT_SET:
          _activity[edge.neighbour] += ACT_INC;
          break;
        }
        if (_activity[edge.neighbour] > ACT_MAX) {
          _activity[edge.neighbour] = ACT_MAX;
        }
        if (_activity[edge.neighbour] < ACT_MIN) {
          _activity[edge.neighbour] = ACT_MIN;
        }
      }
      break;
    }
  }
}

void ActivityAlgorithm::decayActivity() {
  for (auto &weight : _activity) {
    weight = ceil(weight * DECAY_RATE);
  }
}

void ActivityAlgorithm::iteration() {
  auto k = helper.getIntFromPowerLawDistribution(_node_count);
  auto nodes_to_flip = helper.chooseKUnique(_pop, _activity, k);
  bool flipped = flipNodesIfBetterCut(nodes_to_flip);

  if (flipped) {
    updateActivity(nodes_to_flip);
    decayActivity();
  }
}

ActivityAlgorithm::ActivityAlgorithm(const AdjList &adj_list,
                                     double power_law_param)
    : Algorithm(adj_list) {
  _activity = std::vector<double>(_node_count, START_ACTIVITY);

  _pop = vector<int>(_node_count);
  for (int node = 0; node < _node_count; ++node) {
    _pop[node] = node;
  }

  helper.setPowerLawParam(power_law_param);
  helper.setUniformRange(0, _node_count);
}

} // namespace maxcut
