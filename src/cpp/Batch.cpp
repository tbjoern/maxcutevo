#include "Batch.hpp"
#include <chrono>
#include <csignal>
#include <functional>
#include <mutex>
#include <omp.h>
#include <thread>
#include <unistd.h>
#include <vector>

namespace maxcut {

AlgorithmResult runAlgorithm(const AdjList &adj_list,
                             std::shared_ptr<Algorithm> algorithm,
                             const RunConfig config) {
  AlgorithmResult result;
  result.algorithm_id = algorithm->id;
  for (int run_nr = 0; run_nr < config.run_count; ++run_nr) {
    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<int> cut_sizes;

    algorithm->setGraph(adj_list);
    algorithm->_init();

    for (int iteration = 0; iteration < config.max_iterations; ++iteration) {
      algorithm->_iteration();
      cut_sizes.push_back(algorithm->getCutSize());
    }

    auto stop_time = std::chrono::high_resolution_clock::now();
    double total_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                            stop_time - start_time)
                            .count();
    result.run_results.push_back({algorithm->calcCutSizes(),
                                  std::move(cut_sizes), total_time,
                                  algorithm->evaluation_count});
  }
  return result;
}

std::vector<AlgorithmResult>
batch(AdjList &adj_list, std::vector<std::shared_ptr<Algorithm>> &algorithms,
      const RunConfig config) {

  std::vector<AlgorithmResult> results;
  std::mutex mutex;

#pragma omp parallel
  for (auto &algorithm : algorithms) {
    auto result = runAlgorithm(adj_list, algorithm, config);
    std::lock_guard<std::mutex> lock(mutex);
    results.push_back(result);
  }

  return results;
}

} // namespace maxcut
