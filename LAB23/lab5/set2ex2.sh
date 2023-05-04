#!/bin/bash

nrOfDirectories=0
count_bash=0
count_accessed=0

now=$(date +%s)
thirty_days_ago=$(date -d '30 days ago' +%s)

doStuff() {
# Loop through all files and directories in the current directory
    for file in "$1"/*; do
        # Check if the current file or directory is a directory
        if [[ -d "$file" ]]; then
            let "nrOfDirectories=nrOfDirectories+1"
            # Call the function recursively on the current directory
            doStuff "$file"
        else
            if [[ -f "$file" && $(stat -c %X "$file") -ge $thirty_days_ago ]]; then
                let "count_accessed=count_accessed+1"
            fi
            if [[ -f "$file" && $(file -b "$file") = *"Bourne-Again shell script"* ]]; then
                let "count_bash=count_bash+1"
            fi
        fi
    done
}

doStuff /home/olaru/Desktop/SO-2023/LAB23/lab5

echo "Total number of subfolders: $nrOfDirectories"
echo "Total number of bash scripts: $count_bash"
echo "Total number of accessed files in the last 30 days: $count_accessed"
