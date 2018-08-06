#!/u r/bin/python3
"""Max Cut Evolutionary Algorithm"""

import networkx as nx
import random
import math
import cProfile

class Solution:
    def __init__(self, array):
        self.array = array

# https://stackoverflow.com/questions/918736/random-number-generator-that-produces-a-power-law-distribution
def randomPowerLawNumber(n, x0, x1):
    y = random.random()
    x = ((x1**(n+1) - x0**(n+1))*y + x0**(n+1))**(1/(n+1))
    return math.floor(x)

class PMUTMutator:
    def __init__(self, beta):
        self.beta = beta
    
    def mutate(self, solution):
        mutated_array = solution.array.copy()
        # choose k from power law distribution
        k = randomPowerLawNumber(self.beta, 0, len(mutated_array))
        
        # flip k bits chosen uniformly at random
        nodes_to_flip = random.sample(range(len(mutated_array)),k)

        for i in nodes_to_flip:
            mutated_array[i] = mutated_array[i] ^ 1

        return Solution(mutated_array)

class MaxCutEvaluator:
    def __init__(self, graph):
        self.graph = graph
    
    def evaluate(self, solution):
        score = 0
        chosen_nodes = [x for x in range(len(solution.array)) if solution.array[x] == True]
        edges = self.graph.out_edges(chosen_nodes)
        score = len(edges)
        return score

def readMTX(filename):
    graph = nx.read_edgelist(filename, create_using=nx.DiGraph(), nodetype=int)
    return graph

def initialSolution(graph):
    node_count = max(graph.nodes())
    return Solution([False for x in range(node_count)])

def main():
    ITERATION_COUNT = 100000
    POWER_LAW_BETA = 1.5
    filename = "data/test.mtx"

    # read graph
    graph = readMTX(filename)

    # set iteration amount
    iterations = ITERATION_COUNT
    mutator = PMUTMutator(POWER_LAW_BETA)
    evaluator = MaxCutEvaluator(graph)
    solution = initialSolution(graph)
    solution.score = evaluator.evaluate(solution)
    solution.iteration = -1

    for i in range(iterations):
        mutation = mutator.mutate(solution)
        mutation.score = evaluator.evaluate(mutation)
        if mutation.score > solution.score:
            solution = mutation
            solution.iteration = i
    
    print("Score: " + str(solution.score) + "\n")
    print("Iteration: " + str(solution.iteration) + "\n")

# cProfile.run('main()')
main()
