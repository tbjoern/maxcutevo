#pragma once
#include "algorithm/Algorithm.hpp"
#include "types.hpp"
#include <memory>
#include <string>
#include <vector>

namespace maxcut {

/**
 *  manages graph file I/O
 */
AdjList
read_graph(std::string filename);

} // namespace maxcut
