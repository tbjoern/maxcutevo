import algorithm

class Runner:
    def __init__(self, graph, logger, iterations):
        self.graph = graph
        self.logger = logger
        self.iterations = iterations
        self.algorithm_id = 0
        self.algorithm_id_mapping = {}

    def run_algorithm(self, algorithm_class):
        self.algorithm_id_mapping[self.algorithm_id] = algorithm_class(self.graph)
        algorithm = self.algorithm_id_mapping[self.algorithm_id]
        for i in range(self.iterations):
            it_data = algorithm.iterate()
            self.logger.log(self.algorithm_id, it_data.iteration, it_data.cut_weight)
        self.algorithm_id += 1

if __name__ == "__main__":
    def main():
        import sys
        if len(sys.argv) != 3:
            print("Usage: runner.py <edgelist file> <iterations>")
            exit(127)
        import graph
        from logger import Logger
        try:
            runner = Runner(graph.Graph(sys.argv[1]), Logger("logfile.txt"), int(sys.argv[2]))
        except graph.GraphReadError:
            print("Graph could not be read")

        from algorithm import UnifAlgorithm

        runner.run_algorithm(UnifAlgorithm)
    import cProfile
    cProfile.run('main()')

