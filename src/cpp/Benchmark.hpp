#pragma once
#include "Algorithm.hpp"
#include "types.hpp"
#include <string>
#include <vector>

namespace maxcut {

class Benchmark {
public:
  Benchmark(std::vector<std::string> &filenames,
            std::vector<Algorithm> &algorithms);

  std::vector<std::vector<RunResult>> &&run() const;

private:
  std::vector<Algorithm> &_algorithms;
  const std::vector<std::string> _filenames;
};

} // namespace maxcut
