#pragma once
#include "../types.hpp"
#include "Algorithm.hpp"

namespace maxcut {

/**
 * Evolutionary Algorithm for MaxCut that chooses k nodes to flip.
 * k is chosen from a power law distribution.
 */
class PMUT : public Algorithm {
  std::vector<int> _weights;
  double power_law_beta;

public:
  PMUT(const AdjList &adj_list, double power_law_beta);

  void iteration() override;
};

} // namespace maxcut
