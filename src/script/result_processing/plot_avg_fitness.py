#!/usr/bin/env python

import matplotlib.pyplot as plt
import json

def to_percentages(iterable):
    max_value = None
    for i in iterable:
        if max_value is None or i > max_value:
            max_value = i
    return {value:value/max_value for value in iterable}

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('avg_fitness_json')
    parser.add_argument('--image')
    args = parser.parse_args()

    with open(args.avg_fitness_json, 'r') as f:
        input = json.load(f)

    avg_fitness = input["avg_fitness"]
    id_name_map = input["id_name_map"]
    max_id = max([int(x) for x in id_name_map.keys()])

    x_axis = sorted(avg_fitness.keys())
    y_axes = [[] for _ in range(max_id + 1)]
    labels = [p[1] for p in sorted(id_name_map.items(), key=lambda x: int(x[0]))]
    fmt = []
    for label in labels:
        label_fmt = 'x'
        if 'pmut_' in label:
            label_fmt = 'o'
        if 'fmut_' in label:
            label_fmt = 's'
        if 'pmutActivity_' in label:
            label_fmt = '^'
        if 'unifSigmoid_' in label:
            label_fmt = 'D'
        fmt.append(label_fmt)

    for instance in x_axis:
        algo_data = avg_fitness[instance]
        percentage_map = to_percentages([int(x) for x in algo_data.values()])
        for id, fitness in algo_data.items():
            id = int(id)
            fitness = percentage_map[int(fitness)]
            y_axes[id].append(fitness)

    f = plt.figure(figsize=(20,6), dpi=80)
    subplt = f.add_subplot(111)
    subplt.tick_params(axis='x', labelrotation=90)
    for y_axis, label, fmt in zip(y_axes, labels, fmt):
        subplt.plot(x_axis, y_axis, fmt, label=label)

    subplt.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0.)
    plt.tight_layout()
    if args.image is not None:
        plt.savefig(args.image)
    else:
        plt.show()

if __name__ == '__main__':
    main()
        
