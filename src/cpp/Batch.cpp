#include "Batch.hpp"
#include <csignal>
#include <functional>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <vector>
#include <chrono>

namespace maxcut {

RunResult runAlgorithm(const AdjList &adj_list, std::shared_ptr<Algorithm> algorithm, const int max_duration, const int max_iterations) {
  auto start_time = std::chrono::high_resolution_clock::now();
  auto current_time = start_time;
  auto max_time = start_time + std::chrono::seconds(max_duration);

  std::vector<int> cut_sizes;

  algorithm->init();

  for(int iteration = 0; iteration < max_iterations && current_time < max_time; ++iteration, current_time = std::chrono::high_resolution_clock::now()) {
    algorithm->iteration();
    cut_sizes.push_back(algorithm->getCutSize());
  }

  auto stop_time = std::chrono::high_resolution_clock::now();
  double total_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                          stop_time - start_time)
                          .count();

  return {algorithm->name(), cut_sizes, total_time, algorithm->evaluation_count};
}

std::mutex mutex;

void runner_thread(const AdjList &adj_list,
                   std::shared_ptr<Algorithm> algorithm,
                   std::vector<RunResult> &results,
                   const int max_time, const int max_iterations) {
  auto result = runAlgorithm(adj_list, algorithm, max_time, max_iterations);
  std::lock_guard<std::mutex> lock(mutex);
  results.push_back(result);
}

std::vector<RunResult> batch(AdjList &adj_list, std::vector<std::shared_ptr<Algorithm>> &algorithms, const int max_time, const int max_iterations) {

  std::vector<RunResult> results;
  std::vector<std::thread> threads;
  for (auto &algorithm : algorithms) {
    threads.emplace_back(runner_thread, std::ref(adj_list), algorithm, std::ref(results), max_time, max_iterations);
  }

  for (auto &thread : threads) {
    thread.join();
  }

  return results;
}

} // namespace maxcut
