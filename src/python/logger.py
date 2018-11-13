class Logger:
    def __init__(self, logfile, *header):
        self.file = open(logfile, "w")
        if header:
            self.file.write(",".join(header) + '\n')
    
    def log(self, *data):
        self.file.write(",".join([str(x) for x in data]) + '\n')
