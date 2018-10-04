#include "Benchmark.hpp"

#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>

#include "Batch.hpp"

namespace maxcut {

enum FileExtension { EDGELIST, MTXREADER };

static std::map<std::string, FileExtension> filename_map = {{".mtx", MTXREADER},
                                                            {".rud", EDGELIST}};

class FileReader {
public:
  virtual ~FileReader(){};
  virtual AdjList readFile(std::string filename) = 0;
};

class EdgeListReader : public FileReader {
public:
  ~EdgeListReader(){};
  EdgeListReader() {}

  AdjList readFile(std::string filename) override {
    std::ifstream input_file(filename);
    int nodes, edges;
    input_file >> nodes >> nodes >> edges;
    AdjList adj_list(nodes);

    for (uint i = 0; i < edges; ++i) {
      int source, dest, weight;
      input_file >> source >> dest >> weight;
      adj_list[source].push_back({dest, 1});
      adj_list[dest].push_back({source, 1});
    }
    return adj_list;
  }
};

class MTXReader : public FileReader {
public:
  ~MTXReader() {}
  MTXReader() {}

  AdjList readFile(std::string filename) override {
    std::ifstream input_file(filename);
    // first line is always a comment
    input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    int nodes, edges;
    input_file >> nodes >> nodes >> edges;
    ++nodes; // indices start at 1
    AdjList adj_list(nodes);

    for (uint i = 0; i < edges; ++i) {
      int source, dest;
      input_file >> source >> dest;
      adj_list[source].push_back({dest, 1});
      adj_list[dest].push_back({source, 1});
    }
    return adj_list;
  }
};

Benchmark::Benchmark(std::vector<std::string> &filename,
                     std::vector<std::unique_ptr<Algorithm>> &algorithms)
    : _algorithms(algorithms), _filenames(filename) {}

std::vector<std::vector<RunResult>> Benchmark::run() const {
  std::vector<std::vector<RunResult>> results(_filenames.size());
  FileReader *reader;
  for (int fileID = 0; fileID < _filenames.size(); ++fileID) {

    std::cout << "Reading file " << _filenames[fileID];
    std::cout.flush();

    std::string fileextension;
    try {
      fileextension = _filenames[fileID].substr(_filenames[fileID].rfind('.'));
      switch (filename_map.at(fileextension)) {
      case FileExtension::EDGELIST:
        reader = new EdgeListReader();
        break;
      case FileExtension::MTXREADER:
        reader = new MTXReader();
        break;
      }
    } catch (const std::out_of_range &e) {
      e.what();
      throw std::runtime_error("Could not determine file extension of file " +
                               _filenames[fileID]);
    }

    AdjList adj_list = reader->readFile(_filenames[fileID]);

    std::cout << " ...done" << std::endl;

    std::cout << "Starting batch";
    std::cout.flush();

    Batch batch(std::move(adj_list), _algorithms);
    results[fileID] = batch.run();

    std::cout << "...done" << std::endl;
    delete reader;
  }
  return results;
}

} // namespace maxcut
