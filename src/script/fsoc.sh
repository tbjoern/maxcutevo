#!/bin/bash

#SBATCH --job-name=maxcut-benchmark
#SBATCH --ntasks=20
#SBATCH --cpus-per-task=4
#SBATCH --output=slurm.out
#SBATCH --mem-per-cpu=1000

instance_dir=$1
exec_name='python3 ../src/python/batch.py'
config_file=$2

conf_ext=${config_file##*.}
result_dir=`basename $config_file .$conf_ext`
echo "using output directory: $result_dir"


install -d $result_dir
mv $config_file $result_dir

for file in `find -L $dir -type f | grep -v .git`
do
    ext=${file##*.}
    fname=`basename $file .$ext`
    fdir=`dirname $file`
    logfile="$fdir/$fname.csv"
    echo "${exec_name} --stdout ${config_file} ${file}  > $result_dir/${logfile}"
    echo "$result_dir/${logfile}"
    install -D -m 644 /dev/null "$result_dir/${logfile}"
    srun -n1 --exclusive --output "$result_dir/${logfile}" ${exec_name} --stdout ${config_file} ${file} &
done

wait
