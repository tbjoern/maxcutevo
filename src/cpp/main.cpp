#include "Algorithm.hpp"
#include "Benchmark.hpp"

#include "ActivityAlgorithm.hpp"
#include "AnnealingAlgorithm.hpp"
#include "FMUTAlgorithm.hpp"
#include "PMUTAlgorithm.hpp"
#include "UnifAlgorithm.hpp"

#include <algorithm>
//#include <experimental/filesystem>
#include <filesystem>
#include <iostream>
#include <memory>
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
  std::filesystem::directory_iterator start(p);
  std::filesystem::directory_iterator end;
  std::transform(start, end, std::back_inserter(v), path_leaf_string());
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

  vector<shared_ptr<Algorithm>> algorithms;

  algorithms.push_back(make_shared<UnifAlgorithm>());
  algorithms.push_back(make_shared<AnnealingAlgorithm>());
  algorithms.push_back(make_shared<PMUTAlgorithm>());
  algorithms.push_back(make_shared<FMUTAlgorithm>());
  algorithms.push_back(make_shared<ActivityAlgorithm>());

  auto filenames = read_directory(dirname);
  sort(filenames.begin(), filenames.end(),
       [](const auto &f1, const auto &f2) { return f1 < f2; });
  Benchmark benchmark(filenames, algorithms);
  auto results = benchmark.run();

  for_each(results.begin(), results.end(), [](auto &v) {
    sort(v.begin(), v.end(), [](const auto &r1, const auto &r2) {
      return r1.algorithmName < r2.algorithmName;
    });
  });

  for (int i = 0; i < results.size(); ++i) {
    const auto &results_for_file = results[i];
    cout << filenames[i] << endl;
    for (const auto run : results_for_file) {
      cout << run.algorithmName << ": " << run.cutSize << " in " << run.time
           << "ms" << endl;
    }
    cout << endl;
  }
  return 0;
}
