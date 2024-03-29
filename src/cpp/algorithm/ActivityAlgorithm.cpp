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
    _activity[node] = parameters.START_ACTIVITY;
  }
  for (const auto node : flipped_nodes) {
    switch (_part[node]) {
    case CUT_SET:
      for (const auto &edge : _adj_list.out_edges[node]) {
        switch (_part[edge.neighbour]) {
        case CUT_SET:
          _activity[edge.neighbour] += parameters.ACT_INC;
          break;
        case NOT_CUT_SET:
          _activity[edge.neighbour] -= parameters.ACT_DEC;
          break;
        }
        if (_activity[edge.neighbour] > parameters.ACT_MAX) {
          _activity[edge.neighbour] = parameters.ACT_MAX;
        }
        if (_activity[edge.neighbour] < parameters.ACT_MIN) {
          _activity[edge.neighbour] = parameters.ACT_MIN;
        }
      }
      break;
    case NOT_CUT_SET:
      for (const auto &edge : _adj_list.in_edges[node]) {
        switch (_part[edge.neighbour]) {
        case CUT_SET:
          _activity[edge.neighbour] -= parameters.ACT_DEC;
          break;
        case NOT_CUT_SET:
          _activity[edge.neighbour] += parameters.ACT_INC;
          break;
        }
        if (_activity[edge.neighbour] > parameters.ACT_MAX) {
          _activity[edge.neighbour] = parameters.ACT_MAX;
        }
        if (_activity[edge.neighbour] < parameters.ACT_MIN) {
          _activity[edge.neighbour] = parameters.ACT_MIN;
        }
      }
      break;
    }
  }
}

void ActivityAlgorithm::decayActivity() {
  for (auto &activity : _activity) {
    activity = (activity - parameters.START_ACTIVITY) * parameters.DECAY_RATE +
               parameters.START_ACTIVITY;
  }
}

ActivityAlgorithm::ActivityAlgorithm(Algorithm::Parameters params,
                                     Parameters parameters)
    : Algorithm(params), _activity(_node_count, parameters.START_ACTIVITY),
      parameters(parameters) {}

std::vector<NodeInfo> ActivityAlgorithm::getNodeInfo() {
  auto result = this->Algorithm::getNodeInfo();
  for (int node = 0; node < _node_count; ++node) {
    NodeInfo &info = result[node];
    info.activity = _activity[node];
  }
  return result;
}

} // namespace maxcut
