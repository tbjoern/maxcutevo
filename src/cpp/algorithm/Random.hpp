#pragma once
#include "../types.hpp"
#include "Algorithm.hpp"

namespace maxcut {

class Random : public Algorithm {
  BernoulliGenerator gen;

public:
  Random(Algorithm::Parameters params);

  void iteration() override;
};

} // namespace maxcut
