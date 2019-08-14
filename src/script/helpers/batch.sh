#!/bin/bash

dir=$1
cmd=$2
shift 2 || exit 1

for f in $(ls $dir)
do
    $cmd $dir/$f $*
done

