#!/bin/bash

dir=$1
exec_name='./maxcut-benchmark'
config_file='./config.json'

echo $2
install -d $2

for file in `find -L $dir -type f`
do
    ext=${file##*.}
    fname=`basename $file .$ext`
    fdir=`dirname $file`
    logfile="$fdir/$fname.csv"
    echo "${exec_name} ${file} ${config_file} > ${2}/${logfile}"
    echo "${2}/${logfile}"
    install -D -m 644 /dev/null "${2}/${logfile}"
	eval "${exec_name} ${file} ${config_file} > ${2}/${logfile}"
done
