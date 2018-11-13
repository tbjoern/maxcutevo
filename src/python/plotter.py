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
    if len(sys.argv) != 2:
        print("Usage: plotter.py <csv file>")
    data_points = []
    with open(sys.argv[1], "r") as f:
        csvreader = csv.DictReader(f, delimiter=',')
        for row in csvreader:
            data_points.append((int(row['iteration']), int(row['cut_weight'])))
    plt.plot([x for x in DataIt(data_points, 0)], [x for x in DataIt(data_points, 1)], 'b-')
    plt.show()

