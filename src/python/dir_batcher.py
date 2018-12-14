import sys
from pathlib import Path
from multiprocessing import Process, Queue, current_process, freeze_support
from batch import Batch
import logging

def worker(input, output):
    for batch, filename in iter(input.get, 'STOP'):
        batch.run(filename)
        output.put(filename)

def run_batch(batch, filename):
    batch.run(filename)

def main():
    global logger
    logger = logging.getLogger("dir_batcher.py")
    logger.setLevel(logging.INFO)
    ch = logging.StreamHandler()
    ch.setLevel(logging.INFO)
    logger.addHandler(ch)

    if len(sys.argv) != 3:
        logger.error("Usage: dir_batcher.py <config.ini> <instance_dir>")

    from config_reader import read_config
    try:
        config = read_config(sys.argv[1])
    except:
        logger.exception("Error while reading config file - aborting")
        exit(1)

    instance_dir = Path(sys.argv[2])
    if not instance_dir.exists():
        logger.error('Path "{}" does not exist - aborting'.format(sys.argv[2]))
        exit(1)

    batch = Batch(config)
    batches = Queue()
    CPU_COUNT = config['cpu_count']
    finished_queue = Queue()

    for filename in instance_dir.glob('**/*'):
        if not filename.is_dir():
            batches.put((batch, filename))

    batch_count = batches.qsize()
    for _ in range(CPU_COUNT):
        Process(target=worker, args=(batches, finished_queue)).start()

    for _ in range(batch_count):
        print("finished instance {}".format(finished_queue.get()))

    for _ in range(CPU_COUNT):
        batches.put('STOP')

if __name__ == "__main__":
    freeze_support()
    main()

