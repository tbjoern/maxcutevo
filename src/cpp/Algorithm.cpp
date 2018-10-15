#include "Algorithm.hpp"

#include <exception>
#include <iostream>
#include <unordered_set>

namespace {
/**
 * for every node that is part of the CUT_SET the weight of the outgoing edges
 * that have an endpoint in the NOT_CUT_SET is counted towards the cut size.
 */
enum side { CUT_SET = -1, NOT_CUT_SET = 1 };
} // namespace

namespace maxcut {

void Algorithm::init() {
  const AdjList &adj_list = *_adj_list;
  stop = 0;
  _part = std::vector<int>(adj_list.node_count, int(NOT_CUT_SET));
  _node_count = adj_list.node_count;
  _cut_weight = 0;
  _max_cut_weight = 0;
}

int Algorithm::changeByFlip(int nodeID) {
  _part[nodeID] = -_part[nodeID];
  const AdjList &adj_list = *_adj_list;
  int change = 0;

  switch (_part[nodeID]) {
  case CUT_SET:
    // node moved from NOT_CUT_SET to CUT_SET
    // all incoming edges from nodes that are in CUT_SET
    // do not count towards the cut size anymore
    for (const auto &edge : adj_list.in_edges[nodeID]) {
      if (_part[edge.neighbour] == CUT_SET) {
        change -= edge.weight;
      }
    }
    // all outgoing edges to nodes that are not in CUT_SET do now count
    for (const auto &edge : adj_list.out_edges[nodeID]) {
      if (_part[edge.neighbour] == NOT_CUT_SET) {
        change += edge.weight;
      }
    }
    break;
  case NOT_CUT_SET:
    // node moved from CUT_SET to NOT_CUT_SET
    // all incoming edges from nodes that are in CUT_SET
    // do now count towards the cut size
    for (const auto &edge : adj_list.in_edges[nodeID]) {
      if (_part[edge.neighbour] == CUT_SET) {
        change += edge.weight;
      }
    }
    // all outgoing edges to nodes that are in CUT_SET do not count anymore
    for (const auto &edge : adj_list.out_edges[nodeID]) {
      if (_part[edge.neighbour] == NOT_CUT_SET) {
        change -= edge.weight;
      }
    }
  }
  _part[nodeID] = -_part[nodeID];
  return change;
}

void Algorithm::flipNode(int nodeID) {
  _cut_weight += changeByFlip(nodeID);
  _part[nodeID] = -_part[nodeID];
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
  auto tmp_part = _part;
  auto tmp_max_weight = _max_cut_weight;
  auto tmp_cut_weight = _cut_weight;
  flipNodes(nodeIDs);
  if (tmp_max_weight >= _max_cut_weight) {
    _part = std::move(tmp_part);
    _max_cut_weight = tmp_cut_weight;
    _cut_weight = tmp_cut_weight;
    return false;
  }
  return true;
}

Cut Algorithm::calcCutSizes() {
  Cut cut{0, 0};
  for (int node = 0; node < _node_count; ++node) {
    for (const auto &edge : _adj_list->out_edges[node]) {
      if (_part[node] != _part[edge.neighbour]) {
        if (_part[node] == 1) {
          cut.size += edge.weight;
        } else {
          cut.inverse_size += edge.weight;
        }
      }
    }
  }
  if (_max_cut_weight != cut.inverse_size && _max_cut_weight != cut.size) {
    std::cout << "Actual cut weight and tracked cut weight do not match. "
              << this->name() << " actual:" << cut.size << "|"
              << cut.inverse_size << " tracked:" << _max_cut_weight
              << std::endl;
  }
  return cut;
}

Cut Algorithm::calcSolution(const AdjList &adj_list) {
  _adj_list = &adj_list;
  this->init();
  this->run();
  return calcCutSizes();
}

} // namespace maxcut
