#pragma once
#include "Greedy.hpp"
#include "PMUT.hpp"
#include "types.hpp"

namespace maxcut {

/**
 * Always chooses the vertex with the highest cut weight gain.
 */
class GreedyPMUT : public Greedy, public PMUT {
public:
  void run() override;

  std::string name() override { return "greedy pmut"; }
};

} // namespace maxcut
