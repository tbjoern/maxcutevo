from collections import namedtuple
import os
import json

AlgorithmConfig = namedtuple('AlgorithmConfig', 'class_obj id arguments')

class MissingConfigParameter(Exception):
    pass

def class_for_name(module_name, class_name):
    # load the module, will raise ImportError if module cannot be loaded
    m = __import__(module_name, globals(), locals(), class_name)
    # get the class, will raise AttributeError if class cannot be found
    c = getattr(m, class_name)
    return c

def read_ini(file_path, config):
    algorithm_id = 0
    with open(file_path, 'r') as f:
        for line in f:
            try:
                entry_type, value = line.split('=')
                entry_type = entry_type.lower()
                if entry_type == "iterations" or entry_type == "run_count" or entry_type == "cpu_count":
                    config[entry_type] = int(value)
                elif entry_type == "algorithm":
                    algorithm_values = value.split()
                    algorithm_class = class_for_name("algorithm", algorithm_values[0])
                    config["algorithms"].append(AlgorithmConfig(class_obj=algorithm_class, arguments=[float(x) for x in algorithm_values[1:]], id=algorithm_id))
                    algorithm_id += 1
                elif entry_type == 'parallel':
                    config[entry_type] = "True" in value
            except Exception as x:
                print(x)
                print("error while parsing config entry: {}={} - skipping".format(entry_type, value))
    return config

def read_json(file_path, config):
    with open(file_path, 'r') as f:
        json_config = json.load(f)
    config = {**config, **json_config}
    config['algorithms'] = []
    for algorithm in json_config['algorithms']:
        arguments = []
        id = algorithm['id']
        algorithm_class = class_for_name('algorithm', algorithm['name'])
        if 'arguments' in algorithm:
            for param, value in algorithm['arguments'].items():
                arguments.append(float(value))
        config['algorithms'].append(AlgorithmConfig(class_obj=algorithm_class, arguments=arguments, id=id))
    return config


def read_config(file_path):
    filename, file_extension = os.path.splitext(file_path)
    default_config = { 'iterations': 100, 'algorithms': [], 'run_count': 10 , 'parallel': False, 'cpu_count': 4 }
    if file_extension == '.ini':
        return read_ini(file_path, config=default_config)
    if file_extension == '.json':
        return read_json(file_path, config=default_config)
