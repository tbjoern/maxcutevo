#include "Benchmark.hpp"
#include "algorithm/Algorithm.hpp"
#include <nlohmann/json.hpp>

#include "algorithm/ActivityAlgorithm.hpp"
#include "algorithm/Greedy.hpp"
#include "algorithm/PMUT.hpp"
#include "algorithm/Unif.hpp"

#include <algorithm>
#include <fstream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace maxcut;
using namespace std;

using json = nlohmann::json;

namespace {

void write_result_to_stream(const vector<AlgorithmResult> &results,
                            std::ostream &stream) {
  stream << "algorithm,run_number,iteration,cut_weight" << endl;
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
    {"unif", &make_algorithm<Unif>},
    {"pmut", &make_algorithm<PMUT>},
    {"activity", &make_algorithm<ActivityAlgorithm>},
    {"greedy", &make_algorithm<Greedy>}};

RunConfig read_config(string filename) {
  auto cfg_file = ifstream(filename);
  json json_cfg;
  cfg_file >> json_cfg;

  auto config = RunConfig();

  config.max_duration = json_cfg["max_duration"];
  config.max_iterations = json_cfg["max_iterations"];
  config.run_count = json_cfg["run_count"];

  for (const auto &algorithm : json_cfg["algorithms"]) {
    AlgorithmConfig cfg;
    cfg.name = algorithm["name"];
    cfg.id = algorithm["id"];
    if (algorithm.find("arguments") != algorithm.end()) {
      cfg.arguments = algorithm["arguments"];
    } else {
      cfg.arguments = json::object();
    }

    config.algorithms.push_back(cfg);
  }

  return config;
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

  for (const auto &algorithmConfig : config.algorithms) {
    algorithms.push_back(create_algorithm[algorithmConfig.name]());
    algorithms.back()->id = algorithmConfig.id;
    algorithms.back()->parse_arguments(algorithmConfig.arguments);
  }

  auto results = benchmark(filename, algorithms, config);

  write_result_to_stream(results, std::cout);

  // cout << "Random seed is: " << RANDOM_SEED << endl;
  return 0;
}
