#include "AdjList.hpp"
#include <exception>
#include <sstream>

namespace maxcut {

bool AdjList::undirected;

void AdjList::add_edge(int source, int dest, int weight) {
  if (edge_exists(source, dest)) {
    auto &edge = get_edge(source, dest);
    if (edge.weight != weight) {
      edge.weight = weight;
    }
    return;
  }
  out_edges[source].push_back({dest, weight});
  in_edges[dest].push_back({source, weight});
  if (undirected && !edge_exists(dest, source)) {
    out_edges[dest].push_back({source, weight});
    in_edges[source].push_back({dest, weight});
  }
}

bool AdjList::edge_exists(int source, int dest) {
  for (const auto &edge : out_edges[source]) {
    if (edge.neighbour == dest) {
      return true;
    }
  }
  return false;
}

Edge &AdjList::get_edge(int source, int dest) {
  for (auto &edge : out_edges[source]) {
    if (edge.neighbour == dest) {
      return edge;
    }
  }
  std::stringstream message;
  message << "Edge does not exist: " << source << "-" << dest;
  throw std::invalid_argument(message.str());
}

} // namespace maxcut
