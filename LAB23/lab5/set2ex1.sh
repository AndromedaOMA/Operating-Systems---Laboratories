#!/bin/bash

# Find all files in the current directory and its subdirectories
for file in $(find . -type f); do
    # Determine the file type
    # -b surpresses the filename from the output so it only contains the file type
    file_type=$(file -b "$file")

    # Check if the file is a bash script
    if [[ $file_type == *"Bourne-Again shell script"* ]]; then
        # Change permissions
        chmod a+x "$file"
        echo "$file is now executable for all users"
    fi
done
