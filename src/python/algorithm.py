import random
import math
import copy
from collections import namedtuple
from numpy.random import exponential
from functools import lru_cache

IterationData = namedtuple('IterationData', 'iteration cut_weight')

# def randomPowerLawNumber(n, x0, x1):
#     y = random.random()
#     x = ((x1**(n+1) - x0**(n+1))*y + x0**(n+1))**(1/(n+1))
#     return math.floor(x)

def randomPowerLawNumber(beta, max_value):
    value = exponential(beta)
    if value > max_value:
        return max_value
    return math.ceil(value)

def make_sigmoid(smoothness):
    mult = smoothness * -1
    @lru_cache(300)
    def sigmoid(x):
        return 1/(1 + math.exp(mult*x))
    return sigmoid

class Algorithm:
    deterministic = False
    
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
        self.CUT_SET = 1
        self.NON_CUT_SET = -1
        self.init_cut_tracking()
        # self.flip_nodes_randomly()

    def flip_nodes_randomly(self):
        for node in self.graph.nodes:
            if random.random() >= 0.5:
                self.flip_node(node)

    def init_cut_tracking(self):
        self.change = {}
        # side stores which partition a node belongs tp
        # -1 is the "non-cut-set", 1 is the "cut-set"
        # only edges going from 1 to -1 are part of the cut
        self.side = {}
        for node in self.graph.in_edges:
            self.change[node] = 0
            self.side[node] = self.NON_CUT_SET
        for node, edges in self.graph.out_edges.items():
            self.change[node] = 0
            for edge in edges:
                self.change[node] += edge.weight
            self.side[node] = self.NON_CUT_SET


    def flip_node(self, node):
        self.cut_weight += self.change[node]
        self.change[node] *= -1
        self.side[node] *= -1
        self.update_change_values(node)
    
    def update_change_values(self, node):
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

    def __str__(self):
        return "GRASP"

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
        self.cut_weight = 0
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
                if max(sigma[0], sigma[1]) >= mu:
                    candidate_list.append(node)

            # select node from candidate list and put it into a partition
            chosen_node = random.choice(candidate_list)
            sigma_l, sigma_r = node_sigma_mapping[chosen_node]
            if sigma_l > sigma_r:
                self.side[chosen_node] = -1
            else:
                self.side[chosen_node] = 1

            self.cut_weight += max(sigma_l, sigma_r)

            # update sigma values
            if self.side[chosen_node] == -1 and chosen_node in self.graph.in_edges:
                for edge in self.graph.in_edges[chosen_node]:
                    if edge.neighbour in node_sigma_mapping:
                        nb_sigma_l, nb_sigma_r = node_sigma_mapping[edge.neighbour]
                        node_sigma_mapping[edge.neighbour] = (nb_sigma_l, nb_sigma_r + edge.weight)

            if self.side[chosen_node] == 1 and chosen_node in self.graph.out_edges:
                for edge in self.graph.out_edges[chosen_node]:
                    if edge.neighbour in node_sigma_mapping:
                        nb_sigma_l, nb_sigma_r = node_sigma_mapping[edge.neighbour]
                        node_sigma_mapping[edge.neighbour] = (nb_sigma_l + edge.weight, nb_sigma_r)

            # delete chosen node from remaining nodes
            del node_sigma_mapping[chosen_node]

        # calculate change values
        for node in self.graph.nodes:
            self.change[node] = 0
        # going over all outgoing edges is sufficient, as every edge is covered that way
        for node, edges in self.graph.out_edges.items():
            for edge in edges:
                if self.side[node] == 1 and self.side[edge.neighbour] == -1:
                    self.change[node] -= edge.weight
                    self.change[edge.neighbour] -= edge.weight
                elif self.side[node] == -1 and self.side[edge.neighbour] == -1:
                    self.change[node] += edge.weight
                elif self.side[node] == 1 and self.side[edge.neighbour] == 1:
                    self.change[edge.neighbour] += edge.weight

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
    def __init__(self,graph, start_activity=10, inc=1, dec=1, max=100, min=1, decay=0.95):
        super().__init__(graph)
        self.start_activity = start_activity
        self.activity_inc = inc
        self.activity_dec = dec
        self.activity_max = max
        self.activity_min = min
        self.decay_rate = decay
        self.activity = {}
        for node in graph.nodes:
            self.activity[node] = self.start_activity

    def update_activity(self, flipped_nodes):
        for node in flipped_nodes:
            if self.side[node] == self.CUT_SET:
                for edge in self.graph.out_edges[node]:
                    if self.side[edge.neighbour] == self.CUT_SET:
                        self.activity[edge.neighbour] += self.activity_inc
                    else:
                        self.activity[edge.neighbour] -= self.activity_dec
            else:
                for edge in self.graph.in_edges[node]:
                    if self.side[edge.neighbour] == self.CUT_SET:
                        self.activity[edge.neighbour] -= self.activity_dec
                    else:
                        self.activity[edge.neighbour] += self.activity_inc
        for node in flipped_nodes:
            self.activity[node] = self.start_activity
    
    def clamp_activity(self, node):
        if self.activity[node] > self.activity_max:
            self.activity[node] = self.activity_max
        if self.activity[node] < self.activity_min:
            self.activity[node] = self.activity_min

    def clamp_all_activity(self):
        for node in self.activity:
            self.clamp_activity(node)
    
    def decay_activity(self):
        for node, activity in self.activity.items():
            self.activity[node] = (activity - self.start_activity) * self.decay_rate + self.start_activity
    
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
    def __init__(self, graph, power_law_beta, **kwargs):
        self.power_law_beta = power_law_beta
        self.node_list = list(graph.nodes)
        super().__init__(graph, **kwargs)

    def __str__(self):
        return "pmutActivity_" + str(self.power_law_beta)

    
    def iterate(self):
        k = randomPowerLawNumber(self.power_law_beta, len(self.graph.nodes))
        chosen_nodes = self.choose_k_unique(self.node_list, self.activity, k=k)
        flipped = self.flip_nodes_if_improvement(chosen_nodes)

        if flipped:
            self.update_activity(chosen_nodes)
            self.decay_activity()
            self.clamp_all_activity()
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
    def __init__(self, graph, power_law_beta, **kwargs):
        self.power_law_beta = power_law_beta
        self.node_list = list(graph.nodes)
        super().__init__(graph, **kwargs)

    def __str__(self):
        return "pmut_" + str(self.power_law_beta)

    def iterate(self):
        k = randomPowerLawNumber(self.power_law_beta, len(self.graph.nodes))
        chosen_nodes = random.sample(self.node_list, k)
        self.flip_nodes_if_improvement(chosen_nodes)
        return super().iterate()

