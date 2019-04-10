#include "Batch.hpp"
#include "AlgorithmFactory.hpp"
#include <chrono>
#include <functional>
#include <mutex>
#include <omp.h>
#include <vector>

namespace maxcut {

RunResult execute(const Run &run) {
  RunResult result;
  auto algorithm = AlgorithmFactory::make(
      run.algorithm_config.name, run.adj_list, run.algorithm_config.arguments);
  auto start_time = std::chrono::high_resolution_clock::now();

  for (int iteration = 0; iteration < run.iterations; ++iteration) {
    algorithm->iteration();
    result.cut_sizes.push_back(algorithm->getCutSize());
  }

  auto stop_time = std::chrono::high_resolution_clock::now();
  result.time = std::chrono::duration_cast<std::chrono::milliseconds>(
                    stop_time - start_time)
                    .count();
  return result;
}

std::vector<RunResult> batch(const std::vector<Run> &runs) {

  std::vector<RunResult> results;
  std::mutex mutex;

#pragma omp parallel
  for (const auto &run : runs) {
    auto result = execute(run);
    std::lock_guard<std::mutex> lock(mutex);
    results.push_back(result);
  }

  return results;
}

} // namespace maxcut
