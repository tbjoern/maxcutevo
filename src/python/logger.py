from queue import Queue
import threading

class Logger:
    def __init__(self, logfile, log_to_stdout=False):
        self.header = ["run_number", "algorithm", "iteration", "cut_weight"]
        self.line_buffer = Queue()
        self.lock = threading.Lock()
        self.log_to_stdout = log_to_stdout
        if not self.log_to_stdout:
            self.filename = logfile
            self.file = open(logfile + '.csv', "w")
            self.file.write(",".join(self.header) + '\n')

    # thread safe
    def log(self, *data):
        data_strings = [str(x) for x in data]
        self.line_buffer.put(",".join(data_strings) + '\n')

    def close(self):
        if self.log_to_stdout:
            import sys
            sys.stdout.write(",".join(self.header) + '\n')
            while self.line_buffer.qsize():
               sys.stdout.write(self.line_buffer.get())
        else:
            while self.line_buffer.qsize():
                self.file.write(self.line_buffer.get())
            self.file.flush()
            self.file.close()
