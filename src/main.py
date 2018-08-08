#!/u r/bin/python3
"""Max Cut Evolutionary Algorithm"""

import networkx as nx
import random
import math
import cProfile

def node_count_of(graph):
    return len(graph.nodes())

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
    def __init__(self, beta, graph):
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
    def __init__(self, beta, graph):
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

class PMUTActivityMutator:
    def __init__(self, beta, graph):
        self.beta = beta
        self.n = node_count_of(graph)
        self.activity = [[x,0] for x in range(self.n)]
        self.graph = graph
        self.DECAY_FACTOR = 0.9
    
    def mutate(self, solution):
        mutated_array = solution.array.copy()
        # choose k from power law distribution
        k = randomPowerLawNumber(self.beta, 1, self.n)
        
        # flip k bits with highest activity
        self.activity.sort(reverse=True, key=lambda item: item[1])

        for i in range(k):
            node = self.activity[i][0]
            mutated_array[node] = mutated_array[node] ^ True
            if mutated_array[node]:
                for neighbor in self.graph.neighbors(node):
                    if mutated_array[neighbor]:
                        modifier = 1
                    else:
                        modifier = -1
                    for j in range(self.n):
                        if self.activity[j][0] == neighbor:
                            self.activity[j][1] += modifier

        for i in range(self.n):
            self.activity[i][1] *= self.DECAY_FACTOR

        return Solution(mutated_array)
    
    def __str__(self):
        return "pmut_activity_" + str(self.beta)


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
    node_count = node_count_of(graph)
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
    graph = nx.convert_node_labels_to_integers(graph)

    # set iteration amount
    iterations = ITERATION_COUNT
    evaluator = MaxCutEvaluator(graph)
    mutators = [f(x, graph) for f in [FMUTMutator, PMUTMutator, PMUTActivityMutator] for x in POWER_LAW_BETAS]

    for mutator in mutators:
        solution = runEA(graph, mutator, evaluator, iterations)
        print(str(mutator) + "\n")
        print(str(solution))
    

#cProfile.run('main()')
main()
