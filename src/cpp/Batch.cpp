#include "Batch.hpp"
#include "AlgorithmFactory.hpp"
#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <omp.h>
#include <vector>

using namespace std;

namespace maxcut {

void write_result_to_stream(const RunResult &result, ostream &stream) {
  stream << "algorithm,run_number,iteration,cut_weight" << endl;
  int algorithm_id = result.algorithm_id;
  int run_nr = result.run_id;
  const auto &run_data = result.cut_sizes;
  for (int iteration = 0; iteration < run_data.size(); ++iteration) {
    stream << algorithm_id << "," << run_nr << "," << iteration << ","
           << run_data[iteration] << endl;
  }
}

RunResult execute(const Run &run) {
  RunResult result;
  result.algorithm_id = run.algorithm_config.id;
  result.run_id = run.run_id;
  result.cut_sizes.reserve(run.iterations);

  auto algorithm = AlgorithmFactory::make(
      run.algorithm_config.name, run.adj_list, run.algorithm_config.arguments);
  auto start_time = chrono::high_resolution_clock::now();

  for (int iteration = 0; iteration < run.iterations; ++iteration) {
    algorithm->iteration();
    result.cut_sizes.push_back(algorithm->getCutSize());
  }

  auto stop_time = chrono::high_resolution_clock::now();
  result.time =
      chrono::duration_cast<chrono::milliseconds>(stop_time - start_time)
          .count();
  return result;
}

void batch(const vector<Run> &runs) {

  // vector<RunResult> results;
  std::mutex mutex;

#pragma omp parallel
  for (const auto &run : runs) {
    auto result = execute(run);
    std::lock_guard<std::mutex> lock(mutex);
    // results.push_back(result);
    write_result_to_stream(result, cout);
  }

  // return results;
}

} // namespace maxcut
