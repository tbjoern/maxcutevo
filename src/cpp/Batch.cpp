#include "Batch.hpp"
#include "Runner.hpp"

namespace maxcut {

Batch::Batch(AdjList &&adj_list, std::vector<Algorithm> &algorithms)
    : _adj_list(adj_list), _algorithms(algorithms) {}

std::vector<RunResult> &&Batch::run() const {
  std::vector<RunResult> results;
  for (auto &algorithm : _algorithms) {
    auto result = Runner::runAlgorithm(_adj_list, algorithm);
    results.push_back(result);
  }

  return std::move(results);
}

} // namespace maxcut
