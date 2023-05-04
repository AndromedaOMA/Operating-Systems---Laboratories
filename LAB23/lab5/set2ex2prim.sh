#!/bin/bash

nrOfDirectories=0
count_hidden=0
count_alias=0

Count()
{
    for file in $(ls -a $1 | tail -n+3); do
        if [ -d "$file" ]; then
            let "nrOfDirectories = nrOfDirectories+1"
            Count "$file" 
        else
            #echo "count_hidden $file ddddddddddddd"
            if echo $file | grep -q "^\..*" ; then
            echo "count_hidden $file"
                let "count_hidden = count_hidden+1"
            else
                if [ -h "$file" ]; then
                    let "count_alias = count_alias+1"
                fi
            fi
        fi
    done
}

Count /home/olaru/Desktop/SO-2023/LAB23/lab5

echo "Total number of subfolders: $nrOfDirectories"
echo "Total number of hidden files: $count_hidden"
echo "Total number of alias files: $count_alias"