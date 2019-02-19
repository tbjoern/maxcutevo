#!/bin/bash

#SBATCH --job-name=maxcut-benchmark
#SBATCH --ntasks=20
#SBATCH --cpus-per-task=4
#SBATCH --output=slurm.out
#SBATCH --mem-per-cpu=1000

dir=$1
exec_name='python3 ../src/python/batch.py'
config_file='./config.json'

echo $2
install -d $2

for file in `find -L $dir -type f`
do
    ext=${file##*.}
    fname=`basename $file .$ext`
    fdir=`dirname $file`
    logfile="$fdir/$fname.csv"
    echo "${exec_name} --stdout ${config_file} ${file}  > ${2}/${logfile}"
    echo "${2}/${logfile}"
    install -D -m 644 /dev/null "${2}/${logfile}"
    srun -n1 --exclusive --output "${2}/${logfile}" ${exec_name} --stdout ${config_file} ${file} &
done

wait
