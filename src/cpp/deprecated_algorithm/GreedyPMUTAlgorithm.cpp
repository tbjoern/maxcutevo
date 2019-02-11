#include "GreedyPMUTAlgorithm.hpp"

namespace maxcut {

void GreedyPMUTAlgorithm::run() {
  GreedyAlgorithm::run();
  PMUTAlgorithm::run();
}

} // namespace maxcut
