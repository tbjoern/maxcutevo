#include "PMUTAlgorithm.hpp"
#include "MathHelper.hpp"
#include <random>

using namespace std;

namespace maxcut {

int PMUTAlgorithm::run(const AdjList &adj_list) {
  const int n = adj_list.size();
  vector<int> side(n, 1), chg(n, 0), pop(n), weights(n, 1);
  for (int node = 0; node < n; ++node) {
    pop[node] = node;
    for (const auto &edge : adj_list[node]) {
      chg[node] += edge.second;
      chg[edge.first] += edge.second;
    }
  }

  auto &helper = MathHelper::getInstance();
  helper.setPowerLawParam(2);

  int obj = 0, best = 0;
  double iterations = 10000;
  for (double it = 0; it < iterations; it++) {
    auto k = helper.getIntFromPowerLawDistribution(n);
    auto nodes_to_flip = helper.chooseKUnique(pop, weights, k);
    auto tmp_chg = chg;
    auto tmp_side = side;
    for (auto node : nodes_to_flip) {
      obj += chg[node];
      side[node] = -side[node];
      chg[node] = -chg[node];
      for (auto e : adj_list[node]) {
        int v = e.first;
        chg[v] += e.second * (2 - 4 * (side[node] != side[v]));
      }
    }
    if (obj > best) {
      best = obj;
      // } else {
      //   // could add annealing here
      //   side = std::move(tmp_side);
      //   chg = std::move(tmp_chg);
    }
  }
  return best;
}

} // namespace maxcut
