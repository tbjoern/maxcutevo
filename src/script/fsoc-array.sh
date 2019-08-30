#!/bin/bash
#SBATCH --job-name=mx-array
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=8000
#SBATCH --ntasks=1

instancedir=$1
outputdir=$2
configfile=$3

configcount=$(./configscripy.py --count $configfile)
instances=$(find $instancedir -type f)
let "instancenum = ${SLURM_ARRAY_TASK_ID}/${configcount}"
instance=${instances[${instancenum}]}
config=$(./configscript.py --make ${SLURM_ARRAY_TASK_ID} $configfile)

basename=${instance%.*}
echo ${basename##*/}
outputfile="${outputdir}/${basename}-${config}.out"
if [ ! -f $outputfile ]
then
    install -D /dev/null $outputfile
    ./maxcut-benchmark $f $config > $outputfile 
fi

rm $config

