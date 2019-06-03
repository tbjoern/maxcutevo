#pragma once
#include "../types.hpp"
#include "ActivityAlgorithm.hpp"

namespace maxcut {

class GreedyActivity : public ActivityAlgorithm {
public:
  GreedyActivity(Algorithm::Parameters params,
                 ActivityAlgorithm::Parameters parameters);

  void iteration() override;
};

} // namespace maxcut
