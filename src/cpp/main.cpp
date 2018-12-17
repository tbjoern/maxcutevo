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
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace maxcut;
using namespace std;

namespace {

struct path_leaf_string {
  std::string operator()(const std::filesystem::directory_entry &entry) const {
    return entry.path().string();
  }
};

vector<string> read_directory(const std::string &name) {
  vector<string> v;
  std::filesystem::path p(name);
  if (!filesystem::exists(p)) {
    throw invalid_argument(name + string(" not found"));
  }
  std::filesystem::recursive_directory_iterator dir(p);
  for (const auto &entry : dir) {
    if (!filesystem::is_directory(entry)) {
      v.push_back(entry.path());
    }
  }

  return v;
}

string stem(string filename) { return filesystem::path(filename).stem(); }

void write_csv(string filename, std::vector<AlgorithmResult> algo_results,
               unordered_map<string, int> algo_id_map) {
  ofstream csv_file(filename);
  if (!csv_file.good()) {
    throw invalid_argument("file cannot be written: " + filename);
  }
  csv_file << "algorithm,run_number,iteration,cut_weight" << endl;

  for (const auto &algorithm_result : algo_results) {
    auto algorithm_id = algo_id_map[algorithm_result.algorithmName];

    for (int run_nr = 0; run_nr < algorithm_result.run_results.size();
         ++run_nr) {
      const auto &run_data = algorithm_result.run_results[run_nr];

      for (int iteration = 0; iteration < run_data.cut_sizes.size();
           ++iteration) {
        csv_file << algorithm_id << "," << run_nr << "," << iteration << ","
                 << run_data.cut_sizes[iteration] << endl;
      }
    }
  }
}

void write_id_map(string filename, unordered_map<string, int> id_map) {
  ofstream csv_file(filename);
  if (!csv_file.good()) {
    throw invalid_argument("file cannot be written: " + filename);
  }
  csv_file << "id,name" << endl;
  for (const auto &pair : id_map) {
    string name;
    int id;
    std::tie(name, id) = pair;
    csv_file << id << "," << name << endl;
  }
}

} // namespace

int main(int argc, char *argv[]) {
  string dirname;
  RunConfig config;
  string csv_dir = R"(./csv/)";
  switch (argc) {
  case 6:
    config.run_count = std::stoi(argv[5]);
    [[fallthrough]];
  case 5:
    config.max_duration = std::stoi(argv[4]);
    [[fallthrough]];
  case 4:
    config.max_iterations = std::stoi(argv[3]);
    [[fallthrough]];
  case 3:
    if (std::filesystem::exists(argv[2])) {
      csv_dir = argv[2];
      if (csv_dir.back() != '/') {
        csv_dir += '/';
      }
    } else {
      cout << "Could not find directory " << argv[2] << endl;
      exit(1);
    }
    [[fallthrough]];
  case 2:
    if (std::filesystem::exists(argv[1])) {
      dirname = argv[1];
    } else {
      cout << "Could not find directory " << argv[1] << endl;
      exit(1);
    }
    break;
  default:
    cout << "Usage: maxcut-benchmark <graph directory> <csv_directory> <max "
            "iterations> "
            "<max_duration> <run_count>"
         << endl;
    exit(1);
  }

  RANDOM_SEED = std::random_device{}();

  vector<shared_ptr<Algorithm>> algorithms;

  algorithms.push_back(make_shared<UnifAlgorithm>());
  // algorithms.push_back(make_shared<AnnealingAlgorithm>());
  algorithms.push_back(make_shared<PMUTAlgorithm>());
  // algorithms.push_back(make_shared<FMUTAlgorithm>());
  algorithms.push_back(make_shared<ActivityAlgorithm>(false));
  algorithms.push_back(make_shared<ActivityAlgorithm>(true));
  // algorithms.push_back(make_shared<UnifActivityAlgorithm>(false));
  // algorithms.push_back(make_shared<UnifActivityAlgorithm>(true));
  algorithms.push_back(make_shared<GreedyAlgorithm>());
  // algorithms.push_back(make_shared<GreedyActivityAlgorithm>(false));
  // algorithms.push_back(make_shared<GreedyActivityAlgorithm>(true));
  // algorithms.push_back(make_shared<ActivityDeterministicAlgorithm>(false));
  // algorithms.push_back(make_shared<ActivityDeterministicAlgorithm>(true));
  // algorithms.push_back(make_shared<GreedyPMUTAlgorithm>());

  auto filenames = read_directory(dirname);
  sort(filenames.begin(), filenames.end());

  auto results = benchmark(filenames, algorithms, config);

  for_each(results.begin(), results.end(), [](auto &v) {
    sort(v.begin(), v.end(), [](const auto &r1, const auto &r2) {
      return r1.algorithmName < r2.algorithmName;
    });
  });

  std::vector<std::vector<int>> output_per_algorithm(algorithms.size());

  for (int i = 0; i < results.size(); ++i) {
    const auto &results_for_file = results[i];
    cout << filenames[i] << endl;
    for (const auto algo_result : results_for_file) {
      const auto &run = algo_result.run_results[0];
      cout << setw(25) << algo_result.algorithmName << ": " << setw(7)
           << run.cut.max_size << setw(7) << run.cut.size << "|" << setw(7)
           << run.cut.inverse_size << setw(9) << run.time << "ms " << setw(10)
           << run.evaluation_count << endl;
    }
    for (int j = 0; j < results_for_file.size(); ++j) {
      // TODO: take the maximum of all runs
      output_per_algorithm[j].push_back(
          results_for_file[j].run_results[0].cut.max_size);
    }
    cout << endl;
  }

  for (int i = 0; i < output_per_algorithm.size(); ++i) {
    cout << results[0][i].algorithmName;
    for (auto number : output_per_algorithm[i]) {
      cout << "," << number;
    }
    cout << endl;
  }

  unordered_map<string, int> algo_id_map;
  for (int i = 0; i < algorithms.size(); ++i) {
    algo_id_map.insert({algorithms[i]->name(), i});
  }

  for (int file = 0; file < filenames.size(); ++file) {
    write_csv(csv_dir + stem(filenames[file]) + string(".csv"), results[file],
              algo_id_map);
    write_id_map(csv_dir + stem(filenames[file]) + string("_mapping.csv"),
                 algo_id_map);
  }

  // cout << "Random seed is: " << RANDOM_SEED << endl;
  return 0;
}
