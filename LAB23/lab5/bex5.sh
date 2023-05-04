#!/bin/bash

# If the current directory wasn't passed as an argument, read it
currentDir="."
if [ $# -eq 0 ]; then
	read currentDir
else
	currentDir=$1
fi

doStuff() {
    for file in "$currentDir"/*; do
        echo $file
        # Determine the file type
        # -b surpresses the filename from the output so it only contains the file type
        file_type=$(file -b "$file")

        # If script file use zip
        if [[ $file_type == *"Bourne-Again shell script"* ]]; then
            zip "$currentDir"$(basename -s ".sh" "$file") "$file"
        else
            # If text file use gzip
            if [[ $file_type == *"ASCII text"* ]]; then
                gzip -k "$file"
            fi
        fi
    done
}

doStuff
