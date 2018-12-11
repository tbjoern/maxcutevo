#!/bin/bash

#SBATCH --ntasks 800
#SBATCH --output slurm.out

dir=$1
exec_name='src/python/batch.py'
config_file='config.ini'

shopt -s globstar

for file in $dir/**/*.*
do
	srun python3 $exec_name $config_file $file &
done
