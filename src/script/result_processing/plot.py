#!/usr/bin/env python3
import matplotlib.pyplot as plt
import json
from fmt_conversion import inst_to_algo

colors = ['black', 'gray', 'red','orange','yellow','lime','green','cyan','lightblue','blue','magenta','pink']

COLOR_BASELINE = 'blue'
COLOR_OTHERS = 'black'
COLOR_PMUTACTVITY = 'lime'
COLOR_GREEDY = 'red'
COLOR_UNIFSIGMOID = 'cyan'
COLOR_GRASP = 'black'
COLOR_DESOUSA = 'black'
COLOR_BURER = 'black'

color_dict = {
        "fmut_1.5": COLOR_BASELINE,
        "fmut_2.5": COLOR_BASELINE,
        "fmut_3.5": COLOR_BASELINE,
        "greedy": COLOR_GREEDY,
        "pmutActivity_1.5_0.8": COLOR_PMUTACTVITY,
        "pmutActivity_1.5_0.95": COLOR_PMUTACTVITY,
        "pmutActivity_2.5_0.8": COLOR_PMUTACTVITY,
        "pmutActivity_2.5_0.95": COLOR_PMUTACTVITY,
        "pmutActivity_3.5_0.8": COLOR_PMUTACTVITY,
        "pmutActivity_3.5_0.95": COLOR_PMUTACTVITY,
        "pmut_1.5": COLOR_BASELINE,
        "pmut_2.5": COLOR_BASELINE,
        "pmut_3.5": COLOR_BASELINE,
        "unif": COLOR_BASELINE,
        "unifSigmoid_0.8": COLOR_UNIFSIGMOID,
        "unifSigmoid_0.95": COLOR_UNIFSIGMOID,
        "BURER2002": COLOR_BURER,
        "DESOUSA2013": COLOR_DESOUSA,
        "DUARTE2005": COLOR_OTHERS,
        "FESTA2002G": COLOR_GRASP,
        "FESTA2002GPR": COLOR_GRASP,
        "FESTA2002GVNS": COLOR_GRASP,
        "FESTA2002GVNSPR": COLOR_GRASP,
        "FESTA2002VNS": COLOR_GRASP,
        "FESTA2002VNSPR": COLOR_GRASP,
        "LAGUNA2009CE": COLOR_OTHERS,
        "LAGUNA2009HCE": COLOR_OTHERS,
}

def fill_gaps(instances, data):
    return [get(data, i) for i in instances]

def get(dic, item):
    if item in dic:
        return dic[item]
    else:
        return None

def to_percentages(y_axes):
    for i in range(len(y_axes[0])):
        data = [axis[i] for axis in y_axes]
        max_value = None
        for d in data:
            if max_value is None or (d is not None and d > max_value):
                max_value = d
        if max_value is not None and max_value > 0:
            for axis in y_axes:
                if axis[i] is not None:
                    axis[i] /= max_value

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('instances')
    parser.add_argument('json_data', nargs='+')
    parser.add_argument('-f', '--fmt', choices=['ALGO', 'INST'], default='INST')
    args = parser.parse_args()

    with open(args.instances) as f:
        instances = [l.strip() for l in f.readlines()]
    instances.sort()

    data_a = []
    for fname in args.json_data:
        with open(fname) as f:
            data = json.load(f)
        data_a.append(data)

    if args.fmt == 'INST':
        for i, data in enumerate(data_a):
            data_a[i] = inst_to_algo(data)

    algorithm_results = {k:v for x in data_a for k,v in x.items()}

    x_axis = instances
    plot_items = []
    for i,(algorithm, data) in enumerate(algorithm_results.items()):
        if algorithm in color_dict:
            color = color_dict[algorithm]
        else:
            color = colors[i%len(colors)]
        item = [
                algorithm,
                'x',
                color,
                fill_gaps(instances, data)
        ]
        plot_items.append(item)

    y_axes = [item[3] for item in plot_items]
    to_percentages(y_axes)

    f = plt.figure(figsize=(20,6), dpi=80)
    subplt = f.add_subplot(111)
    for label, fmt, color, y_axis in plot_items:
        subplt.plot(x_axis, y_axis, fmt, label=label, color=color)

    subplt.set_xticklabels([])

    subplt.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0.)
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()
