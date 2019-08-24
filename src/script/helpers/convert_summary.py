#!/usr/bin/env python3

import os
import json
import sys

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('file')
    args = parser.parse_args()

    with open(args.file, 'r') as f:
        data = json.load(f)

    new_data = {}
    for k,v in data.items():
        new_data[k] = {}
        for a, b in v.items():
            new_data[k][a] = {}
            for c, d in b.items():
                if isinstance(d, dict):
                    new_data[k][a][c] = list(d.values())
                else:
                    new_data[k][a][c] = d

    json.dump(new_data, sys.stdout)

if __name__ == "__main__":
    main()
