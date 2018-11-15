import random
import math
from collections import namedtuple

IterationData = namedtuple('IterationData', 'iteration cut_weight')

def randomPowerLawNumber(n, x0, x1):
    y = random.random()
    x = ((x1**(n+1) - x0**(n+1))*y + x0**(n+1))**(1/(n+1))
    return math.floor(x)

"""MaxCut Algoritm Framework Class"""
class Algorithm:
    def __init__(self, graph):
        self.graph = graph
        self.side = {}
        self.change = {}
        self.cut_weight = 0
        self.iteration = 0
        for node in graph.in_edges:
            self.change[node] = 0
            self.side[node] = -1
        for node, edges in graph.out_edges.items():
            self.change[node] = 0
            for edge in edges:
                self.change[node] += edge.weight
            self.side[node] = -1

    def flip_node(self, node):
        self.cut_weight += self.change[node]
        self.change[node] *= -1
        self.side[node] *= -1
        for edge in self.graph.in_edges[node]:
            self.change[edge.neighbour] += edge.weight * self.side[edge.neighbour] * self.side[node]
        for edge in self.graph.out_edges[node]:
            self.change[edge.neighbour] += edge.weight * self.side[edge.neighbour] * self.side[node]

    def flip_nodes(self, nodes):
        for node in nodes:
            self.flip_node(node)

    def flip_nodes_if_improvement(self, nodes):
        old_cut_weight = self.cut_weight
        self.flip_nodes(nodes)
        if self.cut_weight < old_cut_weight:
            self.flip_nodes(nodes)
            return False
        return True
    
    def iterate(self):
        self.iteration += 1
        return IterationData(self.iteration, self.cut_weight)

class ActivityAlgorithm(Algorithm):
    def __init__(self,graph):
        super().__init__(graph)
        self.start_activity = len(graph.nodes)/10
        self.activity_inc = len(graph.nodes)/10
        self.activity_dec = -1*len(graph.nodes)/10
        self.activity_max = 10*len(graph.nodes)
        self.activity_min = 1
        self.activity = {}
        for node in graph.nodes:
            self.activity[node] = self.start_activity

    def update_activity(self, flipped_nodes):
        for node in flipped_nodes:
            for edge in self.graph.in_edges[node]:
                if self.side[node] == self.side[edge.neighbour]:
                    self.activity[edge.neighbour] += self.activity_inc
                else:
                    self.activity[edge.neighbour] += self.activity_dec
                if self.activity[edge.neighbour] > self.activity_max:
                    self.activity[edge.neighbour] = self.activity_max
                if self.activity[edge.neighbour] < self.activity_min:
                    self.activity[edge.neighbour] = self.activity_min
            for edge in self.graph.out_edges[node]:
                if self.side[node] == self.side[edge.neighbour]:
                    self.activity[edge.neighbour] += self.activity_inc
                else:
                    self.activity[edge.neighbour] += self.activity_dec
                if self.activity[edge.neighbour] > self.activity_max:
                    self.activity[edge.neighbour] = self.activity_max
                if self.activity[edge.neighbour] < self.activity_min:
                    self.activity[edge.neighbour] = self.activity_min
    
    def choose_k_unique(self, population, weights, k):
        result = []
        total_weight = sum(weights.values())
        node_chosen = {}
        for node in population:
            node_chosen[node] = False
        for _ in range(k):
            r = random.random() * total_weight
            cum_weight = 0
            for node in population:
                if not node_chosen[node]:
                    cum_weight += weights[node]
                    if r <= cum_weight:
                        result.append(node)
                        node_chosen[node] = True
                        break
        return result

class pmutActivity(ActivityAlgorithm):
    def __init__(self, graph, power_law_beta):
        self.power_law_beta = power_law_beta
        self.node_list = list(graph.nodes)
        super().__init__(graph)

    def __str__(self):
        return "pmutActivity_" + str(self.power_law_beta)

    
    def iterate(self):
        k = randomPowerLawNumber(self.power_law_beta, 1, len(self.graph.nodes))
        chosen_nodes = self.choose_k_unique(self.node_list, self.activity, k=k)
        self.flip_nodes_if_improvement(chosen_nodes)
        return super().iterate()


class unif(Algorithm):
    def __str__(self):
        return "unif"

    def iterate(self):
        chosen_nodes = []
        sample_threshold = 1/len(self.graph.nodes)
        while not chosen_nodes:
            for node in self.graph.nodes:
                # sample with probabilty 1/n
                # sample = random.randrange(len(self.graph.nodes))
                sample = random.random()
                if sample <= sample_threshold:
                    chosen_nodes.append(node)
        self.flip_nodes_if_improvement(chosen_nodes)
        return super().iterate()

class pmut(Algorithm):
    def __init__(self, graph, power_law_beta):
        self.power_law_beta = power_law_beta
        self.node_list = list(graph.nodes)
        super().__init__(graph)

    def __str__(self):
        return "pmut_" + str(self.power_law_beta)

    def iterate(self):
        k = randomPowerLawNumber(self.power_law_beta, 1, len(self.graph.nodes))
        chosen_nodes = random.choices(self.node_list, k=k)
        self.flip_nodes_if_improvement(chosen_nodes)
        return super().iterate()

class greedy(Algorithm):
    def __str__(self):
        return "greedy"

    def iterate(self):
        maximum = 0
        best_node = None
        for node, change in self.change.items():
            if change > maximum:
                best_node = node
                maximum = change
        if best_node is not None:
            self.flip_node(best_node)
        return super().iterate()
