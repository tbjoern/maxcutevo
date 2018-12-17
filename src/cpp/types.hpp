#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace maxcut {

struct Edge {
  int neighbour; // for in edges: start of edge ; for out edges: end of edge
  int weight;
};

struct AdjList {
  AdjList(int nodes) : in_edges(nodes), out_edges(nodes), node_count(nodes) {}
  ~AdjList() = default;

  AdjList(const AdjList &) = delete;
  AdjList(AdjList &&) = default;

  std::vector<std::vector<Edge>> in_edges;
  std::vector<std::vector<Edge>> out_edges;
  int node_count;
};

struct Cut {
  int size, inverse_size, max_size;
};

struct RunResult {
  std::string algorithmName;
  std::vector<int> cut_sizes;
  double time;
  int evaluation_count;
};

} // namespace maxcut
