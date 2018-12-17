#pragma once
#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * Evolutionary Algorithm for MaxCut that chooses k nodes to flip.
 * k is chosen from a power law distribution.
 */
class PMUTAlgorithm : public virtual Algorithm {
  std::vector<int> _pop, _weights;

public:
  void iteration() override;

  void init() override;

  std::string name() override { return "pmut"; }
};

} // namespace maxcut
