#!/bin/bash

dir=$1
exec_name='python3 ../src/python/cnf_converter.py'

echo $2
install -d $2

for file in `find -L $dir -type f`
do
    ext=${file##*.}
    fname=`basename $file .$ext`
    fdir=`dirname $file`
    logfile="$fdir/$fname.rud"
    echo "${exec_name} $file LIG > ${2}/${logfile}"
    install -D -m 644 /dev/null "${2}/${logfile}"
    `${exec_name} $file LIG > ${2}/${logfile}`
done
