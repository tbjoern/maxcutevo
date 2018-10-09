#include "UnifAlgorithm.hpp"
#include <random>

using namespace std;

namespace maxcut {

void UnifAlgorithm::run() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, _node_count - 1);

  double iterations = 10000;
  for (double it = 0; it < iterations; it++) {
    int k = dis(gen);
    flipNodesIfBetterCut({k});
  }
}
} // namespace maxcut
