#include "UnifAlgorithm.hpp"
#include <random>

using namespace std;

namespace maxcut {

int UnifAlgorithm::run(const AdjList &adj_list) {
  const int n = adj_list.size();
  vector<int> side(n, 1), chg(n, 0);
  for (int node = 0; node < n; ++node) {
    for (const auto &edge : adj_list[node]) {
      chg[node] += edge.second;
      chg[edge.first] += edge.second;
    }
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, n - 1);

  int obj = 0, best = 0;
  double iterations = 10000;
  for (double it = 0; it < iterations; it++) {
    int k = dis(gen);
    obj += chg[k];
    side[k] = -side[k];
    chg[k] = -chg[k];
    for (auto e : adj_list[k]) {
      int v = e.first;
      chg[v] += e.second * (2 - 4 * (side[k] != side[v]));
    }
    if (obj > best) {
      best = obj;
    }
  }
  return best;
}

} // namespace maxcut
