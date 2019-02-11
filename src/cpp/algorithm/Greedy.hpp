#pragma once
#include "../types.hpp"
#include "Algorithm.hpp"

namespace maxcut {

/**
 * Always chooses the vertex with the highest cut weight gain.
 */
class Greedy : public virtual Algorithm {
  bool _improved = false;

public:
  void iteration() override;

  void init() override{};

  std::string name() override { return "greedy"; }
};

} // namespace maxcut
