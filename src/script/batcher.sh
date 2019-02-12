#!/bin/bash

dir=$1
exec_name='./maxcut-benchmark'
config_file='./config.json'

echo $2
install -d $2

for file in `find -L $dir -type f`
do
    echo "${exec_name} ${file} ${config_file} > ${2}/${file}"
    echo "${2}/${file}"
    install -Dv -m 644 /dev/null "${2}/${file}"
	eval "${exec_name} ${file} ${config_file} > ${2}/${file}"
done
