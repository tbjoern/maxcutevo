#include "Batch.hpp"
#include <chrono>
#include <csignal>
#include <functional>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <vector>

namespace maxcut {

AlgorithmResult runAlgorithm(const AdjList &adj_list,
                             std::shared_ptr<Algorithm> algorithm,
                             const RunConfig config) {
  AlgorithmResult result;
  result.algorithmName = algorithm->name();
  for (int run_nr = 0; run_nr < config.run_count; ++run_nr) {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto current_time = start_time;
    auto max_time = start_time + std::chrono::seconds(config.max_duration);

    std::vector<int> cut_sizes;

    algorithm->setGraph(adj_list);
    algorithm->_init();

    for (int iteration = 0;
         iteration < config.max_iterations && current_time < max_time;
         ++iteration,
             current_time = std::chrono::high_resolution_clock::now()) {
      algorithm->iteration();
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

std::mutex mutex;

void runner_thread(const AdjList &adj_list,
                   std::shared_ptr<Algorithm> algorithm,
                   std::vector<AlgorithmResult> &results,
                   const RunConfig config) {
  auto result = runAlgorithm(adj_list, algorithm, config);
  std::lock_guard<std::mutex> lock(mutex);
  results.push_back(result);
}

std::vector<AlgorithmResult>
batch(AdjList &adj_list, std::vector<std::shared_ptr<Algorithm>> &algorithms,
      const RunConfig config) {

  std::vector<AlgorithmResult> results;
  std::vector<std::thread> threads;
  for (auto &algorithm : algorithms) {
    threads.emplace_back(runner_thread, std::ref(adj_list), algorithm,
                         std::ref(results), config);
  }

  for (auto &thread : threads) {
    thread.join();
  }

  return results;
}

} // namespace maxcut
