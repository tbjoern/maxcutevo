#!/bin/bash

#SBATCH --ntasks 800
#SBATCH --output slurm.out

exec_name=$1
config_file=$2
dir=$3

shopt -s globstar

for file in $dir/**/*.*
do
	srun python3 $exec_name $config_file $file &
done
