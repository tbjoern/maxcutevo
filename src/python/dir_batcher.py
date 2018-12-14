import sys
from pathlib import Path
from multiprocessing import Process
from batch import Batch
import logging

logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)
ch = logging.StreamHandler()
ch.setLevel(logging.INFO)
logger.addHandler(ch)

if len(sys.argv) != 3:
    logger.error("Usage: dir_batcher.py <config.ini> <instance_dir>")

from config_reader import read_config
try:
    config = read_config(sys.argv[1])
except Exception as x:
    logger.exception("Error while reading config file - aborting")
    exit(1)

instance_dir = Path(sys.argv[2])
if not instance_dir.exists():
    logger.error('Path "{}" does not exist - aborting'.format(sys.argv[2]))
    exit(1)

batch = Batch(config)
processes = []
CPU_COUNT = config['cpu_count']

def run_batch(batch, filename):
    batch.run(filename)

for filename in instance_dir.glob('**/*'):
    if not filename.is_dir():
        p = Process(target=run_batch, args=(batch, filename, ))
        processes.append(p)

if CPU_COUNT == 0:
    logger.info("Starting all batch processes:")
    for i,p in enumerate(processes):
        p.start()
        logger.info("start batch {}".format(i))

    logger.info("waiting for batches to finish")

    for i,p in enumerate(processes):
        p.join()
        logger.info("joined batch {}".format(i))
else:
    logger.info("Running batches with cpu_count {}:".format(CPU_COUNT))
    p_nr = 0
    running_processes = []
    for i,p in enumerate(processes):
        p.start()
        p_nr += 1
        logger.info("start batch {}".format(p_nr))
        running_processes.append((p,p_nr))
        if len(running_processes) == CPU_COUNT:
            for p,nr in running_processes:
                p.join()
                logger.info("joined batch {}".format(nr))
            running_processes = []

