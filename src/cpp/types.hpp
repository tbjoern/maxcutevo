#pragma once
#include <string>
#include <utility>
#include <vector>

namespace maxcut {

typedef std::pair<int, int> Edge;
typedef std::vector<std::vector<Edge>> AdjList;

struct RunResult {
  std::string algorithmName;
  int cutSize;
  double time;
};

} // namespace maxcut
