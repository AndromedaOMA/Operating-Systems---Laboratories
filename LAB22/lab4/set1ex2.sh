#!/bin/bash

A=$1
B=$2

CMMDC()
{
    if [ $A -eq 0 ] || [ $B -eq 0 ]; then
        echo "Nu exista C.M.M.D.C.-ul numerelor $A si $B!"
        return
    fi
    if [ $1 -eq $2 ]; then
        echo "C.M.M.D.C.-ul numerelor $A si $B este: $1"
    else
        if [ $1 -gt $2 ]; then
            CMMDC $(($1-$2)) $1 
        else
            CMMDC $1 $(($2-$1)) 
        fi
    fi
}

CMMDC $1 $2