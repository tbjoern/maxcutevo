#pragma once
#include "../types.hpp"
#include "Algorithm.hpp"
#include <memory>

namespace maxcut {

class Random : public Algorithm {
  std::shared_ptr<BernoulliGenerator> gen = nullptr;

public:
  Random(Algorithm::Parameters params);

  void iteration() override;
};

} // namespace maxcut
