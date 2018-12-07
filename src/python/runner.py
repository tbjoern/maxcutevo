import algorithm
import threading

class Runner:
    def __init__(self, graph, logger, iterations, algorithm_class, *algorithm_params):
        self.graph = graph
        self.logger = logger
        self.iterations = iterations
        self.algorithm_class = algorithm_class
        self.algorithm_params = algorithm_params

    def run_algorithm(self, run_nr):
        algorithm = self.algorithm_class(self.graph, *(self.algorithm_params))
        id = self.logger.add_algorithm(str(algorithm))
        for _ in range(self.iterations):
            it_data = algorithm.iterate()
            self.logger.log(run_nr, id, it_data.iteration, it_data.cut_weight)

class ThreadRunner(threading.Thread):
    def __init__(self, runner, run_nr):
        super().__init__(self)
        self.runner = runner
        self.run_nr = run_nr

    def run(self):
        self.runner.run_algorithm(self.run_nr)

if __name__ == "__main__":
    def class_for_name(module_name, class_name):
        # load the module, will raise ImportError if module cannot be loaded
        m = __import__(module_name, globals(), locals(), class_name)
        # get the class, will raise AttributeError if class cannot be found
        c = getattr(m, class_name)
        return c
    def main():
        import sys
        import os
        if len(sys.argv) < 4:
            print("Usage: runner.py <edgelist file> <iterations> <algorithm> <algorithm params>")
            exit(127)
        import graph
        from logger import Logger
        from math import floor
        from time import time

        logfile_name = "_".join([os.path.basename(sys.argv[1]),sys.argv[3],str(floor(time()))])

        try:
            c = class_for_name('algorithm', sys.argv[3])
            runner = Runner(graph.Graph(sys.argv[1]), Logger(logfile_name), int(sys.argv[2]), c, *[int(x) for x in sys.argv[4:]])
        except graph.GraphReadError:
            print("Graph could not be read")

        runner.run_algorithm(0)
    import cProfile
    cProfile.run('main()')

