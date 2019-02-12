#pragma once
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
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
  Cut cut;
  std::vector<int> cut_sizes;
  double time;
  int evaluation_count;
};

struct AlgorithmResult {
  std::vector<RunResult> run_results;
  int algorithm_id;
};

struct AlgorithmConfig {
  std::string name;
  int id;
  nlohmann::json arguments;
};

struct RunConfig {
  int max_duration;
  int max_iterations;
  int run_count;
  // pair<classname, id>
  std::vector<AlgorithmConfig> algorithms;
};

} // namespace maxcut
