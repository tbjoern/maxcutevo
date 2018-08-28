#include "Algorithm.hpp"
#include "Benchmark.hpp"
#include <string>
#include <vector>

using namespace maxcut;

int main() {
  std::vector<std::string> filenames{"test"};
  std::vector<Algorithm> algorithms;

  Benchmark(filenames, algorithms);
  return 0;
}
