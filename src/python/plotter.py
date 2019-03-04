import csv
import matplotlib.pyplot as plt
import numpy as np
import os
import cProfile
from pathlib import Path
from argparse import ArgumentParser
import json

class Plotter:
    def __init__(self):
        self.colors = ['b','g','r','m','c','y','k']
        self.figure = 0
        self.plot_data = []
        self.matfigure = plt.figure()
        self.config = None

    def add_config_json(self, config_file):
        with open(config_file, 'r') as f:
            self.config = json.load(f)

    def add_csv(self, csv_name, mapping_name=None):
        self.plot_data.append({
            'data': {},
            'labels': {},
            'file': os.path.basename(csv_name),
            'file_path': csv_name,
            'is_loaded': False
        })
        self.figure = len(self.plot_data) - 1
        # read and store data
        if mapping_name is None:
            mapping_name = os.path.splitext(csv_name)[0] + '_mapping.csv'
        # read algorithm mapping
        self.read_algorithm_mapping(mapping_name)

    def read_algorithm_mapping(self, mapping_name):
        data = self.plot_data[self.figure]['data']
        labels = self.plot_data[self.figure]['labels']
        if self.config:
            for algorithm in self.config["algorithms"]:
                algo_id = algorithm["id"]
                labels[algo_id] = self.build_algorithm_name(algorithm)
                data[algo_id] = {}
        else:
            with open(mapping_name, "r") as f:
                csvreader = csv.DictReader(f, delimiter=',')
                for row in csvreader:
                    algo_id = int(row["id"])
                    labels[algo_id] = row["name"]
                    data[algo_id] = {}

    def build_algorithm_name(self, algorithm):
        name_parts = list()
        name_parts.append(algorithm["name"])
        if "arguments" in algorithm:
            for key,value in algorithm["arguments"].items():
                name_parts.append(f"{key}={value}")
        return " ".join(name_parts)

    def read_csv_data(self, csv_name):
        print(f'reading {csv_name}')
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
        self.plot_data[self.figure]['is_loaded'] = True

    def plot_current_figure(self):
        if not self.plot_data[self.figure]['is_loaded']:
            self.read_csv_data(self.plot_data[self.figure]['file_path'])
        data = self.plot_data[self.figure]['data']
        labels = self.plot_data[self.figure]['labels']
        file = self.plot_data[self.figure]['file']
        for algo_id, runs in data.items():
            data_array = []
            for run_nr, run_data in runs.items():
                data_array.append(run_data['cut_weight'])
            min_length = None
            for run_data in data_array:
                if min_length is None or len(run_data) < min_length:
                    min_length = len(run_data)
            for i, run_data in enumerate(data_array):
                data_array[i] = run_data[:min_length]
            nparray = np.array(data_array)
            cut_weight_mean = nparray.mean(axis=0)
            sigma = nparray.std(axis=0)
            color = self.colors[algo_id%len(self.colors)]
            fmt = color + '-'
            indices = np.arange(min_length)
            plt.plot(indices, cut_weight_mean, fmt, label=labels[algo_id])
            plt.fill_between(indices, cut_weight_mean+sigma, cut_weight_mean-sigma, facecolor=color, alpha=0.5)
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
    parser.add_argument('--config', '-c', action='store', help='a json config file containing algorithm ids/names under json["algorithms"][#nr]["id"] and ..["name"')
    cfg = parser.parse_args()

    plotter = Plotter()

    plotter.matfigure.canvas.mpl_connect('button_press_event', onclick)

    if cfg.config:
        plotter.add_config_json(cfg.config)

    print("Loading csv..")
    if not cfg.dir:
        plotter.add_csv(cfg.csv_path)
    else:
        csv_dir = Path(os.path.abspath(cfg.csv_path))
        filenames = sorted(list(csv_dir.glob('**/*')))
        for filename in filenames:
            if not filename.is_dir() and not 'mapping' in str(filename):
                print(filename)
                plotter.add_csv(filename)
    print("Plotting")
    plotter.show_plot()

if __name__ == "__main__":
    main()
