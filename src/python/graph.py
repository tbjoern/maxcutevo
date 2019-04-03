from collections import namedtuple
import os

Edge = namedtuple('Edge', 'neighbour weight')

class Graph:
    def __init__(self):
        # adjacency list format: { node: [(neighbour, weight), ...], ... }
        self.in_edges = {} # adjacency list with incoming edges
        self.out_edges = {} # adjacency list with outgoing edges
        self.nodes = set()
        self.edge_count = 0

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

    def neatify_node_indices(self):
        node_list = sorted(self.nodes)
        node_mapping = {}
        for i, node in enumerate(node_list):
            node_mapping[node] = i
        in_edges = self.in_edges
        out_edges = self.out_edges
        self.in_edges = {}
        self.out_edges = {}
        for node, adj_nodes in in_edges.items():
            self.in_edges[node_mapping[node]] = [Edge(node_mapping[i], weight) for i, weight in adj_nodes]
        for node, adj_nodes in out_edges.items():
            self.out_edges[node_mapping[node]] = [Edge(node_mapping[i], weight) for i, weight in adj_nodes]
        self.nodes = set(range(len(node_list)))

class GraphReadError(Exception):
    pass

def read_file(file):
    graph = Graph()
    filename, file_extension = os.path.splitext(file)
    if file_extension == ".mtx":
        graph = read_matrix_notation(graph, file)
    elif file_extension == ".csv":
        graph = read_csv(graph, file)
    elif file_extension == ".rud" or file_extension == ".mc" or file_extension == ".txt":
        graph = read_headed_weighted_edgelist(graph, file)
    else:
        graph = read_edgelist(graph, file)
    graph.neatify_node_indices()
    return graph

def read_edgelist(graph, file):
    lines = []
    with open(file, "r") as file:
        lines = file.readlines()
    try:
        for line in lines:
            graph.edge_count += 1
            start, end = [int(x.strip()) for x in line.split()]
            graph.add_edge(start, end, 1)
    except Exception as x:
        print(x)
        raise GraphReadError()
    return graph

def read_headed_weighted_edgelist(graph, file):
    lines = []
    with open(file, "r") as file:
        lines = file.readlines()
    try:
        header = lines[0]
        nodes, graph.edge_count = [int(x) for x in header.split()]
        for line in lines[1:]:
            start, end, weight = [int(x) for x in line.split()]
            graph.add_edge(start, end, weight)
    except Exception as x:
        print(x)
        raise GraphReadError()
    return graph

def read_csv(graph, file):
    lines = []
    with open(file, "r") as file:
        lines = file.readlines()
    try:
        edge_count = 0
        for line in lines:
            start, end = [int(x) for x in line.split()]
            graph.add_edge(start, end, 1)
            graph.add_edge(end, start, 1)
            edge_count += 2
        graph.edge_count = edge_count
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
        nodes, nodes, graph.edge_count = [int(x) for x in header.split()]
        for line in lines[(index+1):]:
            start, end = [int(x) for x in line.split()]
            graph.add_edge(start, end, 1)
    except Exception as x:
        print(x)
        raise GraphReadError()
    return graph

def as_headed_weighted_edgelist(graph):
    yield '{} {}'.format(len(graph.nodes), graph.edge_count)
    for node, edges in graph.out_edges.items():
        for edge in edges:
            yield '{} {} {}'.format(node, edge.neighbour, edge.weight)
    

if __name__ == "__main__":
    test_data = ["4 2", "0 1 1", "2 3 -1"]
    with open("test.rud", "w") as f:
        f.write("\n".join(test_data))
    g = read_file('test.rud')
    assert(g.nodes == set([0,1,2,3]))
    assert(g.edge_count == 2)
    assert(g.out_edges[0][0].neighbour == 1)
    assert(g.out_edges[0][0].weight == 1)
    assert(g.in_edges[1][0].neighbour == 0)
    assert(g.in_edges[1][0].weight == 1)
    assert(g.out_edges[2][0].neighbour == 3)
    assert(g.out_edges[2][0].weight == -1)
    assert(g.in_edges[3][0].neighbour == 2)
    assert(g.in_edges[3][0].weight == -1)
    print("graph.py test successful")
