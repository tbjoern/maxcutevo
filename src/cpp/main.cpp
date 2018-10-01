#include "Algorithm.hpp"
#include "Benchmark.hpp"

#include "AnnealingAlgorithm.hpp"
#include "PMUTAlgorithm.hpp"
#include "UnifAlgorithm.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace maxcut;
using namespace std;

int main() {
  vector<string> filenames{"mtx/mvccographsbip.n100m30p0.03.1.mtx",
                           "mtx/mvccographsbip.n100m30p1.0.1.mtx",
                           "mtx/ca-citeseer.mtx"};
  vector<unique_ptr<Algorithm>> algorithms;

  algorithms.push_back(make_unique<UnifAlgorithm>());
  // algorithms.push_back(make_unique<AnnealingAlgorithm>());
  algorithms.push_back(make_unique<PMUTAlgorithm>());

  Benchmark benchmark(filenames, algorithms);
  auto results = benchmark.run();
  for (const auto results_for_file : results) {
    for (const auto run : results_for_file) {
      cout << run.first << " " << run.second << endl;
    }
  }
  return 0;
}
