#include "Algorithm.hpp"

#include <exception>
#include <iostream>
#include <unordered_set>

namespace maxcut {

using maxcut::side;

void Algorithm::calculateChange() {
  for (int node = 0; node < _adj_list->node_count; ++node) {
    for (const auto &edge : _adj_list->out_edges[node]) {
      _change[node] += edge.weight * _part[node] * _part[edge.neighbour];
      _change[edge.neighbour] -=
          edge.weight * _part[node] * _part[edge.neighbour];
    }
    for (const auto &edge : _adj_list->in_edges[node]) {
      _change[node] -= edge.weight * _part[node] * _part[edge.neighbour];
      _change[edge.neighbour] +=
          edge.weight * _part[node] * _part[edge.neighbour];
    }
  }
}

std::pair<int, int> Algorithm::calculateCurrentCutSize() {
  std::pair<int, int> cut;
  for (int node = 0; node < _node_count; ++node) {
    for (const auto &edge : _adj_list->out_edges[node]) {
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

void Algorithm::_init() {
  const AdjList &adj_list = *_adj_list;
  _part = std::vector<int>(adj_list.node_count, int(NOT_CUT_SET));
  _change = std::vector<int>(adj_list.node_count, 0);
  for (int node = 0; node < adj_list.node_count; ++node) {
    for (const auto &edge : adj_list.out_edges[node]) {
      _change[node] += edge.weight;
    }
  }
  // calculateChange();
  _node_count = adj_list.node_count;
  _cut_weight = 0;
  _max_cut_weight = 0;
  evaluation_count = 0;
  this->init();
}

int Algorithm::changeByFlip(int nodeID) { return _change[nodeID]; }

void Algorithm::flipNode(int nodeID) {
  _cut_weight += changeByFlip(nodeID);
  _part[nodeID] = -_part[nodeID];
  _change[nodeID] = -_change[nodeID];

  for (const auto &edge : _adj_list->in_edges[nodeID]) {
    _change[edge.neighbour] +=
        edge.weight * _part[nodeID] * _part[edge.neighbour];
  }
  for (const auto &edge : _adj_list->out_edges[nodeID]) {
    _change[edge.neighbour] +=
        edge.weight * _part[nodeID] * _part[edge.neighbour];
  }

  if (_cut_weight > _max_cut_weight) {
    _max_cut_weight = _cut_weight;
  }
}

void Algorithm::flipNodes(std::vector<int> nodeIDs) {
  for (auto nodeID : nodeIDs) {
    flipNode(nodeID);
  }
}

bool Algorithm::flipNodesIfBetterCut(std::vector<int> nodeIDs) {
  auto prev_cut_weight = _cut_weight;
  flipNodes(nodeIDs);
  if (prev_cut_weight > _cut_weight) {
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
              << this->name() << " actual:" << cut.size << "|"
              << cut.inverse_size << " tracked:" << _max_cut_weight
              << std::endl;
  }
  return cut;
}

int Algorithm::getCutSize() { return _max_cut_weight; }

void Algorithm::setGraph(const AdjList &adj_list) { _adj_list = &adj_list; }

void Algorithm::_iteration() {
  this->iteration();
  ++evaluation_count;
}

} // namespace maxcut
