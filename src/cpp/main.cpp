#include "Algorithm.hpp"
#include "Benchmark.hpp"

// #include "ActivityAlgorithm.hpp"
// #include "ActivityDeterministicAlgorithm.hpp"
// #include "AnnealingAlgorithm.hpp"
// #include "FMUTAlgorithm.hpp"
// #include "GreedyActivityAlgorithm.hpp"
// #include "GreedyAlgorithm.hpp"
// #include "GreedyPMUTAlgorithm.hpp"
// #include "PMUTAlgorithm.hpp"
// #include "UnifActivityAlgorithm.hpp"
#include "UnifAlgorithm.hpp"

#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
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

} // namespace

int main(int argc, char *argv[]) {
  string dirname;
  switch (argc) {
  case 1:
    dirname = R"(/home/tbjoern/development/maxcut/maxcutevo/build/graphs)";
    break;
  case 2:
    if (std::filesystem::exists(argv[1])) {
      dirname = argv[1];
    } else {
      cout << "Could not find directory " << argv[1] << endl;
      exit(127);
    }
    break;
  default:
    cout << "Wrong number of arguments. Usage: maxcut-benchmark <graph "
            "directory>"
         << endl;
    exit(1);
  }

  RANDOM_SEED = std::random_device{}();

  vector<shared_ptr<Algorithm>> algorithms;

  algorithms.push_back(make_shared<UnifAlgorithm>());
  // algorithms.push_back(make_shared<AnnealingAlgorithm>());
  // algorithms.push_back(make_shared<PMUTAlgorithm>());
  // algorithms.push_back(make_shared<FMUTAlgorithm>());
  // algorithms.push_back(make_shared<ActivityAlgorithm>(false));
  // algorithms.push_back(make_shared<ActivityAlgorithm>(true));
  // algorithms.push_back(make_shared<UnifActivityAlgorithm>(false));
  // algorithms.push_back(make_shared<UnifActivityAlgorithm>(true));
  // algorithms.push_back(make_shared<GreedyAlgorithm>());
  // algorithms.push_back(make_shared<GreedyActivityAlgorithm>(false));
  // algorithms.push_back(make_shared<GreedyActivityAlgorithm>(true));
  // algorithms.push_back(make_shared<ActivityDeterministicAlgorithm>(false));
  // algorithms.push_back(make_shared<ActivityDeterministicAlgorithm>(true));
  // algorithms.push_back(make_shared<GreedyPMUTAlgorithm>());

  auto filenames = read_directory(dirname);
  sort(filenames.begin(), filenames.end());

  auto results = benchmark(filenames, algorithms);

  for_each(results.begin(), results.end(), [](auto &v) {
    sort(v.begin(), v.end(), [](const auto &r1, const auto &r2) {
      return r1.algorithmName < r2.algorithmName;
    });
  });

  std::vector<std::vector<int>> output_per_algorithm(algorithms.size());

  for (int i = 0; i < results.size(); ++i) {
    const auto &results_for_file = results[i];
    cout << filenames[i] << endl;
    for (const auto run : results_for_file) {
      cout << setw(25) << run.algorithmName << ": " << setw(7)
           << run.cut.max_size << setw(7) << run.cut.size << "|" << setw(7)
           << run.cut.inverse_size << setw(9) << run.time << "ms " << setw(10)
           << run.evaluation_count << endl;
    }
    for (int j = 0; j < results_for_file.size(); ++j) {
      output_per_algorithm[j].push_back(results_for_file[j].cut_sizes.back());
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

  // cout << "Random seed is: " << RANDOM_SEED << endl;
  return 0;
}
