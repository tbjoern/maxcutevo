#pragma once
#include "../types.hpp"
#include "Algorithm.hpp"

namespace maxcut {

/**
 * Always chooses the vertex with the highest cut weight gain.
 */
class Greedy : public virtual Algorithm {
public:
  Greedy(const AdjList &adj_list) : Algorithm(adj_list) {}

  void iteration() override;
};

} // namespace maxcut
