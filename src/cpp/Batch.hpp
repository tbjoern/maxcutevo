#pragma once
#include <memory>
#include <vector>

#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 *  runs a set of algorithms on a graph
 */
std::vector<RunResult> batch(AdjList &adj_list, std::vector<std::shared_ptr<Algorithm>> &algorithms, const int max_time, const int max_iterations);

} // namespace maxcut
