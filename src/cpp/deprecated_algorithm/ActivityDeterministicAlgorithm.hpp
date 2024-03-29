#pragma once
#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * super cool algorithm that is the best and will beat all other algorithms ...
 * ;)
 */
class ActivityDeterministicAlgorithm : public Algorithm {
  bool _reverse;

public:
  void run() override;

  std::string name() override {
    return _reverse ? "reverse activity determ." : "activity determ.";
  }

  ActivityDeterministicAlgorithm(bool reverse) : _reverse(reverse) {}
};

} // namespace maxcut
