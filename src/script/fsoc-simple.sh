#!/bin/bash

instance_dir=$1
output_dir=$2
cpus=$3

sbatch --cpus-per-task $3 --job-name mxct-$2 ../src/script/fsoc.sh $1 $2 $2/*.json

