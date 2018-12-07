from runner import Runner, ThreadRunner
from logger import Logger
import graph
from time import time
from math import floor
import os
import threading

class Batch:
    def __init__(self, config):
        self.algorithm_configs = config["algorithms"]
        self.iterations = config["iterations"]
        self.run_count = config["run_count"]
        self.parallel = config["parallel"]

    def run(self, file):
        try:
            instance = graph.Graph(file)
        except FileNotFoundError:
            print("Cannot find file {}, skipping.".format(file))
            return ""
        logfile_name = "_".join([os.path.basename(file),str(floor(time()))])
        logger = Logger(logfile_name)

        if self.parallel:
            threads = []
            for algorithm_config in self.algorithm_configs:
                runner = Runner(instance, logger, self.iterations, algorithm_config.class_obj, *algorithm_config.arguments)
                for run_nr in range(self.run_count):
                    # using the same runner for each thread is fine,
                    # because run_algorithm does not mutate the runner instance
                    threads.append(ThreadRunner(runner, run_nr))
            for thread in threads:
                thread.start()
            for thread in threads:
                thread.join()
        else:
            for algorithm_config in self.algorithm_configs:
                runner = Runner(instance, logger, self.iterations, algorithm_config.class_obj, *algorithm_config.arguments)

                for run_nr in range(self.run_count):
                    runner.run_algorithm(run_nr)
            
        logger.write_algorithm_id_map(logfile_name + '_mapping')
        logger.close()

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
    import cProfile
    cProfile.run('batch.run(sys.argv[2])')

    