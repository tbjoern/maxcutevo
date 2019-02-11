#pragma once
#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * Evolutionary Algorithm for MaxCut that flips each node with probabilty
 * a / n, where a is chosen from a power law distribution
 */
class FMUTAlgorithm : public Algorithm {
public:
  void run() override;

  std::string name() override { return "fmut"; }
};

} // namespace maxcut
