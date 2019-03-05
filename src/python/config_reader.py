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

def read_json(file_path, config):
    with open(file_path, 'r') as f:
        json_config = json.load(f)
    config = {**config, **json_config}
    config['algorithms'] = []
    for algorithm in json_config['algorithms']:
        arguments = {}
        id = algorithm['id']
        algorithm_class = class_for_name('algorithm', algorithm['name'])
        if 'arguments' in algorithm:
            for param, value in algorithm['arguments'].items():
                arguments[param] = float(value)
        config['algorithms'].append(AlgorithmConfig(class_obj=algorithm_class, arguments=arguments, id=id))
    return config


def read_config(file_path):
    filename, file_extension = os.path.splitext(file_path)
    default_config = { 'iterations': 100, 'algorithms': [], 'run_count': 10 , 'parallel': False, 'cpu_count': 4 }
    if file_extension == '.json':
        return read_json(file_path, config=default_config)
    raise RuntimeError(f'Unknown config format: {file_extension}')
