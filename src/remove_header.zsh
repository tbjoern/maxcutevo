#!/bin/zsh

for file in *.mtx; do 
    echo $file;
    tail -n +3 $file | sponge $file
done
