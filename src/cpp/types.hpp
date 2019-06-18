#pragma once
#include "AdjList.hpp"
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace maxcut {

class Algorithm;

struct Cut {
  int size, inverse_size, max_size;
};

struct RunResult {
  std::vector<int> cut_sizes;
  std::vector<int> iterations;
  int run_id;
  int algorithm_id;
  double time;
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
  int max_iterations;
  int run_count;
  bool random_start;
  std::vector<AlgorithmConfig> algorithms;
};

struct Run {
  const AlgorithmConfig &algorithm_config;
  const AdjList &adj_list;
  const std::vector<char> &start_assigment;
  bool use_start_assigment;
  int run_id;
  int iterations;
};

struct NodeInfo {
  double activity;
  int in_degree;
  int out_degree;
};

} // namespace maxcut
