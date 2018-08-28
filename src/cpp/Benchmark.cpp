#include "Benchmark.hpp"

#include <fstream>

#include "Batch.hpp"

namespace maxcut {

Benchmark::Benchmark(std::vector<std::string> &filename,
                     std::vector<Algorithm> &algorithms)
    : _algorithms(algorithms), _filenames(filename) {}

std::vector<std::vector<RunResult>> &&Benchmark::run() const {
  std::vector<std::vector<RunResult>> results(_filenames.size());
  for (int fileID = 0; fileID < _filenames.size(); ++fileID) {

    std::ifstream input_file(_filenames[fileID]);
    int nodes, edges;
    input_file >> nodes >> edges;
    AdjList adj_list(nodes);

    for (uint i = 0; i < edges; ++i) {
      int source, dest, weight;
      input_file >> source >> dest >> weight;
      adj_list[source].push_back({dest, weight});
    }

    Batch batch(std::move(adj_list), _algorithms);
    results[fileID] = batch.run();
  }
  return std::move(results);
}

} // namespace maxcut
