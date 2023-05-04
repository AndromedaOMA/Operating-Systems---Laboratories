#!/bin/bash

src=$1
dest=$2
file=$3

transfer()
{
    if [ $1 = $2 ]; then
        echo "Directorul sursa si directorul destinatie sunt aceiasi!";
        return
    fi
    for x in $(ls $1); do
        if [ -d "$1/$x" ]; then
            echo "$x este un dir"
            if [ "$1/$x" = $4 ]; then
                echo "Directorul sursa contine directorul destinatie!"
                return 
            else
                if [ ! -d "$2/$x" ]; then
                    mkdir "$2/$x"
                fi
                transfer "$1/$x" "$2/$x" $3 $4
            fi
        else
            #aici este un fisier
            extensie=$(echo $x | cut -d'.' -f2)
            echo $extensie
            if cat $file | grep -q $extensie ; then
                if [ $(du -b "$1/$x" | cut -f 1) -le 1024 ]; then
                    echo "copiaza $1/$x"
                    cp "$1/$x" "$2/$x"
                fi
            fi
        fi
    done
}

transfer $src $dest $file $dest
