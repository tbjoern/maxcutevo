#pragma once
#include "../types.hpp"
#include "Algorithm.hpp"

namespace maxcut {

/**
 * Always chooses the vertex with the highest cut weight gain.
 */
class Greedy : public virtual Algorithm {
public:
  Greedy(Algorithm::Parameters params) : Algorithm(params) {}

  void iteration() override;
};

} // namespace maxcut
