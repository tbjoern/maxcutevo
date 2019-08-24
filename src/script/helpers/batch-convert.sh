#!/bin/bash
set -e

for d in $(ls | grep "1M_10m")
do
    if [ -d $d ]
    then
        ./script/helpers/convert_summary.py $d/summary_clean.json > $d/summary_converted.json
    fi
done
