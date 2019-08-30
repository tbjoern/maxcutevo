#!/usr/bin/env python

import json
import sys
from fmt_conversion import inst_to_algo

def to_percentages(kvpairs):
    best_value = None
    for k,v in kvpairs:
        if best_value is None or (v is not None and v > best_value):
            best_value = v
    def div(v):
        if v is None:
            return 0
        return v/best_value
    return {k:div(v) for k,v in kvpairs}

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('avg_fitness_json')
    parser.add_argument('-r' , '--ranktype', choices=['DISCRETE', 'ABSOLUTE'], default='DISCRETE')
    args = parser.parse_args()

    with open(args.avg_fitness_json, 'r') as f:
        avg_fitness = json.load(f)

    algofmt_avg_fitness = inst_to_algo(avg_fitness)
    ranks = {a:[] for a in algofmt_avg_fitness}

    if args.ranktype == 'DISCRETE':
        for instance, algo_data in avg_fitness.items():
            if len(algo_data) == 0:
                continue
            ranked_fitness = sorted(algo_data.items(), key=lambda x: x[1], reverse=True)
            rank = 1
            cur_fitness = ranked_fitness[0][1]
            for name, fitness in ranked_fitness:
                if fitness < cur_fitness:
                    rank += 1
                    cur_fitness = fitness
                ranks[name].append(rank)
    elif args.ranktype == 'ABSOLUTE':
        for instance, algo_data in avg_fitness.items():
            if len(algo_data) == 0:
                continue
            pct_fitness = to_percentages(algo_data.items())
            for name, pct in pct_fitness.items():
                ranks[name].append(pct)

    average_ranks = {}
    for name, rank_a in ranks.items():
        average_ranks[name] = sum(rank_a)/len(rank_a)

    sorted_average_ranks = sorted(average_ranks.items(), key=lambda x: x[1])
    for name, rank in sorted_average_ranks:
        print(f"{name}:{rank:.2f}")
    

if __name__ == '__main__':
    main()
        
