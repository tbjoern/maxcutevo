import csv
import matplotlib.pyplot as plt

class DataIt():
    def __init__(self, data, pos):
        self.index = 0
        self.pos = pos
        self.data = data

    def __iter__(self):
        return self

    def __next__(self):
        if self.index >= len(self.data):
            raise StopIteration()
        value = self.data[self.index][self.pos]
        self.index += 1
        return value

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("Usage: plotter.py <csv file> <mapping file>")
    csv_filename = sys.argv[1]
    if len(sys.argv) == 3:
        mapping_filename = sys.argv[2]
    else:
        mapping_filename = csv_filename + '_mapping'
    data = {}
    labels = {}
    with open(mapping_filename, "r") as f:
        csvreader = csv.DictReader(f, delimiter=',')
        for row in csvreader:
            algo_id = int(row["id"])
            labels[algo_id] = row["name"]
            data[algo_id] = { 'iteration':[], 'cut_weight':[] }
    with open(csv_filename, "r") as f:
        csvreader = csv.DictReader(f, delimiter=',')
        for row in csvreader:
            algo_id = int(row["algorithm"])
            data[algo_id]['iteration'].append(int(row['iteration']))
            data[algo_id]['cut_weight'].append(int(row['cut_weight']))

    colors = ['b','g','r','m','c','y','k']

    plot_param = []

    for algo_id, data in data.items():
        plt.plot(data['iteration'], data['cut_weight'], colors[algo_id%len(colors)] + '-', label=labels[algo_id])

    plt.legend(loc='lower right')
    plt.show()

