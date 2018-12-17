#pragma once
#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * super cool algorithm that is the best and will beat all other algorithms ...
 * ;)
 */
class ActivityAlgorithm : public Algorithm {
protected:
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

  void run() override;

  std::string name() override {
    return _reverse ? "reverse activity" : "activity";
  }

  ActivityAlgorithm(bool reverse) : _reverse(reverse ? -1 : 1) {}

  void updateActivity(const std::vector<int> &flipped_nodes);

  void decayActivity();
};

} // namespace maxcut
