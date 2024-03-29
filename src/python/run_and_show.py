from batch import Batch
from plotter import Plotter

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 3:
        print("Usage: run_and_show.py <config_file> [instance_file, ...]")
        exit(127)
    
    from config_reader import read_config
    config = read_config(sys.argv[1])

    batch = Batch(config)
    for filename in sys.argv[2:]:
        logfile = batch.run(filename)
        if logfile is not None:
            plotter = Plotter(logfile)
            plotter.prepare_plot()
            plotter.show_plot()

