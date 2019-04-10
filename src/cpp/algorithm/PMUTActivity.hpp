#pragma once
#include "../types.hpp"
#include "ActivityAlgorithm.hpp"

namespace maxcut {

/**
 * Evolutionary Algorithm for MaxCut that chooses a node to flip with even
 * probability distribution
 */
class PMUTActivity : public ActivityAlgorithm {
public:
  PMUTActivity(const AdjList &adj_list, double power_law_param);

  void iteration() override;
};

} // namespace maxcut
