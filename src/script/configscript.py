#!/usr/bin/env python3

import json
import sys
from math import floor

def count(config):
    return len(config['algorithms']) * run_count(config)

def run_count(config):
    return int(config['run_count'])

def read_config(filename):
    with open(filename, 'r') as f:
        data = json.load(f)
    return data

def make_config(slurmid, config):
    cfgid = floor(slurmid / count(config))
    algo_id = floor(cfgid / run_count(config))
    run_nr = cfgid % run_count(config)

    new_config = dict(config)
    new_config['run_count'] = 1
    new_config['algorithms'] = []
    for v in config['algorithms']:
        if int(v['id']) == algo_id:
            new_config['algorithms'] = [v]
            break

    configname = 'config_' + str(slurmid) + str('.json')
    with open(configname, 'w') as f:
        json.dump(new_config, f)
    sys.stdout.write(configname + '\n')

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('config')
    parser.add_argument('--make')
    parser.add_argument('--count', action='store_true')
    args = parser.parse_args()

    config = read_config(args.config)

    if args.count:
        sys.stdout.write(str(count(config)) + "\n")
    elif args.make is not None:
        make_config(int(args.make), config)

if __name__ == "__main__":
    main()
