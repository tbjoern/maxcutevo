#! /bin/bash
#SBATCH --job-name=maxcut-benchmark
#SBATCH --ntasks-per-node=12
#SBATCH --cpus-per-task=4
#SBATCH --mem-per-cpu=4000

instance_dir=$1
exec_name='./maxcut-benchmark'
config_file=$2

conf_ext=${config_file##*.}
result_dir=`basename $config_file .$conf_ext`
echo "using output directory: $result_dir"

install -d $result_dir
mv $config_file $result_dir
config_basename=`basename $config_file`
config_file="$result_dir/$config_basename"

for file in `find -L $instance_dir -type f | grep -v .git | grep -v .assignment`
do
    ext=${file##*.}
    fname=`basename $file .$ext`
    fdir=`dirname $file`
    logfile="$fdir/$fname.csv"
    echo "${exec_name} ${file} ${config_file}  > $result_dir/${logfile}"
    echo "$result_dir/${logfile}"
    install -D -m 644 /dev/null "$result_dir/${logfile}"
    srun -N1 -n1 --output "$result_dir/${logfile}" ${exec_name} ${file} ${config_file} &
done

wait
