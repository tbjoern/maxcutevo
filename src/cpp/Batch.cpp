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
  int algorithm_id = result.algorithm_id;
  int run_nr = result.run_id;
  const auto &run_data = result.cut_sizes;
  for (int iteration = 0; iteration < run_data.size(); ++iteration) {
    stream << run_nr << "," << algorithm_id << "," << iteration << ","
           << run_data[iteration] << endl;
  }
}

RunResult execute(const Run &run) {
  RunResult result;
  result.algorithm_id = run.algorithm_config.id;
  result.run_id = run.run_id;
  result.cut_sizes.reserve(run.iterations);

  auto algorithm_params = Algorithm::Parameters{
      run.adj_list, run.use_start_assigment, run.start_assigment};

  auto algorithm =
      AlgorithmFactory::make(run.algorithm_config.name, algorithm_params,
                             run.algorithm_config.arguments);
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

  cout << "run_number,algorithm,iteration,cut_weight" << endl;

#pragma omp parallel for
  for (auto it = runs.cbegin(); it < runs.cend(); ++it) {
    auto result = execute(*it);
    std::lock_guard<std::mutex> lock(mutex);
    // results.push_back(result);
    write_result_to_stream(result, cout);
  }

  // return results;
}

} // namespace maxcut
