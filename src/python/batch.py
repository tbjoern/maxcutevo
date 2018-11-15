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

    def run(self, file, run_count=1):
        if run_count < 1:
            raise ValueError("run_count must be a positive integer value")
        try:
            instance = graph.Graph(file)
        except FileNotFoundError:
            print("Cannot find file {}, skipping.".format(file))
            return ""
        logfile_name = "_".join([os.path.basename(file),str(floor(time()))])
        logger = Logger(logfile_name)

        for run_nr in range(run_count):
            logger.set_run_number(run_nr)
            runner = Runner(instance, logger, self.iterations)

            for algorithm_config in self.algorithm_configs:
                runner.run_algorithm(algorithm_config.class_obj, *algorithm_config.arguments)
            
        logger.write_algorithm_id_map(logfile_name + '_mapping')

        return logfile_name
        


if __name__ == "__main__":
    import sys
    if len(sys.argv) < 3:
        print("Usage: batch.py <config_file> <instance_file> <number of runs>")
        exit(127)
    
    from config_reader import read_config

    number_of_runs = 1
    if len(sys.argv) == 4:
        number_of_runs = int(sys.argv[3])

    batch = Batch(read_config(sys.argv[1]))
    batch.run(sys.argv[2], number_of_runs)

    