import algorithm

class Runner:
    def __init__(self, graph, logger, iterations):
        self.graph = graph
        self.logger = logger
        self.iterations = iterations

    def run_algorithm(self, algorithm_class, *algorithm_params):
        algorithm = algorithm_class(self.graph, *algorithm_params)
        self.logger.set_algorithm(str(algorithm))
        for _ in range(self.iterations):
            it_data = algorithm.iterate()
            self.logger.log(it_data.iteration, it_data.cut_weight)

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
            runner = Runner(graph.Graph(sys.argv[1]), Logger(logfile_name, "algorithm", "iteration", "cut_weight"), int(sys.argv[2]))
        except graph.GraphReadError:
            print("Graph could not be read")

        c = class_for_name('algorithm', sys.argv[3])

        runner.run_algorithm(c, *[int(x) for x in sys.argv[4:]])
    import cProfile
    cProfile.run('main()')

