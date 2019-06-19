#include "AdjList.hpp"
#include "Batch.hpp"
#include "Graph.hpp"
#include "MathHelper.hpp"
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

RunConfig read_config(string filename) {
  auto cfg_file = ifstream(filename);
  json json_cfg;
  cfg_file >> json_cfg;

  auto config = RunConfig();

  config.max_iterations = json_cfg["iterations"];
  config.run_count = json_cfg["run_count"];
  config.random_start = json_cfg["random_start"];
  maxcut::AdjList::undirected = json_cfg["undirected"];

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

vector<char> make_random_start(int node_count) {
  vector<char> start_assignment(node_count, NOT_CUT_SET);
  MathHelper helper;
  auto random_gen = helper.probabilitySampler(0.5);
  for (int node = 0; node < node_count; ++node) {
    if (random_gen.get()) {
      start_assignment[node] = CUT_SET;
    }
  }
  return start_assignment;
}

int main(int argc, char *argv[]) {
  string filename;
  RunConfig config;
  OutputType output_type;
  string print_mode;
  switch (argc) {
  case 3:
    config = read_config(argv[2]);
    filename = argv[1];
    break;
  case 4:
    config = read_config(argv[2]);
    filename = argv[1];
    print_mode = argv[3];
    break;
  default:
    cout
        << "Usage: maxcut-benchmark <graph_instance> <config_file> <print-mode>"
        << endl;
    exit(1);
  }

  if (print_mode == "info") {
    output_type = OutputType::ITERATION_INFO;
  } else {
    output_type == OutputType::CUT_WEIGHT;
  }

  const auto adj_list = read_graph(filename);

  vector<char> start_assigment;
  if (config.random_start) {
    start_assigment = make_random_start(adj_list.node_count);
  }

  vector<Run> runs;
  for (auto &algorithm_config : config.algorithms) {
    for (int run_id = 0; run_id < config.run_count; ++run_id) {
      runs.push_back({algorithm_config, adj_list, start_assigment,
                      config.random_start, run_id, config.max_iterations});
    }
  }

  try {
    batch(runs, output_type);
  } catch (exception x) {
    cout << x.what() << endl;
  }

  // cout << "Random seed is: " << RANDOM_SEED << endl;
  return 0;
}

// vector<shared_ptr<Algorithm>> algorithms;

// for (const auto &algorithmConfig : config.algorithms) {
//   algorithms.push_back(create_algorithm[algorithmConfig.name]());
//   algorithms.back()->id = algorithmConfig.id;
//   algorithms.back()->parse_arguments(algorithmConfig.arguments);
// }
