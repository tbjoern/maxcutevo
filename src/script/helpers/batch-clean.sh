#!/bin/bash

for d in $(ls | grep "1M_10m")
do
    if [ -d $d ]
    then
        ./clean_summary.py $d/summary.json > $d/summary_clean.json
    fi
done
