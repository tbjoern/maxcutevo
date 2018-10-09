#pragma once
#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * Algorithm from "Solving maximum cut problems by simulated annealing" by TOR
 * MYKLEBUST
 */
class AnnealingAlgorithm : public Algorithm {
  void run() override;
};

} // namespace maxcut
