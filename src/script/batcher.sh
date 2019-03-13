#!/bin/bash

dir=$1
exec_name='python3 ../src/python/batch.py'
config_file=$3

echo $2
install -d $2

for file in `find -L $dir -type f`
do
    ext=${file##*.}
    fname=`basename $file .$ext`
    fdir=`dirname $file`
    logfile="$fdir/$fname.csv"
    cmd="${exec_name} --stdout ${config_file} ${file} > ${2}/${logfile}"
    echo $cmd
    install -D -m 644 /dev/null "${2}/${logfile}"
	eval $cmd
done
