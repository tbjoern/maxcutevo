from collections import namedtuple

AlgorithmConfig = namedtuple('AlgorithmConfig', 'class_obj arguments')

def class_for_name(module_name, class_name):
    # load the module, will raise ImportError if module cannot be loaded
    m = __import__(module_name, globals(), locals(), class_name)
    # get the class, will raise AttributeError if class cannot be found
    c = getattr(m, class_name)
    return c

def read_config(filename):
    # config format:
    # iterations=1000
    # algorithm=unifAlgorithm
    # algorithm=pmutAlgorithm -2
    config = { 'iterations': 100, 'algorithms': [], 'run_count': 10 , 'parallel': False, 'cpu_count': 0 }
    with open(filename, 'r') as f:
        for line in f:
            try:
                entry_type, value = line.split('=')
                entry_type = entry_type.lower()
                if entry_type == "iterations" or entry_type == "run_count" or entry_type == "cpu_count":
                    config[entry_type] = int(value)
                elif entry_type == "algorithm":
                    algorithm_values = value.split()
                    algorithm_class = class_for_name("algorithm", algorithm_values[0])
                    config["algorithms"].append(AlgorithmConfig(class_obj=algorithm_class, arguments=[float(x) for x in algorithm_values[1:]]))
                elif entry_type == 'parallel':
                    config[entry_type] = "True" in value
            except Exception as x:
                print(x)
                print("error while parsing config entry: {}={} - skipping".format(entry_type, value))

    return config
