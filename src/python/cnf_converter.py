import graph
from itertools import product

def readCNF(filename):
    with open(filename, 'r') as cnffile:
        lines = cnffile.readlines()
    without_comments = [line for line in lines if line[0] not in 'cp']
    unify = " ".join(without_comments)
    tokens = unify.split()
    clauses = []
    clause = []
    for token in tokens:
        if token != '0':
            clause.append(int(token))
        else:
            clauses.append(clause)
            clause = []
    return clauses

def clauses_to_LIG(clauses):
    g = graph.Graph()
    for clause in clauses:
        literal_pairs = ((a,b) for a in clause for b in clause if a != b)
        for literal_1, literal_2 in literal_pairs:
            g.add_edge(literal_1, literal_2, 1)
    return g

if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument('cnf')
    parser.add_argument('type', choices=['LIG'])
    parser.add_argument('--output')

    args = parser.parse_args()

    clauses = readCNF(args.cnf)

    if args.type == 'LIG':
        g = clauses_to_LIG(clauses)
    
    if args.output:
        with open(args.output, 'w') as f:
            for line in graph.as_headed_weighted_edgelist(g):
                f.write(line)
                f.write('\n')
    else:
        for line in graph.as_headed_weighted_edgelist(g):
            print(line)
