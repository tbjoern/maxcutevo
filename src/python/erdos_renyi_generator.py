from argparse import ArgumentParser
import networkx as nx
import matplotlib.pyplot as plt
import os

def generate_graph(size, probabilty, directed):
    return nx.fast_gnp_random_graph(size,probabilty,directed=directed)

if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument('-size', type=int, help='number of nodes in the graphs', required=True)
    parser.add_argument('-p', type=float, help='probabilty that an edge exists', required=True)
    parser.add_argument('-u', action='store_false', help='creates undirected graphs', default=True)
    parser.add_argument('-c', type=int, help='amount of graphs that will be created', default=1)
    parser.add_argument('-prefix', help='specify a file prefix', default='ed_graph')
    parser.add_argument('-dir', help='where to store the generated graphs', default='.')

    cfg = parser.parse_args()
    graph_dir = os.path.abspath(cfg.dir)

    for i in range(cfg.c):
        G = generate_graph(cfg.size, cfg.p, cfg.u)
        nx.write_weighted_edgelist(G, os.path.join(graph_dir, cfg.prefix + "_{}.edgelist".format(i)))
