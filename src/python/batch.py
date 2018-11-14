from runner import Runner
from logger import Logger
import graph
from time import time
from math import floor
import os
from collections import namedtuple

AlgorithmConfig = namedtuple('AlgorithmConfig', 'class_obj arguments')

class Batch:
    def __init__(self, config):
        self.algorithm_configs = config["algorithms"]
        self.iterations = config["iterations"]

    def run(self, file):
        try:
            instance = graph.Graph(file)
        except FileNotFoundError:
            print("Cannot find file {}, skipping.".format(file))
            return
        logfile_name = "_".join([os.path.basename(file),str(floor(time()))])
        runner = Runner(instance, Logger(logfile_name,"algorithm", "iteration", "cut_weight"), self.iterations)

        for algorithm_config in self.algorithm_configs:
            runner.run_algorithm(algorithm_config.class_obj, *algorithm_config.arguments)
        
        mapping_logger = Logger(logfile_name + "_mapping", "id", "name")
        for algo_id, algo_name in runner.algorithm_id_mapping.items():
            mapping_logger.log(algo_id, algo_name)
        


if __name__ == "__main__":
    def class_for_name(module_name, class_name):
        # load the module, will raise ImportError if module cannot be loaded
        m = __import__(module_name, globals(), locals(), class_name)
        # get the class, will raise AttributeError if class cannot be found
        c = getattr(m, class_name)
        return c
    import sys
    if len(sys.argv) < 3:
        print("Usage: batch.py <config_file> [instance_file, ...]")
        exit(127)

    # config format:
    # iterations=1000
    # algorithm=unifAlgorithm
    # algorithm=pmutAlgorithm -2
    config = { 'iterations': 100, 'algorithms': []}
    with open(sys.argv[1], 'r') as f:
        for line in f:
            try:
                entry_type, value = line.split('=')
                entry_type = entry_type.lower()
                if entry_type == "iterations":
                    config["iterations"] = int(value)
                elif entry_type == "algorithm":
                    algorithm_values = value.split()
                    algorithm_class = class_for_name("algorithm", algorithm_values[0])
                    config["algorithms"].append(AlgorithmConfig(class_obj=algorithm_class, arguments=[int(x) for x in algorithm_values[1:]]))
            except Exception as x:
                print(x)
                print("error while parsing config entry, skipping")

    batch = Batch(config)
    for filename in sys.argv[2:]:
        batch.run(filename)

    