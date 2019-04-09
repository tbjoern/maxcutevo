#pragma once
#include "../types.hpp"
#include "Algorithm.hpp"

namespace maxcut {

/**
 * Evolutionary Algorithm for MaxCut that chooses k nodes to flip.
 * k is chosen from a power law distribution.
 */
class PMUT : public virtual Algorithm {
  std::vector<int> _pop, _weights;
  double power_law_beta;

public:
  void parse_arguments(nlohmann::json arguments) override;

  void iteration() override;

  void init() override;
};

} // namespace maxcut
