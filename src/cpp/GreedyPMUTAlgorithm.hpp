#pragma once
#include "GreedyAlgorithm.hpp"
#include "PMUTAlgorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * Always chooses the vertex with the highest cut weight gain.
 */
class GreedyPMUTAlgorithm : public GreedyAlgorithm, public PMUTAlgorithm {
public:
  void run() override;

  std::string name() override { return "greedy pmut"; }
};

} // namespace maxcut
