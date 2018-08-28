#pragma once
#include <vector>

#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {
class Batch {
public:
  Batch(AdjList &&adj_list, std::vector<Algorithm> &algorithms);

  std::vector<RunResult> &&run() const;

private:
  AdjList _adj_list;
  std::vector<Algorithm> &_algorithms;
};
} // namespace maxcut
