#pragma once
#include "Algorithm.hpp"
#include "types.hpp"

namespace maxcut {

class UnifAlgorithm : public Algorithm {
public:
  int run(const AdjList &) override;
};

} // namespace maxcut