class greedy(FlipAlgorithm):
    deterministic = True

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

class greedyActivity(ActivityAlgorithm):
    deterministic = True

    def __str__(self):
        return "greedyActivity"

    def iterate(self):
        maximum = 0
        best_node = None
        for node, change in self.change.items():
            activity_change = change * (self.activity[node] / self.start_activity)
            if activity_change > maximum:
                best_node = node
                maximum = activity_change
        if best_node is not None:
            self.flip_node(best_node)
            self.update_activity([best_node])
            self.decay_activity()
            self.clamp_all_activity()
        return super().iterate()

class greedyActivityReverse(ActivityAlgorithm):
    deterministic = True

    def __str__(self):
        return "greedyActivity"

    def iterate(self):
        maximum = 0
        best_node = None
        for node, change in self.change.items():
            activity_change = change * (self.start_activity / self.activity[node])
            if activity_change > maximum:
                best_node = node
                maximum = activity_change
        if best_node is not None:
            self.flip_node(best_node)
            self.update_activity([best_node])
            self.decay_activity()
            self.clamp_all_activity()
        return super().iterate()


class unifActivitySigmoid(ActivityAlgorithm):
    def __init__(self, graph, power_law_beta, max=100, min=-100, sigmoid_smoothness = 0.05,**kwargs):
        self.power_law_beta = power_law_beta
        self.node_list = list(graph.nodes)
        self.sigmoid_lower = 1 / (len(self.node_list)**2)
        self.sigmoid_center = 1 / (len(self.node_list))
        self.sigmoid_upper = 0.5
        self.sigmoid_mp = self.sigmoid_upper - self.sigmoid_center
        self.sigmoid_mn = self.sigmoid_center - self.sigmoid_lower
        self.sigmoid = make_sigmoid(sigmoid_smoothness)
        super().__init__(graph, max=max, min=min, start_activity=0, **kwargs)

    def __str__(self):
        return "unifActivitySigmoid"

    def decay_activity(self):
        for node, activity in self.activity.items():
            self.activity[node] = activity * self.decay_rate

    def map_to_sigmoid_chance(self, activity):
        sigmoid_value = self.sigmoid(math.ceil(activity)) * 2
        if sigmoid_value >= 1:
            sigmoid_value -= 1
            return self.sigmoid_mp * sigmoid_value + self.sigmoid_center
        else:
            return self.sigmoid_mn * sigmoid_value + self.sigmoid_lower

    def sample_nodes(self):
        chosen_nodes = []
        for node in self.node_list:
            chance = self.map_to_sigmoid_chance(self.activity[node])
            sample = random.random()
            if sample <= chance:
                chosen_nodes.append(node)
        return chosen_nodes

    def iterate(self):
        chosen_nodes = self.sample_nodes()
        if chosen_nodes:
            flipped = self.flip_nodes_if_improvement(chosen_nodes)

            if flipped:
                self.update_activity(chosen_nodes)
                self.decay_activity()
                self.clamp_all_activity()
        return super().iterate()
