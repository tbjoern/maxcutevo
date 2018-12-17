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
std::vector<std::vector<RunResult>> benchmark(std::vector<std::string> &filenames,
                     std::vector<std::shared_ptr<Algorithm>> &algorithms,
                     const int max_time=1, const int max_iterations=1000);

} // namespace maxcut
