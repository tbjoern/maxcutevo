#include "Benchmark.hpp"

#include <fstream>
#include <iostream>
#include <limits>

#include "Batch.hpp"

namespace maxcut {

Benchmark::Benchmark(std::vector<std::string> &filename,
                     std::vector<std::unique_ptr<Algorithm>> &algorithms)
    : _algorithms(algorithms), _filenames(filename) {}

std::vector<std::vector<RunResult>> Benchmark::run() const {
  std::vector<std::vector<RunResult>> results(_filenames.size());
  for (int fileID = 0; fileID < _filenames.size(); ++fileID) {

    std::cout << "Reading file " << _filenames[fileID];
    std::cout.flush();

    std::ifstream input_file(_filenames[fileID]);
    input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    int nodes, edges;
    input_file >> nodes >> nodes >> edges;
    ++nodes; // some start at 0, others at 1 ...
    AdjList adj_list(nodes);

    for (uint i = 0; i < edges; ++i) {
      int source, dest, weight;
      input_file >> source >> dest; // TODO weight
      adj_list[source].push_back({dest, 1});
      adj_list[dest].push_back({source, 1});
    }

    std::cout << " ...done" << std::endl;

    std::cout << "Starting batch";
    std::cout.flush();

    Batch batch(std::move(adj_list), _algorithms);
    results[fileID] = batch.run();

    std::cout << "...done" << std::endl;
  }
  return results;
}

} // namespace maxcut
