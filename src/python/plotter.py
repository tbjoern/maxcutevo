import csv
import matplotlib.pyplot as plt

class Plotter:
    def __init__(self, csv_name, mapping_name=None):
        self.colors = ['b','g','r','m','c','y','k']
        self.data = {}
        self.labels = {}
        if mapping_name is None:
            mapping_name = csv_name + '_mapping'
        with open(mapping_name, "r") as f:
            csvreader = csv.DictReader(f, delimiter=',')
            for row in csvreader:
                algo_id = int(row["id"])
                self.labels[algo_id] = row["name"]
                self.data[algo_id] = { 'iteration':[], 'cut_weight':[] }
        with open(csv_name, "r") as f:
            csvreader = csv.DictReader(f, delimiter=',')
            for row in csvreader:
                algo_id = int(row["algorithm"])
                self.data[algo_id]['iteration'].append(int(row['iteration']))
                self.data[algo_id]['cut_weight'].append(int(row['cut_weight']))

    def prepare_plot(self):
        for algo_id, data in self.data.items():
            plt.plot(data['iteration'], data['cut_weight'], self.colors[algo_id%len(self.colors)] + '-', label=self.labels[algo_id])
        plt.legend(loc='lower right')

    def show_plot(self):
        plt.show()


if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("Usage: plotter.py <csv file> <mapping file>")
    csv_filename = sys.argv[1]
    if len(sys.argv) == 3:
        mapping_filename = sys.argv[2]
    else:
        mapping_filename = None

    plotter = Plotter(csv_filename, mapping_filename)
    plotter.prepare_plot()
    plotter.show_plot()

