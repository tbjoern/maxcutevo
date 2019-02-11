#include "Algorithm.hpp"
#include "Benchmark.hpp"

#include "ActivityAlgorithm.hpp"
// #include "ActivityDeterministicAlgorithm.hpp"
// #include "AnnealingAlgorithm.hpp"
// #include "FMUTAlgorithm.hpp"
// #include "GreedyActivityAlgorithm.hpp"
#include "GreedyAlgorithm.hpp"
// #include "GreedyPMUTAlgorithm.hpp"
#include "PMUTAlgorithm.hpp"
// #include "UnifActivityAlgorithm.hpp"
#include "UnifAlgorithm.hpp"

#include <algorithm>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace maxcut;
using namespace std;

namespace {

void write_result_to_stream(const vector<AlgorithmResult> &results,
                            const RunConfig config, std::ostream &stream) {
  for (const auto &algorithm_result : results) {
    int algorithm_id = algorithm_result.algorithm_id;

    for (int run_nr = 0; run_nr < algorithm_result.run_results.size();
         ++run_nr) {
      const auto &run_data = algorithm_result.run_results[run_nr];

      for (int iteration = 0; iteration < run_data.cut_sizes.size();
           ++iteration) {
        stream << algorithm_id << "," << run_nr << "," << iteration << ","
               << run_data.cut_sizes[iteration] << endl;
      }
    }
  }
}

} // namespace

template <typename T> shared_ptr<Algorithm> make_algorithm() {
  return make_shared<T>();
}

unordered_map<string, shared_ptr<Algorithm> (*)()> create_algorithm = {
    {"unif", &make_algorithm<UnifAlgorithm>},
    {"pmut", &make_algorithm<PMUTAlgorithm>},
    {"activity", &make_algorithm<ActivityAlgorithm>},
    {"greedy", &make_algorithm<GreedyAlgorithm>}};

RunConfig read_config(string filename) { 
    // TODO: parse config file in ini format
    // sections: run information and algorithm
    // run information section has max_duration, max_iterations and run_count
    // algorithm sections can be repeated, each has
    //      name, unique id, parameters
    return RunConfig(); 
}

int main(int argc, char *argv[]) {
  string filename;
  RunConfig config;
  switch (argc) {
  case 3:
    config = read_config(argv[2]);
    filename = argv[1];
    break;
  default:
    cout << "Usage: maxcut-benchmark <graph_instance> <config_file>" << endl;
    exit(1);
  }

  RANDOM_SEED = std::random_device{}();

  vector<shared_ptr<Algorithm>> algorithms;

  for (const auto &p : config.algorithms) {
    algorithms.push_back(create_algorithm[p.first]());
    algorithms.back()->id = p.second;
  }

  auto results = benchmark(filename, algorithms, config);

  write_result_to_stream(results, config, std::cout);

  // cout << "Random seed is: " << RANDOM_SEED << endl;
  return 0;
}
