#!/bin/bash

instancedir=$1
outputdir=$2
configfile=$3
slurmoutput=slurm-output

if [ -d $slurmoutput ]
then
	echo "Slurm dir not empty"
	exit 1
fi

if [ -d $outputdir ]
then
	echo "Output dir not empty"
	exit 1
fi

mkdir $slurmoutput
mkdir $outputdir

configcount=$(./configscript.py --count $configfile)
instances=($(find -L $instancedir -type f | grep -v '.assignment'))
let "jobs = ${configcount}*${#instances[@]} - 1"

sbatch -o "${slurmoutput}/slurm-%A_%a.out" --array=0-${jobs} --nodes=1-22 ./fsoc-array.sh $instancedir $outputdir $configfile

