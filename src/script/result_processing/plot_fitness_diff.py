#!/usr/bin/env python

import matplotlib.pyplot as plt
import json

fmts = ['x', '^']
colors = ['red', 'black', 'orange', 'deepskyblue', 'limegreen', 'blueviolet', 'blue']

def to_percentages(iterable):
    max_value = None
    for i in iterable:
        if max_value is None or i > max_value:
            max_value = i
    return {value:value/max_value for value in iterable}

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('avg_fitness')
    parser.add_argument('avg_fitness_other')
    parser.add_argument('--ids', nargs='+')
    parser.add_argument('--image')
    args = parser.parse_args()

    with open(args.avg_fitness, 'r') as f:
        input = json.load(f)

    with open(args.avg_fitness_other, 'r') as f:
        input_other = json.load(f)

    avg_fitness = input["avg_fitness"]
    id_name_map = input["id_name_map"]
    avg_fitness_other = input_other["avg_fitness"]
    ids = [x for x in id_name_map.keys()]
    if args.ids:
        ids = [x for x in ids if x in args.ids]
    ids = sorted([int(x) for x in ids])
    x_axis = sorted(avg_fitness.keys())

    percentage_maps = {}
    for instance in x_axis:
        algo_data = [avg_fitness[instance][str(id)] for id in ids] + [avg_fitness_other[instance][str(id)] for id in ids]
        percentage_maps[instance] = to_percentages([float(x) for x in algo_data])

    plot_items = []
    for i, id in enumerate(ids):
        plot_a = {
            'y_axis': [],
            'label': id_name_map[str(id)],
            'color': colors[i],
            'fmt': fmts[0],
        }
        plot_b = { 
            'y_axis': [],
            'label': id_name_map[str(id)],
            'color': colors[i],
            'fmt': fmts[1],
        }
        for instance in x_axis:
            plot_a['y_axis'].append(percentage_maps[instance][avg_fitness[instance][str(id)]])
            plot_b['y_axis'].append(percentage_maps[instance][avg_fitness_other[instance][str(id)]])
        plot_items.append(plot_a)
        plot_items.append(plot_b)


    f = plt.figure(figsize=(20,6), dpi=80)
    subplt = f.add_subplot(111)
    #subplt.tick_params(axis='x', labelrotation=90)
    for item in plot_items:
        y_axis = item['y_axis']
        fmt = item['fmt']
        label = item['label']
        color = item['color']
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
        
