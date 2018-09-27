#pragma once
#include <memory>
#include <vector>

#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

/**
 *  runs a set of algorithms on a graph
 */
class Batch {
public:
  Batch(AdjList &&adj_list,
        std::vector<std::unique_ptr<Algorithm>> &algorithms);

  std::vector<RunResult> run() const;

private:
  AdjList _adj_list;
  std::vector<std::unique_ptr<Algorithm>> &_algorithms;
};
} // namespace maxcut
