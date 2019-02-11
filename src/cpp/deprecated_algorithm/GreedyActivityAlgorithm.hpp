#pragma once
#include "ActivityAlgorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * Uses Greedy as a baseline
 * After Greedy is finished, choose vertex with highest activity
 */
class GreedyActivityAlgorithm : public ActivityAlgorithm {
  bool greedyIteration();

  bool activityIteration();

public:
  void run() override;

  GreedyActivityAlgorithm(bool reverse) : ActivityAlgorithm(reverse) {}

  std::string name() override {
    return _reverse == -1 ? "greedy activity rev" : "greedy activity";
  }
};

} // namespace maxcut
