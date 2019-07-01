#pragma once
#include "../types.hpp"
#include "ActivityAlgorithm.hpp"

namespace maxcut {

/**
 * Evolutionary Algorithm for MaxCut that chooses a node to flip with even
 * probability distribution
 */
class PMUTActivity : public ActivityAlgorithm {
  PowerLawGenerator plaw_gen;

public:
  PMUTActivity(Algorithm::Parameters params,
               ActivityAlgorithm::Parameters parameters,
               double power_law_param);

  void iteration() override;
};

} // namespace maxcut
