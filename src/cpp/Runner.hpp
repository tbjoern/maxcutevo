#pragma once
#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * runs an algorithm on a graph
 */
class Runner {
public:
  constexpr static int ALGORITHM_RUN_DURATION = 1; // seconds
  static RunResult runAlgorithm(const AdjList &adj_list, Algorithm &algorithm);
};

} // namespace maxcut
