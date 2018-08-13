#!/u r/bin/python3
"""Max Cut Evolutionary Algorithm"""

import networkx as nx
import random
import math
import cProfile
import sys
import os

def node_count_of(graph):
    return len(graph.nodes())

def choose_k_unique(population, weights, k):
    population = list(population)
    weights = list(weights)
    result = []
    total_weight = sum(weights)
    node_chosen = [False for i in range(len(population))]
    for i in range(k):
        r = random.random() * total_weight
        cum_weight = 0
        for i in range(len(population)):
            cum_weight += weights[i]
            if r <= cum_weight and not node_chosen[i]:
                result.append(population[i])
                node_chosen[i] = True
                break
    return result

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
        self.activity = dict([(x,1) for x in range(self.n)])
        self.graph = graph
        self.DECAY_FACTOR = 0.2
    
    def mutate(self, solution):
        mutated_array = solution.array.copy()
        # choose k from power law distribution
        k = randomPowerLawNumber(self.beta, 1, self.n)
        
        chosen_nodes = choose_k_unique(self.graph.nodes(), self.activity.values(), k)
        for node in chosen_nodes:
            # choose node with probability proportional to its activity
            self.activity[node] = 1
            mutated_array[node] = mutated_array[node] ^ True # flip
            if mutated_array[node]:
                # for all neighbours in A - increase activity by 1
                # for all neighbours not in A - decrease activity by 1
                activity_modifier = 1
            else:
                # for all neighbours in A - decrease activity by 1
                # for all neighbours not in A - increase activity by 1
                activity_modifier = -1
            for neighbor in self.graph.neighbors(node):
                if not mutated_array[neighbor]:
                    activity_modifier *= -1
                self.activity[neighbor] = max(1, self.activity[neighbor] + activity_modifier)

        for node, activity in self.activity.items():
            activity = activity * self.DECAY_FACTOR

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

class EvolutionaryAlgorithm:
    def __init__(self, filename, mutator_classes, power_law_betas, evaluator_class, iterations):
        self.iterations = iterations
        self.filename = filename
        self.readMTX()
        self.evaluator = evaluator_class(self.graph)
        self.mutators = [f(x, self.graph) for f in mutator_classes for x in power_law_betas]
        self.solutions = list()

    def readMTX(self):
        graph = nx.read_edgelist(self.filename, create_using=nx.DiGraph(), nodetype=int)
        self.graph = nx.convert_node_labels_to_integers(graph)

    def getInitialSolution(self):
        node_count = node_count_of(self.graph)
        return Solution([False for x in range(node_count)])

    def runMutator(self, mutator):
        solution = self.getInitialSolution()
        solution.score = self.evaluator.evaluate(solution)
        for i in range(self.iterations):
            mutation = mutator.mutate(solution)
            mutation.score = self.evaluator.evaluate(mutation)
            if mutation.score > solution.score:
                solution = mutation
                solution.iteration = i
        return solution
    
    def runAllMutators(self):
        for mutator in self.mutators:
            self.solutions.append(self.runMutator(mutator))
            # print(str(mutator) + "\n")
            # print(str(self.solutions[-1]))

    def getSolutions(self):
        return [(self.mutators[i], self.solutions[i]) for i in range(len(self.solutions))]

class Logger:
    def __init__(self, filename):
        self.file = open(filename, "w")

    def log_console(self, string):
        print(string)

    def log_csv(self, values):
        line = ""
        for value in values:
            line += str(value)
            line += ','
        line = line[:-1]
        line += "\n"
        self.file.write(line)


def main():
    if len(sys.argv) != 3:
        print("Usage: main.py <path_to_graph_dir> <logfile>")
        exit(127)

    ITERATION_COUNT = 10
    POWER_LAW_BETAS = [-1.5, -2.5, -3.5]
    MUTATOR_CLASSES = [FMUTMutator, PMUTMutator, PMUTActivityMutator]
    EVALUATOR_CLASS = MaxCutEvaluator

    DIR = sys.argv[1]
    FILESIZE_THRESHOLD = 1024*1024*1 # 1 MB
    FILENAMES = [os.path.join(DIR, file) for file in os.listdir(DIR) if os.path.getsize(os.path.join(DIR, file)) < FILESIZE_THRESHOLD]

    FILENAME_ID_MAP = dict([(FILENAMES[i], i) for i in range(len(FILENAMES))])
    MUTATOR_ID_MAP = dict([(MUTATOR_CLASSES[i], i) for i in range(len(MUTATOR_CLASSES))])

    with open("file_mapping.csv", "w") as file_mapping:
        for f in FILENAMES:
            file_mapping.write(str(FILENAME_ID_MAP[f]) + "," + f + "\n")
    with open("mutator_mapping.csv", "w") as mutator_mapping:
        for m in MUTATOR_CLASSES:
            mutator_mapping.write(str(MUTATOR_ID_MAP[m]) + "," + m.__name__ + "\n")


    LOGFILE = sys.argv[2]
    logger = Logger(LOGFILE)

    for filename in FILENAMES:
        print(filename)
        id = FILENAME_ID_MAP[filename]
        ea = EvolutionaryAlgorithm(filename, MUTATOR_CLASSES, POWER_LAW_BETAS, EVALUATOR_CLASS, ITERATION_COUNT)

        ea.runAllMutators()

        for mutator, solution in ea.getSolutions():
            logger.log_csv([id, MUTATOR_ID_MAP[type(mutator)], mutator.beta , solution.score, solution.iteration])
    

#cProfile.run('main()')
main()
