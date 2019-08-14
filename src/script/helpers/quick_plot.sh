#!/bin/bash

name=`basename $1 .json`

./script/result_processing/plot_avg_fitness.py --image $name.png $1
