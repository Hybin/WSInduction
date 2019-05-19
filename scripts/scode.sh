#!/bin/bash

files=pairs/*

for file in $files
do
    echo "Processing $file ..."
    ./scode < $file > $file.out
done

echo "Complete!"
