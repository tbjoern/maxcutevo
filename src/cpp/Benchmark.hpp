#pragma once
#include "Algorithm.hpp"
#include "types.hpp"
#include <memory>
#include <string>
#include <vector>

namespace maxcut {

class Benchmark {
public:
  Benchmark(std::vector<std::string> &filenames,
            std::vector<std::unique_ptr<Algorithm>> &algorithms);

  std::vector<std::vector<RunResult>> run() const;

private:
  std::vector<std::unique_ptr<Algorithm>> &_algorithms;
  const std::vector<std::string> _filenames;
};

} // namespace maxcut
