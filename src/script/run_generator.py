#!/usr/bin/env python
import json
import copy
from collections import namedtuple

Algorithm = namedtuple('Algorithm', ['name', 'arguments'])
Argument = namedtuple('Argument', ['name', 'values'])
ArgumentGroup = namedtuple('ArgumentGroup', ['arguments', 'validator'])

power_law_beta = Argument('power_law_beta', [1.5,2.5,3.5])
def activity_validator(start_activity, max, min, inc, dec, decay_rate):
    return (
        min < max and
        start_activity <= max and 
        start_activity >= min and
        inc < max and
        dec == inc
    )

def pmut_validator(start_activity, max, min, inc, dec, decay_rate):
    return (
        activity_validator(start_activity, max, min, inc, dec, decay_rate) and
        min >= 1
    )

def unif_validator(start_activity, max, min, inc, dec, decay_rate):
    return (
        activity_validator(start_activity, max, min, inc, dec, decay_rate) and
        start_activity == 0 and
        abs(min) == max
    )

activity_arguments = [
    Argument('start_activity', [0, 50, 500]),
    Argument('max', [100, 1000]),
    Argument('min', [-1000, -100, 1]),
    Argument('inc', [1,10]),
    Argument('dec', [1,10]),
    Argument('decay_rate', [0.95, 0.8, 0.5, 0.3])
]


algorithms = [
    Algorithm(
        name='greedy',
        arguments=[]
    ),
    Algorithm(
        name = 'unif',
        arguments=[]
    ),
    Algorithm(
        name = 'pmut',
        arguments=[
            power_law_beta
        ]
    ),
    Algorithm(
        name = 'fmut',
        arguments=[
            power_law_beta
        ]
    ),
    Algorithm(
        name = 'pmutActivity',
        arguments=[
            ArgumentGroup(activity_arguments, pmut_validator),
            power_law_beta
        ]
    ),
    Algorithm(
        name = 'unifSigmoid',
        arguments=[
            ArgumentGroup(activity_arguments, unif_validator),
            Argument('sigmoid_smoothness', [ 0.5, 0.05, 0.005, 0.0005 ])
        ]
    )
]

def generate_arguments(arguments):
    if len(arguments) == 0:
        yield {}
    else:
        arg = arguments[0]
        if type(arg).__name__ == 'Argument':
            for value in arg.values:
                for arglist in generate_arguments(arguments[1:]):
                    arglist.update({arg.name: value})
                    yield arglist
        elif type(arg).__name__ == 'ArgumentGroup':
            for arglist in generate_arguments(arg.arguments):
                if arg.validator(**arglist):
                    for arglist2 in generate_arguments(arguments[1:]):
                        arglist.update(arglist2)
                        yield arglist

id = 0
run_config = {
    "iterations": 1000,
    "run_count": 10,
    "random_start": False,
    "algorithms": [],
}
for algorithm in algorithms:
    # print(config)
    # print(algorithm.arguments)
    for arglist in generate_arguments(algorithm.arguments):
        config = {}
        config["id"] = id
        config["name"] = algorithm.name
        config["arguments"] = arglist
        run_config["algorithms"].append(copy.deepcopy(config))
        id += 1

print(json.dumps(run_config, indent=4, separators=(',', ': ')))
