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
    logger.error(x)
    logger.error("Error while reading config file - aborting")
    exit(1)

instance_dir = Path(sys.argv[2])
if not instance_dir.exists():
    logger.error('Path "{}" does not exist - aborting'.format(sys.argv[2]))
    exit(1)

batch = Batch(config)
processes = []

def run_batch(batch, filename):
    batch.run(filename)

for filename in instance_dir.glob('**/*'):
    p = Process(target=run_batch, args=(batch, filename, ))
    processes.append(p)

logger.info("Starting batch processes:")
for i,p in enumerate(processes):
    p.start()
    logger.info("start batch {}".format(i))

logger.info("waiting for batches to finish")

for i,p in enumerate(processes):
    p.join()
    logger.info("joined batch {}".format(i))
