#pragma once
#include "../types.hpp"
#include "Algorithm.hpp"

namespace maxcut {

/**
 * Evolutionary Algorithm for MaxCut that chooses a node to flip with even
 * probability distribution
 */
class Unif : public Algorithm {
  double _probability;

public:
  void iteration() override;

  void init() override;
};

} // namespace maxcut
