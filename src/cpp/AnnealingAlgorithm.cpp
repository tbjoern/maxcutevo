#include <random>
#include "AnnealingAlgorithm.hpp"
#include "types.hpp"

using namespace std;

namespace maxcut {

int AnnealingAlgorithm::run(const AdjList & adj_list) {
    const int n = adj_list.size();
    vector<int> side(n, 1), chg(n, 0);
    for(int node = 0; node < n; ++node) {
        for(const auto& edge : adj_list[node]) {
            chg[node] += edge.second;
            chg[edge.first] += edge.second;
        }
    }
    int obj = 0, best = 0;
    double heatmax = 10000;
    for (double heat = 0; heat < heatmax; heat += 2e-6) {
        int k = rand() % n;
        if (!(drand48() > exp(heat*chg[k]/best))) {
            obj += chg[k]; side[k] = -side[k]; chg[k] = -chg[k];
            for (auto e : adj_list[k]) {
            int v = e.first;
            chg[v] += e.second * (2 - 4 * (side[k] != side[v]));
        }
            if (obj > best) {
                best = obj;
            }
        }
    }
    return best;
}

}
