#pragma once
#include "types.hpp"

namespace maxcut {

class Algorithm {
public:
  // returns solution size
  virtual int run(const AdjList &) = 0;
};

} // namespace maxcut
