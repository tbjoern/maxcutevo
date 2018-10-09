#pragma once
#include "Algorithm.hpp"
#include "types.hpp"
#include <memory>
#include <string>
#include <vector>

namespace maxcut {

/**
 *  runs a set of algorithms on a set of graphs
 */
class Benchmark {
public:
  Benchmark(std::vector<std::string> &filenames,
            std::vector<std::shared_ptr<Algorithm>> &algorithms);

  std::vector<std::vector<RunResult>> run() const;

private:
  std::vector<std::shared_ptr<Algorithm>> &_algorithms;
  const std::vector<std::string> _filenames;
};

} // namespace maxcut
