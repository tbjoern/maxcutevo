import csv
import matplotlib.pyplot as plt
import numpy as np

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
                self.data[algo_id] = {}
        with open(csv_name, "r") as f:
            csvreader = csv.DictReader(f, delimiter=',')
            for row in csvreader:
                algo_id = int(row["algorithm"])
                run_nr = int(row["run_number"])
                if not run_nr in self.data[algo_id]:
                    self.data[algo_id][run_nr] = { 'iteration':[], 'cut_weight':[] }
                self.data[algo_id][run_nr]['iteration'].append(int(row['iteration']))
                self.data[algo_id][run_nr]['cut_weight'].append(int(row['cut_weight']))

    def prepare_plot(self):
        for algo_id, runs in self.data.items():
            data_array = []
            for run_nr, data in runs.items():
                data_array.append(data['cut_weight'])
            nparray = np.array(data_array)
            cut_weight_mean = nparray.mean(axis=0)
            sigma = nparray.std(axis=0)
            color = self.colors[algo_id%len(self.colors)]
            fmt = color + '-'
            plt.plot(data['iteration'], cut_weight_mean, fmt, label=self.labels[algo_id])
            plt.fill_between(data['iteration'], cut_weight_mean+sigma, cut_weight_mean-sigma, facecolor=color, alpha=0.5)
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

