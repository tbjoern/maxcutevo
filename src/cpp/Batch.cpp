#include "Batch.hpp"
#include "AlgorithmFactory.hpp"
#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <omp.h>
#include <vector>

using namespace std;

namespace {
inline bool record_iteration(int it) {
  if (it < 1000) {
    return it % 10 == 0;
  } else {
    return it % 100 == 0;
  }
}
} // namespace

namespace maxcut {

void write_result_to_stream(const RunResult &result, ostream &stream) {
  int algorithm_id = result.algorithm_id;
  int run_nr = result.run_id;
  const auto &run_data = result.cut_sizes;
  const auto &iteration_data = result.iterations;
  assert(run_data.size() == iteration_data.size());
  for (int it = 0; it < run_data.size(); ++it) {
    stream << run_nr << "," << algorithm_id << "," << iteration_data[it] << ","
           << run_data[it] << endl;
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

  for (int iteration = 0; iteration <= run.iterations; ++iteration) {
    algorithm->iteration();
    if (record_iteration(iteration)) {
      result.cut_sizes.push_back(algorithm->getCutSize());
      result.iterations.push_back(iteration);
    }
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
