#pragma once
#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * Evolutionary Algorithm for MaxCut that chooses k nodes to flip.
 * k is chosen from a power law distribution.
 */
class PMUTAlgorithm : public Algorithm {
public:
  int run(const AdjList &) override;
};

} // namespace maxcut
