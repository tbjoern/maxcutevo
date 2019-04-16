#pragma once
#include "../types.hpp"
#include "Algorithm.hpp"

namespace maxcut {

class Random : public Algorithm {
  BernoulliGenerator gen;

public:
  Random(const AdjList &adj_list);

  void iteration() override;
};

} // namespace maxcut
