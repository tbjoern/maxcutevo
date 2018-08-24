#include <vector>
#include <string>
#include "AlgorithmFactory.h"
#include "types.h"

namespace maxcut {

class Benchmark {
  public:
    Benchmark(std::string _filename, std::vector<AlgorithmFactory>& _algorithm_factories);

    std::vector<RunResult> run(std::string filename) const;

  private:
    std::vector<AlgorithmFactory>& algorithm_factories;
    AdjList adj_list;
};

}
