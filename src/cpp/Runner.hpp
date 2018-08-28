#pragma once
#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

class Runner {
public:
  static RunResult runAlgorithm(const AdjList &adj_list, Algorithm &algorithm);
};

} // namespace maxcut
