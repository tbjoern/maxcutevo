#!/bin/bash

fsoc_processor_count=80
instance_dir=$1
config=$2
cpus=$3
name=`basename $config .json`
((tasks = fsoc_processor_count / cpus))

sbatch --cpus-per-task $cpus --job-name mxct-$name --ntasks $tasks ../src/script/fsoc.sh $instance_dir $config

