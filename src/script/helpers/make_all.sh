#!/bin/bash

set -e

dirs=$*

for d in ${dirs[@]}
do
    if [ ! -d $d ]
    then
        echo "$d is not a directory"
        exit 1
    fi
done

for d in ${dirs[@]}
do
    echo "$d"
    echo "Ranking"
    ./script/helpers/batch.sh $d .json ./script/helpers/quick_rank.sh
    echo "Plotting"
    ./script/helpers/batch.sh $d .json ./script/helpers/quick_plot.sh
done
