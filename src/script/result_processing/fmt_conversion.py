#!/usr/bin/env python3
import json
import sys

def algo_to_inst(data):
    inst = {}
    for algo, algodata in data.items():
        for instance, value in algodata.items():
            if instance not in inst:
                inst[instance] = {}
            inst[instance][algo] = value
    return inst

def inst_to_algo(data):
    algod = {}
    for instance, idata in data.items():
        for algo, value in idata.items():
            if algo not in algod:
                algod[algo] = {}
            algod[algo][instance] = value
    return algod

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('json')
    parser.add_argument('-i' , '--inputfmt', choices=['ALGO', 'INST'], required=True)
    parser.add_argument('-o', '--outputfmt', choices=['ALGO', 'INST'], required=True)
    args = parser.parse_args()

    with open(args.json, 'r') as f:
        data = json.load(f)

    converted = None
    if args.inputfmt == 'ALGO' and args.outputfmt == 'INST':
        converted = algo_to_inst(data)
    elif args.inputfmt == 'INST' and args.outputfmt == 'ALGO':
        converted = inst_to_algo(data)

    if converted is None:
        print("Unknown conversion")
        exit(1)

    json.dump(converted, sys.stdout)

if __name__ == "__main__":
    main()
