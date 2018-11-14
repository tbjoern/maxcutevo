from runner import Runner
from logger import Logger
import graph
from time import time
from math import floor
import os

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

        return logfile_name
        


if __name__ == "__main__":
    import sys
    if len(sys.argv) < 3:
        print("Usage: batch.py <config_file> [instance_file, ...]")
        exit(127)
    
    from config_reader import read_config

    batch = Batch(read_config(sys.argv[1]))
    for filename in sys.argv[2:]:
        batch.run(filename)

    