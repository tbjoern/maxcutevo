#!/usr/bin/env python

import matplotlib.pyplot as plt
import json

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('avg_fitness_json')
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
        for id, fitness in algo_data.items():
            id = int(id)
            fitness = int(fitness)
            y_axes[id].append(fitness)


    f = plt.figure()
    subplt = f.add_subplot(111)
    subplt.tick_params(axis='x', labelrotation=90)
    for y_axis, label, fmt in zip(y_axes, labels, fmt):
        subplt.plot(x_axis, y_axis, fmt, label=label)

    subplt.legend()
    plt.tight_layout()
    plt.show()

if __name__ == '__main__':
    main()
        
