#pragma once
#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * Always chooses the vertex with the highest cut weight gain.
 */
class GreedyAlgorithm : public Algorithm {
public:
  void run() override;

  std::string name() override { return "greedy"; }
};

} // namespace maxcut
