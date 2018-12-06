class Logger:
    def __init__(self, logfile):
        self.filename = logfile
        self.file = open(logfile + '.csv', "w")
        self.header = ["run_number", "algorithm", "iteration", "cut_weight"]
        self.file.write(",".join(self.header) + '\n')
        self.run_number = 0
        self.algorithm_id_map = {}
        self.algorithm_id_current = 0
        self.algorithm_id_max = 0
        self.line_buffer = []

    def set_run_number(self, run_nr):
        self.run_number = run_nr
    
    def set_algorithm(self, algorithm_name):
        if not algorithm_name in self.algorithm_id_map:
            self.algorithm_id_map[algorithm_name] = self.algorithm_id_max
            self.algorithm_id_max += 1
        self.algorithm_id_current = self.algorithm_id_map[algorithm_name]

    def write_algorithm_id_map(self, filename):
        with open(filename + '.csv', 'w') as mapfile:
            mapfile.write('id,name\n')
            for name,id in self.algorithm_id_map.items():
                mapfile.write('{},{}\n'.format(id,name))
    
    def log(self, *data):
        data_strings = [str(x) for x in data]
        data_strings = [str(self.run_number), str(self.algorithm_id_current)] + data_strings
        self.line_buffer.append(",".join(data_strings) + '\n')

    def close(self):
        self.file.writelines(self.line_buffer)


