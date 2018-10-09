#include "Algorithm.hpp"

#include <unordered_set>
#include <vector>

namespace maxcut {

void Algorithm::init() {
  const AdjList &adj_list = *_adj_list;
  stop = 0;
  _change = std::vector(_node_count, 0);
  _part = std::vector(_node_count, 1);
  _cut_weight = 0;
  _max_cut_weight = 0;
  for (int node = 0; node < _node_count; ++node) {
    for (const auto &edge : adj_list[node]) {
      _change[node] += edge.second;
      _change[edge.first] += edge.second;
    }
  }
}

void Algorithm::flipNode(int nodeID) {
  const AdjList &adj_list = *_adj_list;
  _cut_weight += _change[nodeID];
  _part[nodeID] = -_part[nodeID];
  _change[nodeID] = -_change[nodeID];
  for (auto edge : adj_list[nodeID]) {
    int neighbour = edge.first;
    _change[neighbour] +=
        edge.second * (2 - 4 * (_part[nodeID] != _part[neighbour]));
  }
  if (_cut_weight > _max_cut_weight) {
    _max_cut_weight = _cut_weight;
  }
}

void Algorithm::flipNodes(std::vector<int> nodeIDs) {
  const AdjList &adj_list = *_adj_list;
  std::unordered_set<int> nodeID_set(nodeIDs.begin(), nodeIDs.end());
  for (auto nodeID : nodeIDs) {
    _cut_weight += _change[nodeID];
    _part[nodeID] = -_part[nodeID];
    _change[nodeID] = -_change[nodeID];
  }
  for (auto nodeID : nodeIDs) {
    for (auto edge : adj_list[nodeID]) {
      int neighbour = edge.first, weight = edge.second;
      if (nodeID_set.count(neighbour)) {
        if (_part[nodeID] != _part[neighbour]) {
          _cut_weight += 2 * weight;
        } else {
          _cut_weight -= 2 * weight;
        }
      } else {
        _change[neighbour] +=
            weight * (2 - 4 * (_part[nodeID] != _part[neighbour]));
      }
    }
  }
  if (_cut_weight > _max_cut_weight) {
    _max_cut_weight = _cut_weight;
  }
}

bool Algorithm::flipNodesIfBetterCut(std::vector<int> nodeIDs) {
  auto tmp_part = _part;
  auto tmp_change = _change;
  auto cur_max_weight = _max_cut_weight;
  flipNodes(nodeIDs);
  if (cur_max_weight >= _max_cut_weight) {
    _part = std::move(tmp_part);
    _change = std::move(tmp_change);
    return false;
  }
  return true;
}

int Algorithm::calcSolution(const AdjList &adj_list) {
  _adj_list = &adj_list;
  _node_count = adj_list.size();
  this->init();
  this->run();
  return _max_cut_weight;
}

} // namespace maxcut
