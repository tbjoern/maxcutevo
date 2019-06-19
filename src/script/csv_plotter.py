#! /usr/bin/env python3
import matplotlib.pyplot as plt
import csv

if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument("csv")

    args = parser.parse_args()

    data = {}

    with open(args.csv, 'r') as csvfile:
        reader = csv.DictReader(csvfile, delimiter=',')
        group_by = input("Select field to group by: " + ", ".join(f"{field} {i}" for i,field in enumerate(reader.fieldnames)) + '\n--> ')
        group_by = reader.fieldnames[int(group_by)]
        label_source = input("Select field for x axis: " + ", ".join(f"{field} {i}" for i,field in enumerate(reader.fieldnames)) + '\n--> ')
        label_source = reader.fieldnames[int(label_source)]
        data_source = input("Select field for y axis: " + ", ".join(f"{field} {i}" for i,field in enumerate(reader.fieldnames)) + '\n--> ')
        data_source = reader.fieldnames[int(data_source)]
        for row in reader:
            group = row[group_by]
            if not group in data:
                data[group] = []
            data[group].append((row[label_source], row[data_source]))


    for key in sorted(data.keys()):
        d = data[key]
        x = [p[0] for p in d]
        y = [p[1] for p in d]
        plt.plot(x,y,'bo')
        plt.title(key)
        plt.show()
            
