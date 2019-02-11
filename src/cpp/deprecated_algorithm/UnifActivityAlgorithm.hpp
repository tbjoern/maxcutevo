#pragma once
#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * Evolutionary Algorithm for MaxCut that chooses a node to flip with even
 * probability distribution
 */
class UnifActivityAlgorithm : public Algorithm {
  bool _reverse;

public:
  void run() override;

  UnifActivityAlgorithm(bool reverse) : _reverse(reverse) {}

  std::string name() override {
    return _reverse ? "unif_activity_reverse" : "unif_activity";
  }
};

} // namespace maxcut
