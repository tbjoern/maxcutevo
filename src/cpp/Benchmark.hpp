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
std::vector<AlgorithmResult>
benchmark(std::vector<std::string> &filenames,
          std::vector<std::shared_ptr<Algorithm>> &algorithms,
          const RunConfig config);

} // namespace maxcut
