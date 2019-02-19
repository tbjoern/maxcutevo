from runner import Runner, ThreadRunner
from logger import Logger
import graph
from time import time
from math import floor
import os
import threading
from multiprocessing import Process, Queue, current_process, freeze_support

def worker(input, output):
    for runner, run_nr in iter(input.get, 'STOP'):
        runner.run_algorithm(run_nr)
        output.put(runner.logger.lines)

class Batch:
    def __init__(self, config, log_to_stdout=False):
        self.algorithm_configs = config["algorithms"]
        self.iterations = config["iterations"]
        self.run_count = config["run_count"]
        self.parallel = config["parallel"]
        self.log_to_stdout = log_to_stdout

    def run(self, file):
        try:
            instance = graph.Graph(file)
        except FileNotFoundError:
            print("Cannot find file {}, skipping.".format(file))
            return ""
        logfile_name = "_".join([os.path.basename(file),str(floor(time()))])
        logger = Logger(logfile_name, log_to_stdout=self.log_to_stdout)

        if self.parallel > 0:
            run_tasks = Queue(self.parallel)
            finished_queue = Queue()
            def generate_run_tasks():
                for algorithm_config in self.algorithm_configs:
                    runner = Runner(instance, logger, self.iterations, algorithm_config)
                    for run_nr in range(self.run_count):
                        # using the same runner for each thread is fine,
                        # because run_algorithm does not mutate the runner instance
                        yield (runner, run_nr)
                for _ in range(self.parallel):
                   yield 'STOP'

            for _ in range(self.parallel):
                Process(target=worker, args=(run_tasks, finished_queue)).start()

            for run_task in generate_run_tasks():
                run_tasks.put(run_task)
                
            for _ in range(len(self.algorithm_configs * self.run_count)):
                lines = finished_queue.get()
                logger.write(lines)
        else:
            for algorithm_config in self.algorithm_configs:
                runner = Runner(instance, logger, self.iterations, algorithm_config)

                for run_nr in range(self.run_count):
                    runner.run_algorithm(run_nr)
        logger.close()
        return logfile_name
        
if __name__ == "__main__":
    import sys, argparse

    parser = argparse.ArgumentParser()
    parser.add_argument('config_file')
    parser.add_argument('instance_file')
    parser.add_argument('--stdout', action='store_true')
    
    args = parser.parse_args()
    
    from config_reader import read_config

    batch = Batch(read_config(args.config_file), log_to_stdout=args.stdout)
    batch.run(args.instance_file)

    