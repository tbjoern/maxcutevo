from collections import namedtuple
import os

Edge = namedtuple('Edge', 'neighbour weight')

class Graph:
    def __init__(self, file):
        # adjacency list format: { node: [(neighbour, weight), ...], ... }
        self.in_edges = {} # adjacency list with incoming edges
        self.out_edges = {} # adjacency list with outgoing edges
        self.nodes = set()
        self.edges = 0
        read_file(self, file)

    def add_edge(self, start, end, weight):
        if not start in self.in_edges:
            self.in_edges[start] = []
            self.out_edges[start] = []
        if not end in self.in_edges:
            self.in_edges[end] = []
            self.out_edges[end] = []
        if not start in self.nodes:
            self.nodes.add(start)
        if not end in self.nodes:
            self.nodes.add(end)
        self.out_edges[start].append(Edge(end, weight))
        self.in_edges[end].append(Edge(start,weight))

class GraphReadError(Exception):
    pass

def read_file(graph, file):
    filename, file_extension = os.path.splitext(file)
    if file_extension == ".mtx":
        return read_matrix_notation(graph, file)
    else:
        return read_edgelist(graph, file)

def read_edgelist(graph, file):
    lines = []
    with open(file, "r") as file:
        lines = file.readlines()
    try:
        header = lines[0]
        nodes, graph.edges = [int(x) for x in header.split()]
        for line in lines[1:]:
            start, end, weight = [int(x) for x in line.split()]
            graph.add_edge(start, end, weight)
    except Exception as x:
        print(x)
        raise GraphReadError()
    return graph

def read_matrix_notation(graph, file):
    lines = []
    with open(file, "r") as file:
        lines = file.readlines()
    try:
        index = 0
        while '%' in lines[index]:
            index += 1
        header = lines[index]
        nodes, nodes, graph.edges = [int(x) for x in header.split()]
        for line in lines[(index+1):]:
            start, end = [int(x) for x in line.split()]
            graph.add_edge(start, end, 1)
    except Exception as x:
        print(x)
        raise GraphReadError()
    return graph
    

if __name__ == "__main__":
    test_data = ["4 2", "0 1 1", "2 3 -1"]
    with open("test.txt", "w") as f:
        f.write("\n".join(test_data))
    g = Graph("test.txt")
    assert(g.nodes == set([0,1,2,3]))
    assert(g.edges == 2)
    assert(g.out_edges[0][0].neighbour == 1)
    assert(g.out_edges[0][0].weight == 1)
    assert(g.in_edges[1][0].neighbour == 0)
    assert(g.in_edges[1][0].weight == 1)
    assert(g.out_edges[2][0].neighbour == 3)
    assert(g.out_edges[2][0].weight == -1)
    assert(g.in_edges[3][0].neighbour == 2)
    assert(g.in_edges[3][0].weight == -1)
    print("graph.py test successful")
