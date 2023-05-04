#!/bin/bash

nr=1
val=1

putere()
{
    if [ $nr -eq $1 ]; then
        echo "Valoarea cautata este: " $val
    else
        let nr+=1
        let val*=3
        putere $1
    fi
}

putere $1