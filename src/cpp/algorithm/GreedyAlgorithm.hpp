#pragma once
#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * Always chooses the vertex with the highest cut weight gain.
 */
class GreedyAlgorithm : public virtual Algorithm {
  bool _improved = false;

public:
  void iteration() override;

  void init() override{};

  std::string name() override { return "greedy"; }
};

} // namespace maxcut
