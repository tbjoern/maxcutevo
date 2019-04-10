#!/bin/bash

thread_count=4

instance_dir=$1
exec_name='./maxcut-benchmark'
config_file=$2

conf_ext=${config_file##*.}
result_dir=`basename $config_file .$conf_ext`
echo "using output directory: $result_dir"

install -d $result_dir
mv $config_file $result_dir
config_file="$result_dir/$config_file"

for file in `find -L $instance_dir -type f | grep -v .git`
do
    ext=${file##*.}
    fname=`basename $file .$ext`
    fdir=`dirname $file`
    logfile="$fdir/$fname.csv"
    cmd="OMP_NUM_THREADS=${thread_count} ${exec_name} ${file} ${config_file} > $result_dir/${logfile}"
    echo $cmd
    install -D -m 644 /dev/null "$result_dir/${logfile}"
    eval $cmd
done
