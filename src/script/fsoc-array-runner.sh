#!/bin/bash

instancedir=$1
outputdir=$2
configfile=$3

configcount=$(./configscript.py --count $configfile)
instances=$(find $instancedir -type f)
let "jobs = ${configcount}*${#instances[@]} - 1"

sbatch --array=0-${jobs} -N=1-22 ./fsoc-mqlib-array.sh $instancedir $outputdir $configfile

