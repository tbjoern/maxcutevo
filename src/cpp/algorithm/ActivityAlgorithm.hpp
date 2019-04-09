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
  int _reverse;
  std::vector<double> _activity;

  void _init() override;

  void initActivity();

public:
  constexpr static int START_ACTIVITY = 10;
  constexpr static int ACT_INC = 5;
  constexpr static int ACT_DEC = 1;
  constexpr static int ACT_MAX = 100;
  constexpr static int ACT_MIN = 1;
  constexpr static double DECAY_RATE = 0.95;

  void iteration() override;

  void init() override;

  ActivityAlgorithm() : _reverse(NORMAL) {}

  ActivityAlgorithm(bool reverse) : _reverse(reverse ? REVERSE : NORMAL) {}

  void updateActivity(const std::vector<int> &flipped_nodes);

  void decayActivity();
};

} // namespace maxcut
