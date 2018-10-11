#pragma once
#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * super cool algorithm that is the best and will beat all other algorithms ...
 * ;)
 */
class ActivityAlgorithm : public Algorithm {
  bool _reverse;

public:
  void run() override;

  std::string name() override {
    return _reverse ? "reverse activity" : "activity";
  }

  ActivityAlgorithm(bool reverse) : _reverse(reverse) {}
};

} // namespace maxcut
