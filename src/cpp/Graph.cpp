#include "Graph.hpp"

#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <stdexcept>
#include <unordered_map>

#include "Batch.hpp"

namespace maxcut {

enum FileExtension { EDGELIST, MTXREADER, CNFREADER, NXEDGELIST };

static std::map<std::string, FileExtension> filename_map = {
    {".mtx", MTXREADER},   {".rud", EDGELIST},  {".mc", EDGELIST},
    {".txt", EDGELIST},    {".cnf", CNFREADER}, {".edgelist", NXEDGELIST},
    {".edges", NXEDGELIST}};

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

    std::set<int> node_names;
    for (uint i = 0; i < edges; ++i) {
      int source, dest, weight;
      input_file >> source >> dest >> weight;
      node_names.insert(source);
      node_names.insert(dest);
    }

    std::unordered_map<int, int> node_alias;
    int node_nr = 0;
    for (auto node : node_names) {
      node_alias[node] = node_nr++;
    }

    input_file.clear();
    input_file.seekg(0);

    input_file >> nodes >> edges;

    for (uint i = 0; i < edges; ++i) {
      int source, dest, weight;
      input_file >> source >> dest >> weight;
      source = node_alias[source];
      dest = node_alias[dest];
      adj_list.add_edge(source, dest, weight);
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
      adj_list.add_edge(source, dest, 1);
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
      if (!(input_file >> node)) {
        ++edge_count;
        input_file.clear();
        input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        continue;
      }
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
      if (!(input_file >> source >> dest)) {
        input_file.clear();
        input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        continue;
      }
      adj_list.add_edge(source, dest, 1);
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
      adj_list.add_edge(source, dest, weight);
    }
    return adj_list;
  }
};

AdjList read_graph(std::string filename) {
  FileReader *reader;
  std::string fileextension;
  try {
    fileextension = filename.substr(filename.rfind('.'));
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
                             filename);
  }

  AdjList adj_list = reader->readFile(filename);
  delete reader;

  return adj_list;
}

} // namespace maxcut
