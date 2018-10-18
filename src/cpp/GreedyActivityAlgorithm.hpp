#pragma once
#include "ActivityAlgorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * Uses GreedyAlgorithm as a baseline
 * After GreedyAlgorithm is finished, choose vertex with highest activity
 */
class GreedyActivityAlgorithm : public ActivityAlgorithm {
  bool greedyIteration();

  bool activityIteration();

public:
  void run() override;

  GreedyActivityAlgorithm(bool reverse) : ActivityAlgorithm(reverse) {}

  std::string name() override { return "greedy activity"; }
};

} // namespace maxcut
