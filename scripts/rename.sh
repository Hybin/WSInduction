#!/bin/bash

files=pairs/*

for file in $files
do
    filename="$(cut -d'/' -f2 <<<"$file")"
    lemma="$(cut -d'.' -f1 <<<"$filename")"
    pos="$(cut -d'.' -f2 <<<"$filename")"

    if [[ ($lemma = "book") || ($lemma = "trace") ]]
    then
        mv $file $file
    else
        mv $file "pairs/"$lemma".pair"
    fi
done
