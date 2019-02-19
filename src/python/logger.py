from queue import Queue
import threading
import sys

class Logger:
    def __init__(self, logfile, log_to_stdout=False):
        self.header = ["run_number", "algorithm", "iteration", "cut_weight"]
        self.lines = []
        self.log_to_stdout = log_to_stdout
        if not self.log_to_stdout:
            self.filename = logfile
            self.file = open(logfile + '.csv', "w")
            self.file.write(",".join(self.header) + '\n')
        else:
            sys.stdout.write(",".join(self.header) + '\n')

    def log(self, *data):
        data_strings = [str(x) for x in data]
        self.lines.append(",".join(data_strings) + '\n')

    def write(self, lines):
        if self.log_to_stdout:
            import sys
            for line in lines:
               sys.stdout.write(line)
        else:
            for line in lines:
                self.file.write(line)
            self.file.flush()
            
    def close(self):
        if self.log_to_stdout:
            sys.stdout.write(",".join(self.header) + '\n')
            for line in self.lines:
               sys.stdout.write(line)
        else:
            for line in self.lines:
                self.file.write(line)
            self.file.flush()
            self.file.close()
