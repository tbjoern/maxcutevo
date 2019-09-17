#pragma once
#include "../types.hpp"
#include "ActivityAlgorithm.hpp"

namespace maxcut {

class GreedySigmoid : public ActivityAlgorithm {
  double sigmoid_smoothness;
  std::function<bool(int)> activity_sampler;
  int generations;

public:
  GreedySigmoid(Algorithm::Parameters params,
                ActivityAlgorithm::Parameters parameters,
                double sigmoid_smoothness);

  std::function<bool(int)> build_activity_sigmoid_sampler();

  void iteration() override;
};

} // namespace maxcut
