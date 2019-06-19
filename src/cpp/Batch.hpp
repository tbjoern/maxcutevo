#pragma once
#include <memory>
#include <vector>

#include "algorithm/Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

enum class OutputType { CUT_WEIGHT, ITERATION_INFO, NODES_FLIPPED };

/**
 *  runs a set of algorithms on a graph
 *  handles parallel processing
 */
void batch(const std::vector<Run> &runs, OutputType output_type);

} // namespace maxcut
