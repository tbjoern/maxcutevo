#include "Benchmark.hpp"

#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>

#include "Batch.hpp"

namespace maxcut {

enum FileExtension { EDGELIST, MTXREADER, CNFREADER, NXEDGELIST };

static std::map<std::string, FileExtension> filename_map = {
    {".mtx", MTXREADER}, {".rud", EDGELIST},  {".mc", EDGELIST},
    {".txt", EDGELIST},  {".cnf", CNFREADER}, {".edgelist", NXEDGELIST}};

class FileReader {
public:
  virtual ~FileReader(){};
  virtual AdjList readFile(std::string filename) = 0;
};

class EdgeListReader : public FileReader {
public:
  AdjList readFile(std::string filename) override {
    std::ifstream input_file(filename);
    int nodes, edges;
    input_file >> nodes >> edges;
    AdjList adj_list(nodes);

    for (uint i = 0; i < edges; ++i) {
      int source, dest, weight;
      input_file >> source >> dest >> weight;
      source--, dest--;
      adj_list.out_edges[source].push_back({dest, weight});
      adj_list.in_edges[dest].push_back({source, weight});
    }
    return adj_list;
  }
};

class MTXReader : public FileReader {
public:
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
      adj_list.out_edges[source].push_back({dest, 1});
      adj_list.in_edges[dest].push_back({source, 1});
    }
    return adj_list;
  }
};

class NXEdgeListReader : public FileReader {
public:
  AdjList readFile(std::string filename) override {
    std::ifstream input_file(filename);
    int largest_node = 0;
    int edge_count = 0;
    while (input_file.peek() != EOF) {
      int node;
      input_file >> node;
      if (node > largest_node) {
        largest_node = node;
      }
      ++edge_count;
    }

    edge_count /= 2;

    input_file.clear();
    input_file.seekg(0);

    // node indices start at 0, that means we have largest_node + 1 nodes in
    // total
    AdjList adj_list(largest_node + 1);

    for (uint i = 0; i < edge_count; ++i) {
      int source, dest;
      input_file >> source >> dest;
      adj_list.out_edges[source].push_back({dest, 1});
      adj_list.in_edges[dest].push_back({source, 1});
    }
    return adj_list;
  }
};

class CNFReader : public FileReader {
public:
  AdjList readFile(std::string filename) override {
    std::ifstream input_file(filename);
    // first line is a comment
    input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string dummy;
    int nodes, edges;
    // format is
    // p cnf <nodes> <edges>
    input_file >> dummy >> dummy >> nodes >> edges;
    // every vertex (literal) occurs twice, negative ones are negated
    AdjList adj_list(nodes * 2);

    for (uint i = 0; i < edges; ++i) {
      int source, dest, weight;
      input_file >> source >> dest;
      // some lines aparrently have only one vertex and a zero - ignore those
      if (dest == 0) {
        continue;
      }
      input_file >> weight;
      // map negative vertices to positive ones
      if (source < 0) {
        source = source * -1 + nodes;
      }
      if (dest < 0) {
        dest = dest * -1 + nodes;
      }
      adj_list.out_edges[source].push_back({dest, 1});
      adj_list.in_edges[dest].push_back({source, 1});
    }
    return adj_list;
  }
};

std::vector<std::vector<AlgorithmResult>>
benchmark(std::vector<std::string> &filenames,
          std::vector<std::shared_ptr<Algorithm>> &algorithms,
          const RunConfig config) {
  std::vector<std::vector<AlgorithmResult>> results(filenames.size());
  FileReader *reader;
  for (int fileID = 0; fileID < filenames.size(); ++fileID) {

    // std::cout << "Reading file " << _filenames[fileID];
    // std::cout.flush();

    std::string fileextension;
    try {
      fileextension = filenames[fileID].substr(filenames[fileID].rfind('.'));
      switch (filename_map.at(fileextension)) {
      case FileExtension::EDGELIST:
        reader = new EdgeListReader();
        break;
      case FileExtension::MTXREADER:
        reader = new MTXReader();
        break;
      case FileExtension::CNFREADER:
        reader = new CNFReader();
        break;
      case FileExtension::NXEDGELIST:
        reader = new NXEdgeListReader();
        break;
      }
    } catch (const std::out_of_range &e) {
      e.what();
      throw std::runtime_error("Could not determine file extension of file " +
                               filenames[fileID]);
    }

    AdjList adj_list = reader->readFile(filenames[fileID]);

    // std::cout << " ...done" << std::endl;

    // std::cout << "Starting batch";
    // std::cout.flush();

    results[fileID] = batch(adj_list, algorithms, config);

    // std::cout << "...done" << std::endl;
    delete reader;
  }
  return results;
}

} // namespace maxcut
