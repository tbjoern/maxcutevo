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
  Unif(Algorithm::Parameters params);

  void iteration() override;
};

} // namespace maxcut
