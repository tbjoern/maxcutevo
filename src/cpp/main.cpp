#include "Batch.hpp"
#include "Graph.hpp"
#include <nlohmann/json.hpp>

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

void write_result_to_stream(const vector<RunResult> &results,
                            std::ostream &stream) {
  stream << "algorithm,run_number,iteration,cut_weight" << endl;
  for (const auto &algorithm_result : results) {
    int algorithm_id = algorithm_result.algorithm_id;
    int run_nr = algorithm_result.run_id;
    const auto &run_data = algorithm_result.cut_sizes;
    for (int iteration = 0; iteration < run_data.size(); ++iteration) {
      stream << algorithm_id << "," << run_nr << "," << iteration << ","
             << run_data[iteration] << endl;
    }
  }
}

} // namespace

RunConfig read_config(string filename) {
  auto cfg_file = ifstream(filename);
  json json_cfg;
  cfg_file >> json_cfg;

  auto config = RunConfig();

  config.max_iterations = json_cfg["iterations"];
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
  const auto adj_list = read_graph(filename);

  vector<Run> runs;
  for (auto &algorithm_config : config.algorithms) {
    for (int run_id = 0; run_id < config.run_count; ++run_id) {
      runs.push_back(
          {algorithm_config, adj_list, run_id++, config.max_iterations});
    }
  }

  auto results = batch(runs);

  write_result_to_stream(results, std::cout);

  // cout << "Random seed is: " << RANDOM_SEED << endl;
  return 0;
}

// vector<shared_ptr<Algorithm>> algorithms;

// for (const auto &algorithmConfig : config.algorithms) {
//   algorithms.push_back(create_algorithm[algorithmConfig.name]());
//   algorithms.back()->id = algorithmConfig.id;
//   algorithms.back()->parse_arguments(algorithmConfig.arguments);
// }
