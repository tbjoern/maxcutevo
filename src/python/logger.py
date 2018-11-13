class Logger:
    def __init__(self, logfile, *header):
        self.file = open(logfile, "w")
        self.file.write(",".join(header))
    
    def log(self, *data):
        self.file.write(",".join([str(x) for x in data]) + '\n')
