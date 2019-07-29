#include "Algorithm.hpp"

#include <exception>
#include <iostream>
#include <unordered_set>

namespace maxcut {

using maxcut::side;

Algorithm::Algorithm(Parameters params)
    : _part(params.adj_list.node_count, int(NOT_CUT_SET)),
      _change(params.adj_list.node_count, 0), _cut_weight(0),
      _max_cut_weight(0), _node_count(params.adj_list.node_count),
      _nodes_flipped(0), _adj_list(params.adj_list) {

  for (int node = 0; node < _node_count; ++node) {
    for (const auto &edge : _adj_list.out_edges[node]) {
      _change[node] += edge.weight;
    }
  }

  if (params.use_start_assignment) {
    assert(params.start_assignment.size() == _node_count);
    for (int node = 0; node < _node_count; ++node) {
      if (params.start_assignment[node] == CUT_SET) {
        flipNode(node);
      }
    }
    _max_cut_weight = _cut_weight;
  }
}

void Algorithm::calculateChange() {
  for (int node = 0; node < _adj_list.node_count; ++node) {
    for (const auto &edge : _adj_list.out_edges[node]) {
      _change[node] += edge.weight * _part[node] * _part[edge.neighbour];
      _change[edge.neighbour] -=
          edge.weight * _part[node] * _part[edge.neighbour];
    }
    for (const auto &edge : _adj_list.in_edges[node]) {
      _change[node] -= edge.weight * _part[node] * _part[edge.neighbour];
      _change[edge.neighbour] +=
          edge.weight * _part[node] * _part[edge.neighbour];
    }
  }
}

std::pair<int, int> Algorithm::calculateCurrentCutSize() {
  std::pair<int, int> cut;
  for (int node = 0; node < _node_count; ++node) {
    for (const auto &edge : _adj_list.out_edges[node]) {
      if (_part[node] != _part[edge.neighbour]) {
        if (_part[node] == CUT_SET) {
          cut.first += edge.weight;
        } else {
          cut.second += edge.weight;
        }
      }
    }
  }
  return cut;
}

int Algorithm::changeByFlip(int nodeID) { return _change[nodeID]; }

void Algorithm::flipNode(int nodeID) {
  _cut_weight += changeByFlip(nodeID);
  _part[nodeID] = -_part[nodeID];
  _change[nodeID] = -_change[nodeID];

  for (const auto &edge : _adj_list.in_edges[nodeID]) {
    _change[edge.neighbour] +=
        edge.weight * _part[nodeID] * _part[edge.neighbour];
  }
  for (const auto &edge : _adj_list.out_edges[nodeID]) {
    _change[edge.neighbour] +=
        edge.weight * _part[nodeID] * _part[edge.neighbour];
  }

  if (_cut_weight > _max_cut_weight) {
    _max_cut_weight = _cut_weight;
  }
  ++_nodes_flipped;
}

void Algorithm::flipNodes(std::vector<int> &nodeIDs) {
  for (auto nodeID : nodeIDs) {
    flipNode(nodeID);
  }
}

bool Algorithm::flipNodesIfBetterCut(std::vector<int> &nodeIDs) {
  auto prev_cut_weight = _cut_weight;
  flipNodes(nodeIDs);
  if (prev_cut_weight > _cut_weight) {
    flipNodes(nodeIDs);
    return false;
  }
  _max_cut_weight = _cut_weight;
  return true;
}

bool Algorithm::flipNodesIfAboveThreshold(std::vector<int> &nodeIDs,
                                          double threshold_factor) {
  auto threshold_cut_weight = _max_cut_weight * threshold_factor;
  flipNodes(nodeIDs);
  if (threshold_cut_weight > _cut_weight) {
    flipNodes(nodeIDs);
    return false;
  }
  return true;
}

Cut Algorithm::calcCutSizes() {
  Cut cut{0, 0, _max_cut_weight};
  auto sizes = calculateCurrentCutSize();
  cut.size = sizes.first;
  cut.inverse_size = sizes.second;
  if (_cut_weight != cut.inverse_size && _cut_weight != cut.size) {
    std::cout << "Actual cut weight and tracked cut weight do not match. "
              << " actual:" << cut.size << "|" << cut.inverse_size
              << " tracked:" << _max_cut_weight << std::endl;
  }
  return cut;
}

int Algorithm::getCutSize() { return _max_cut_weight; }

std::vector<NodeInfo> Algorithm::getNodeInfo() {
  std::vector<NodeInfo> result(_node_count);
  for (int node = 0; node < _node_count; ++node) {
    NodeInfo &info = result[node];
    info.in_degree = _adj_list.in_edges[node].size();
    info.out_degree = _adj_list.out_edges[node].size();
  }
  return result;
}

int Algorithm::getNodesFlipped() const { return _nodes_flipped; }

void Algorithm::clear_stats() { _nodes_flipped = 0; }
} // namespace maxcut
