#pragma once
#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * Evolutionary Algorithm for MaxCut that chooses a node to flip with even probability distribution
 */
class UnifAlgorithm : public Algorithm {
public:
  int run(const AdjList &) override;
};

} // namespace maxcut
