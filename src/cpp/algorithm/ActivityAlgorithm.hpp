#pragma once
#include "../types.hpp"
#include "Algorithm.hpp"

namespace {
constexpr int REVERSE = -1, NORMAL = 1;
}

namespace maxcut {

/**
 * super cool algorithm that is the best and will beat all other algorithms ...
 * ;)
 */
class ActivityAlgorithm : public Algorithm {
protected:
  std::vector<int> _pop;
  std::vector<double> _activity;

  void initActivity();

public:
  constexpr static int START_ACTIVITY = 100;
  constexpr static int ACT_INC = 1;
  constexpr static int ACT_DEC = 1;
  constexpr static int ACT_MAX = 200;
  constexpr static int ACT_MIN = 1;
  constexpr static double DECAY_RATE = 0.95;

  void iteration() override;

  ActivityAlgorithm(const AdjList &adj_list, double power_law_param);

  void updateActivity(const std::vector<int> &flipped_nodes);

  void decayActivity();
};

} // namespace maxcut
