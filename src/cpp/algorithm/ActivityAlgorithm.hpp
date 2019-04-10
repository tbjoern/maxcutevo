#pragma once
#include "../types.hpp"
#include "Algorithm.hpp"
#include <functional>

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
  std::vector<double> _activity;

  void initActivity();

public:
  struct Parameters {
    int START_ACTIVITY = 100;
    int ACT_INC = 1;
    int ACT_DEC = 1;
    int ACT_MAX = 200;
    int ACT_MIN = 1;
    double DECAY_RATE = 0.95;
  };

  Parameters parameters;

  ActivityAlgorithm(const AdjList &adj_list, Parameters parameters);

  void updateActivity(const std::vector<int> &flipped_nodes);

  void decayActivity();
};

} // namespace maxcut
