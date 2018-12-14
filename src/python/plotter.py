import csv
import matplotlib.pyplot as plt
import numpy as np
import os
import cProfile
from pathlib import Path
from argparse import ArgumentParser

class Plotter:
    def __init__(self):
        self.colors = ['b','g','r','m','c','y','k']
        self.figure = 0
        self.plot_data = []
        self.matfigure = plt.figure()

    def add_csv(self, csv_name, mapping_name=None):
        self.plot_data.append({
            'data': {},
            'labels': {},
            'file': os.path.basename(csv_name)
        })
        self.figure = len(self.plot_data) - 1
        # read and store data
        if mapping_name is None:
            mapping_name = os.path.splitext(csv_name)[0] + '_mapping.csv'
        # read algorithm mapping
        self.read_algorithm_mapping(mapping_name)
        
        self.read_csv_data(csv_name)
    
    def read_algorithm_mapping(self, mapping_name):
        data = self.plot_data[self.figure]['data']
        labels = self.plot_data[self.figure]['labels']
        with open(mapping_name, "r") as f:
            csvreader = csv.DictReader(f, delimiter=',')
            for row in csvreader:
                algo_id = int(row["id"])
                labels[algo_id] = row["name"]
                data[algo_id] = {}

    def read_csv_data(self, csv_name):
        data = self.plot_data[self.figure]['data']
        with open(csv_name, "r") as f:
            csvreader = csv.DictReader(f, delimiter=',')
            for row in csvreader:
                algo_id = int(row["algorithm"])
                run_nr = int(row["run_number"])
                if not run_nr in data[algo_id]:
                    data[algo_id][run_nr] = { 'iteration':[], 'cut_weight':[] }
                data[algo_id][run_nr]['iteration'].append(int(row['iteration']))
                data[algo_id][run_nr]['cut_weight'].append(int(row['cut_weight']))

    def plot_current_figure(self):
        data = self.plot_data[self.figure]['data']
        labels = self.plot_data[self.figure]['labels']
        file = self.plot_data[self.figure]['file']
        for algo_id, runs in data.items():
            data_array = []
            for run_nr, run_data in runs.items():
                data_array.append(run_data['cut_weight'])
            nparray = np.array(data_array)
            cut_weight_mean = nparray.mean(axis=0)
            sigma = nparray.std(axis=0)
            color = self.colors[algo_id%len(self.colors)]
            fmt = color + '-'
            plt.plot(runs[0]['iteration'], cut_weight_mean, fmt, label=labels[algo_id])
            plt.fill_between(runs[0]['iteration'], cut_weight_mean+sigma, cut_weight_mean-sigma, facecolor=color, alpha=0.5)
        plt.legend(loc='lower right')
        plt.suptitle(file)

    def show_plot(self):
        self.plot_current_figure()
        plt.show()

    def next_figure(self):
        if self.figure == len(self.plot_data) - 1:
            self.figure = 0
        else:
            self.figure += 1

    def previous_figure(self):
        if self.figure == 0:
            self.figure = len(self.plot_data) - 1
        else:
            self.figure -= 1

def onclick(event):
    global plotter
    event.canvas.figure.clear()

    if event.button == 1:
        plotter.next_figure()
    else:
        plotter.previous_figure()
    plotter.plot_current_figure()

    event.canvas.draw()

def main():
    global plotter
    parser = ArgumentParser()
    parser.add_argument('--dir', action='store_true', help='treats csv_path as a directory, creates a plot for every csv file in the directory')
    parser.add_argument('csv_path', action='store', help='path of the csv file')
    cfg = parser.parse_args()

    plotter = Plotter()

    plotter.matfigure.canvas.mpl_connect('button_press_event', onclick)

    print("Loading csv..")
    if not cfg.dir:
        plotter.add_csv(cfg.csv_path)
    else:
        csv_dir = Path(os.path.abspath(cfg.csv_path))
        filenames = sorted(list(csv_dir.glob('**/*.csv')))
        for filename in filenames:
            if not 'mapping' in str(filename):
                print(filename)
                plotter.add_csv(filename)
    print("Plotting")
    plotter.show_plot()

if __name__ == "__main__":
    main()
