#!/bin/bash

#1
currentDir="."
if [ $# -eq 0 ]; then
	exit 2
else
	currentDir=$1
fi

#2
gcc -Wall filetypeandids.c -o filetypeandids

#3
./filetypeandids $1
exitCode=$?
echo
echo "Exit code: $exitCode"



if [[ -f $file_name ]]; then
    chmod 006 "$file_name"
    echo "$file_name este acum executabil si scriere pentru others"
else
    #4
    if [[ -d $file_name ]]; then

    fi