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
inline bool record_cut(int it) {
  if (it < 100) {
    return true;
  } else if (it < 1000) {
    return it % 10 == 0;
  } else {
    return it % 100 == 0;
  }
}

inline bool record_info(int it) { return record_cut(it); }

} // namespace

namespace maxcut {

void write_header(ostream &stream, OutputType output_type) {
  if (output_type == OutputType::CUT_WEIGHT) {
    stream << "run_number,algorithm,iteration,cut_weight,time,flips" << endl;
  } else if (output_type == OutputType::ITERATION_INFO) {
    stream << "iteration,node,in_degree,out_degree,activity" << endl;
  } else if (output_type == OutputType::NODES_FLIPPED) {
    stream << "iteration,flips" << endl;
  }
}

void write_result_to_stream(const RunResult &result, ostream &stream,
                            OutputType output_type) {
  int algorithm_id = result.algorithm_id;
  int run_nr = result.run_id;
  const auto &run_data = result.cut_sizes;
  const auto &iteration_data = result.iterations;
  const auto &iteration_infos = result.iteration_infos;
  const auto &time = result.time;
  const auto &flips = result.flips;
  assert(run_data.size() == iteration_data.size());
  if (output_type == OutputType::CUT_WEIGHT) {
    for (int it = 0; it < run_data.size(); ++it) {
      stream << run_nr << "," << algorithm_id << "," << iteration_data[it]
             << "," << run_data[it] << "," << time[it] << "," << flips[it]
             << endl;
    }
  } else if (output_type == OutputType::ITERATION_INFO) {
    for (auto &pair : iteration_infos) {
      const auto node_count = pair.second.nodeInfo.size();
      for (int node = 0; node < node_count; ++node) {
        const auto &nodeInfo = pair.second.nodeInfo[node];
        stream << pair.first << "," << node << "," << nodeInfo.in_degree << ","
               << nodeInfo.out_degree << "," << nodeInfo.activity << endl;
      }
    }
  } else if (output_type == OutputType::NODES_FLIPPED) {
    for (auto &pair : iteration_infos) {
      stream << pair.first << "," << pair.second.flip_count << endl;
    }
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
  auto cur_time = chrono::high_resolution_clock::now();
  auto time_elapsed =
      chrono::duration_cast<chrono::milliseconds>(cur_time - start_time)
          .count();
  auto time_limit_in_ms = run.time_limit * 60 * 1000;

  for (int iteration = 0;
       iteration <= run.iterations && time_elapsed < time_limit_in_ms;
       ++iteration) {
    algorithm->iteration();
    cur_time = chrono::high_resolution_clock::now();
    time_elapsed =
        chrono::duration_cast<chrono::milliseconds>(cur_time - start_time)
            .count();
    if (record_cut(iteration)) {
      result.time.push_back(time_elapsed);
      result.cut_sizes.push_back(algorithm->getCutSize());
      result.iterations.push_back(iteration);
      result.flips.push_back(algorithm->getNodesFlipped());
    }
    algorithm->clear_stats();
  }

  return result;
}

void batch(const vector<Run> &runs, OutputType output_type) {

  // vector<RunResult> results;
  std::mutex mutex;

  write_header(cout, output_type);

#pragma omp parallel for
  for (auto it = runs.cbegin(); it < runs.cend(); ++it) {
    auto result = execute(*it);
    std::lock_guard<std::mutex> lock(mutex);
    // results.push_back(result);
    write_result_to_stream(result, cout, output_type);
  }

  // return results;
}

} // namespace maxcut
