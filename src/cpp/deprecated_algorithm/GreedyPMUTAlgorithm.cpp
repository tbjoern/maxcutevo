#include "GreedyPMUT.hpp"

namespace maxcut {

void GreedyPMUT::run() {
  Greedy::run();
  PMUT::run();
}

} // namespace maxcut
