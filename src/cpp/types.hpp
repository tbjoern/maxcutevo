#pragma once
#include "AdjList.hpp"
#include <iostream>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <vector>

namespace maxcut {

class Algorithm;

enum class START_TYPE { EMPTY = 0, RANDOM = 1, FULL = 2 };

struct Cut {
  int size, inverse_size, max_size;
};

struct NodeInfo {
  double activity;
  int in_degree;
  int out_degree;
};

struct IterationInfo {
  std::vector<NodeInfo> nodeInfo;
  int flip_count;
};

struct RunResult {
  std::vector<int> cut_sizes;
  std::vector<int> iterations;
  std::vector<int> flips;
  std::vector<double> time;
  std::map<int, IterationInfo> iteration_infos;
  int run_id;
  int algorithm_id;
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
  int time_limit;
  START_TYPE random_start;
  std::vector<AlgorithmConfig> algorithms;
};

struct Run {
  const AlgorithmConfig &algorithm_config;
  const AdjList &adj_list;
  const std::vector<char> &start_assigment;
  bool use_start_assigment;
  int run_id;
  int iterations;
  int time_limit;
};

} // namespace maxcut
