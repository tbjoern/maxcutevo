#include "Algorithm.hpp"
#include "Benchmark.hpp"

#include "AnnealingAlgorithm.hpp"
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

int main() {
  string dirname = R"(/home/tbjoern/development/maxcut/maxcutevo/build/graphs)";
  vector<unique_ptr<Algorithm>> algorithms;

  algorithms.push_back(make_unique<UnifAlgorithm>());
  // algorithms.push_back(make_unique<AnnealingAlgorithm>());
  algorithms.push_back(make_unique<PMUTAlgorithm>());

  auto filenames = read_directory(dirname);

  Benchmark benchmark(filenames, algorithms);
  auto results = benchmark.run();
  for (const auto results_for_file : results) {
    for (const auto run : results_for_file) {
      cout << run.first << " " << run.second << endl;
    }
  }
  return 0;
}
