#!/bin/bash

files=result/final/*

for file in $files
do
    name="$(cut -d'.' -f1 <<< $file)"
    token="$(cut -d'_' -f2 <<< $name)"

    temp=$name"_temp.txt"

    pattern=$token'\.[^1-9]*'

    cat $file | grep $pattern > $temp

    first_line="$(head -n 1 $temp)"

    lemma="$(cut -d'.' -f1 <<< $first_line)"

    pos="$(cut -d'.' -f2 <<< $first_line)"

    filename="$lemma.$pos.txt"

    # echo $filename

    cat $temp > result/final_senses/$filename

    echo "Process $file complete..."
done





