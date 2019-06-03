#pragma once
#include <vector>

namespace maxcut {

struct Edge {
  int neighbour; // for in edges: start of edge ; for out edges: end of edge
  int weight;
};

struct AdjList {
  static bool undirected;
  AdjList(int nodes) : in_edges(nodes), out_edges(nodes), node_count(nodes) {}
  ~AdjList() = default;

  AdjList(const AdjList &) = delete;
  AdjList(AdjList &&) = default;

  std::vector<std::vector<Edge>> in_edges;
  std::vector<std::vector<Edge>> out_edges;
  int node_count;

  void add_edge(int source, int dest, int weight);

  bool edge_exists(int source, int dest);

  Edge &get_edge(int source, int dest);
};

} // namespace maxcut
