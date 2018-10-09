#include "Runner.hpp"

#include <chrono>

namespace maxcut {

RunResult Runner::runAlgorithm(const AdjList &adj_list, Algorithm &algorithm) {
  auto start_time = std::chrono::high_resolution_clock::now();

  int cut_size = algorithm.calcSolution(adj_list);

  auto stop_time = std::chrono::high_resolution_clock::now();
  double total_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                          stop_time - start_time)
                          .count();

  return {algorithm.name(), cut_size, total_time};
}

} // namespace maxcut
