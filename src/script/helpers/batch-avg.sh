#!/bin/bash

folder_groups="undir_empty undir_full undir_random"
select=$*

for i in ${select[@]}
do
    mkdir ${i%.*}
done

for f in ${folder_groups[@]}
do
    ls_out=$(ls | grep _$f)
    dirs=""
    for d in ${ls_out[@]}
    do
        if [ -d $d ]
        then
            dirs="$dirs $d"
        fi
    done
    echo $dirs
    for i in ${select[@]}
    do
        ./script/result_processing/avg_fitness.py -i $i $dirs > ${i%.*}/$f.json
    done
    echo "-----"
done
