#!/bin/bash

name=`basename $1 .json`

./script/result_processing/rank_avg_fitness.py $1 | ./kv_to_latex_table.sh > ${name}_rank.tex
