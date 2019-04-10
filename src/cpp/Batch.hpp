#pragma once
#include <memory>
#include <vector>

#include "algorithm/Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 *  runs a set of algorithms on a graph
 *  handles parralel processing
 */
std::vector<RunResult> batch(const std::vector<Run> &runs);

} // namespace maxcut
