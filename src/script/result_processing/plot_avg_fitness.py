#!/usr/bin/env python

import matplotlib.pyplot as plt
import json

fmts = ['x', 'o', 's', '^', 'D', '.']
colors = ['red', 'black', 'orange', 'deepskyblue', 'limegreen', 'blueviolet']

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
    color = []
    last_prefix = "greedy"
    color_i = 0
    fmt_i = 0 
    for label in labels:
        prefix = label.split('_')[0]
        if prefix != last_prefix:
            color_i += 1
            fmt_i = 0
            last_prefix = prefix
        else:
            fmt_i += 1
        fmt.append(fmts[fmt_i])
        color.append(colors[color_i])

    for instance in x_axis:
        algo_data = avg_fitness[instance]
        percentage_map = to_percentages([int(x) for x in algo_data.values()])
        for id, fitness in algo_data.items():
            id = int(id)
            fitness = percentage_map[int(fitness)]
            y_axes[id].append(fitness)

    f = plt.figure(figsize=(20,6), dpi=80)
    subplt = f.add_subplot(111)
    #subplt.tick_params(axis='x', labelrotation=90)
    for y_axis, label, fmt, color in zip(y_axes, labels, fmt, color):
        subplt.plot(x_axis, y_axis, fmt, label=label, color=color)

    subplt.set_xticklabels([])

    subplt.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0.)
    plt.tight_layout()
    if args.image is not None:
        plt.savefig(args.image)
    else:
        plt.show()

if __name__ == '__main__':
    main()
        
