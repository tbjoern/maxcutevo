#!/bin/bash

fsoc_processor_count=80
instance_dir=$1
config=$2
cpus=$3
nodes=$4
name=`basename $config .json`
((tasks = fsoc_processor_count / cpus))

sbatch --output $name.out --nodes $nodes --cpus-per-task $cpus --job-name mx-$name --ntasks-per-node $tasks ../src/script/fsoc.sh $instance_dir $config

