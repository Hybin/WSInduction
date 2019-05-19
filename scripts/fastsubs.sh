#!/bin/bash

files=input/*

for file in $files
do
    echo "Processing $file ..."
    ./fastsubs -n 100 -p 1.0 model.lm < $file >> $file.out
done

echo "Complete!"