#pragma once
#include "algorithm/Algorithm.hpp"
#include "types.hpp"
#include <memory>
#include <string>
#include <vector>

namespace maxcut {

/**
 *  manages graph file I/O
 */
std::vector<AlgorithmResult>
benchmark(std::string &filename,
          std::vector<std::shared_ptr<Algorithm>> &algorithms,
          const RunConfig config);

} // namespace maxcut
