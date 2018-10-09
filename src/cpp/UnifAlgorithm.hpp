#pragma once
#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * Evolutionary Algorithm for MaxCut that chooses a node to flip with even
 * probability distribution
 */
class UnifAlgorithm : public Algorithm {
public:
  void run() override;

  std::string name() override { return "unif"; }
};

} // namespace maxcut
