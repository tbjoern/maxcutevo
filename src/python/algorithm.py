import random
import math
import copy
from collections import namedtuple

IterationData = namedtuple('IterationData', 'iteration cut_weight')

def randomPowerLawNumber(n, x0, x1):
    y = random.random()
    x = ((x1**(n+1) - x0**(n+1))*y + x0**(n+1))**(1/(n+1))
    return math.floor(x)

class Algorithm:
    def __init__(self, graph):
        self.graph = graph
        self.iteration = 0
        self.cut_weight = 0

    def iterate(self):
        self.iteration += 1
        return IterationData(self.iteration, self.cut_weight)

"""MaxCut Algorithm Framework Class"""
class FlipAlgorithm(Algorithm):
    def __init__(self, graph):
        super().__init__(graph)
        self.init_cut_tracking()

    def init_cut_tracking(self):
        self.change = {}
        # side stores which partition a node belongs tp
        # -1 is the "non-cut-set", 1 is the "cut-set"
        # only edges going from 1 to -1 are part of the cut
        self.side = {}
        for node in self.graph.in_edges:
            self.change[node] = 0
            self.side[node] = -1
        for node, edges in self.graph.out_edges.items():
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

class GRASP(FlipAlgorithm):
    def __init__(self, graph):
        super().__init__(graph)

    def get_bounds(self, node_sigma_mapping):
        sigma_min = None
        sigma_max = None
        for sigmaL, sigmaR in node_sigma_mapping.values():
            sigma_t_max = max(sigmaL, sigmaR)
            sigma_t_min = min(sigmaL, sigmaR)
            if sigma_min is None or sigma_t_min < sigma_min:
                sigma_min = sigma_t_min
            if sigma_max is None or sigma_t_max > sigma_max:
                sigma_max = sigma_t_max
        return (sigma_max, sigma_min)

    def build_random_soluton(self):
        self.change = {}
        self.side = {}
        node_sigma_mapping = {}
        for node in self.graph.nodes:
            # sigma for side: (-1, 1)
            node_sigma_mapping[node] = (0,0)
        while node_sigma_mapping:
            sigma_max, sigma_min = self.get_bounds(node_sigma_mapping)
            alpha = random.random()

            # threshold value for selection
            mu = sigma_min + alpha * (sigma_max - sigma_min)
            # build candidate list
            candidate_list = []
            for node, sigma in node_sigma_mapping.items():
                if sigma >= mu:
                    candidate_list.append(node)

            # select node from candidate list and put it into a partition
            chosen_node = random.choice(candidate_list)
            sigma_l, sigma_r = node_sigma_mapping[chosen_node]
            if sigma_l > sigma_r:
                side[chosen_node] = -1
            else
                side[chosen_node] = 1

            # update sigma values
            if side[chosen_node] == -1 and chosen_node in self.graph.in_edges:
                for edge in self.graph.in_edges[chosen_node]:
                    nb_sigma_l, nb_sigma_r = node_sigma_mapping[edge.neighbour]
                    node_sigma_mapping[edge.neighbour] = (nb_sigma_l, nb_sigma_r + edge.weight)

            if side[chosen_node] == 1 and chosen_node in self.graph.out_edges:
                for edge in self.graph.out_edges[chosen_node]:
                    nb_sigma_l, nb_sigma_r = node_sigma_mapping[edge.neighbour]
                    node_sigma_mapping[edge.neighbour] = (nb_sigma_l + edge.weight, nb_sigma_r)

        # calculate change values
        # going over all outgoing edges is sufficient, as every edge is covered that way
        for node, edges in self.graph.out_edges.items():
            for edge in edges:
                if self.side[node] == 1 and self.side[edge.neigbour] == -1:
                    self.change[node] -= edge.weight
                    self.change[edge.neighbour] -= edge.weight
                elif self.side[node] == -1 and self.side[edge.neighbour] == -1:
                    self.change[node] += edge.weight
                elif self.side[node] == 1 and self.side[edge.neighbour] == 1:
                    self.change[node.neighbour] += edge.weight

    def sigma(self, node, side):
        total = 0
        if side == 1:
            if node in self.graph.out_edges:
                for edge in self.graph.out_edges[node]:
                    if edge.neighbour in self.side and self.side[edge.neighbour] == -1:
                        total += edge.weight
        else:
            if node in self.graph.in_edges:
                for edge in self.graph.in_edges[node]:
                    if edge.neighbour in self.side and self.side[edge.neighbour] == 1:
                        total += edge.weight
        return total

    
    def reached_local_optimum(self):
        for change in self.change.values():
            if change > 0:
                break
        else:
            return True
        return False

    def optimize_solution(self):
        max_change = 0
        best_node = None
        for node, change in self.change.items():
            if change > max_change:
                best_node = node
                max_change = change
        assert(best_node is not None)
        self.flip_node(best_node)

    def iterate(self):
        if self.reached_local_optimum():
            self.build_random_soluton()
        else:
            self.optimize_solution()
        return super().iterate()
    

class ActivityAlgorithm(FlipAlgorithm):
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

    def flip_node(self, node):
        self.activity[node] = self.start_activity
        super().flip_node(node)
    
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
                        total_weight -= weights[node]
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


class unif(FlipAlgorithm):
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

class pmut(FlipAlgorithm):
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

class greedy(FlipAlgorithm):
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
