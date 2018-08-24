#include <vector>

#include "types.h"
#include "AlgorithmFactory.h"

namespace maxcut {
class Batch {
    public:
    Batch(AdjList adj_list, std::vector<AlgorithmFactory> algorithm_factories);

    std::vector<RunResult> run() const;
    private:
    AdjList& _adj_list;
    std::vector<AlgorithmFactory>& _algorithm_factories;
};
}
