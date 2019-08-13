#!/usr/bin/env python3

from summary import get_summaries_and_configs, id_name_map_from_configs, merge_summary

def avg_fitness(summary):
    summary_avg_fitness = {}
    for instance, algo_data in summary.items():
        summary_avg_fitness[instance] = {}
        for id, runs in algo_data.items():
            avg_fitness = 0
            data_points = 0
            for run, data in runs.items():
                avg_fitness += data['fitness']
                data_points += 1
            avg_fitness /= data_points
            summary_avg_fitness[instance][id] = avg_fitness
    return summary_avg_fitness

def main():
    import argparse
    import sys
    import json
    parser = argparse.ArgumentParser()
    parser.add_argument('dirs', nargs='*')
    args = parser.parse_args()

    if len(args.dirs) == 0:
        exit(1)

    summaries,configs = get_summaries_and_configs(args.dirs)

    summary = merge_summary(*summaries)

    summary_avg_fitness = avg_fitness(summary)

    id_name_map = id_name_map_from_configs(configs)

    output = { 'avg_fitness': summary_avg_fitness, 'id_name_map': id_name_map }

    json.dump(output, sys.stdout)
    


if __name__ == '__main__':
    main()


