#!/u r/bin/python3
"""Max Cut Evolutionary Algorithm"""

import networkx as nx
import random
import math
import cProfile

class Solution:
    def __init__(self, array):
        self.array = array
        self.iteration = -1

    def __str__(self):
        return "Score: " + str(self.score) + "\n" + "Iteration: " + str(self.iteration) + "\n"

# https://stackoverflow.com/questions/918736/random-number-generator-that-produces-a-power-law-distribution
def randomPowerLawNumber(n, x0, x1):
    y = random.random()
    x = ((x1**(n+1) - x0**(n+1))*y + x0**(n+1))**(1/(n+1))
    return math.floor(x)

class FMUTMutator:
    def __init__(self, beta):
        self.beta = beta
    
    def mutate(self, solution):
        mutated_array = solution.array.copy()
        n = len(mutated_array)
        # choose a from power law distribution
        a = randomPowerLawNumber(self.beta, 1, n)
        
        # flip each bit with probability a/n

        for i in range(n):
            r = random.randint(1, n)
            if r <= a:
                mutated_array[i] = mutated_array[i] ^ True

        return Solution(mutated_array)

    def __str__(self):
        return "fmut_" + str(self.beta)

class PMUTMutator:
    def __init__(self, beta):
        self.beta = beta
    
    def mutate(self, solution):
        mutated_array = solution.array.copy()
        n = len(mutated_array)
        # choose k from power law distribution
        k = randomPowerLawNumber(self.beta, 1, n)
        
        # flip k bits chosen uniformly at random
        nodes_to_flip = random.sample(range(n),k)

        for i in nodes_to_flip:
            mutated_array[i] = mutated_array[i] ^ True

        return Solution(mutated_array)
    
    def __str__(self):
        return "pmut_" + str(self.beta)

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

def runEA(graph, mutator, evaluator, iterations):
    solution = initialSolution(graph)
    solution.score = evaluator.evaluate(solution)

    for i in range(iterations):
        mutation = mutator.mutate(solution)
        mutation.score = evaluator.evaluate(mutation)
        if mutation.score > solution.score:
            solution = mutation
            solution.iteration = i

    return solution

def main():
    ITERATION_COUNT = 1000
    POWER_LAW_BETAS = [-1.5, -2.5, -3.5]
    filename = "data/test.mtx"

    # read graph
    graph = readMTX(filename)

    # set iteration amount
    iterations = ITERATION_COUNT
    evaluator = MaxCutEvaluator(graph)
    mutators = [f(x) for f in [FMUTMutator, PMUTMutator] for x in POWER_LAW_BETAS]

    for mutator in mutators:
        solution = runEA(graph, mutator, evaluator, iterations)
        print(str(mutator) + "\n")
        print(str(solution))
    

cProfile.run('main()')
#main()
