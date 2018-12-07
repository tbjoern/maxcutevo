from queue import Queue
import threading

class Logger:
    def __init__(self, logfile):
        self.filename = logfile
        self.file = open(logfile + '.csv', "w")
        self.header = ["run_number", "algorithm", "iteration", "cut_weight"]
        self.file.write(",".join(self.header) + '\n')
        self.algorithm_id_map = {}
        self.algorithm_id_max = 0
        self.line_buffer = Queue()
        self.lock = threading.Lock()
    
    def add_algorithm(self, algorithm_name):
        self.lock.acquire()
        if not algorithm_name in self.algorithm_id_map:
            id = self.algorithm_id_max
            self.algorithm_id_map[algorithm_name] = id
            self.algorithm_id_max += 1
        else:
            id = self.algorithm_id_map[algorithm_name]
        self.lock.release()
        return id

    def write_algorithm_id_map(self, filename):
        with open(filename + '.csv', 'w') as mapfile:
            mapfile.write('id,name\n')
            for name,id in self.algorithm_id_map.items():
                mapfile.write('{},{}\n'.format(id,name))
    
    # thread safe
    def log(self, *data):
        data_strings = [str(x) for x in data]
        self.line_buffer.put(",".join(data_strings) + '\n')

    def close(self):
        while self.line_buffer.qsize():
            self.file.write(self.line_buffer.get())
        self.file.flush()
        self.file.close()


