#!/bin/bash

A=$1
B=$2

CMMDC()
{
    if [ $A -eq 0 ] || [ $B -eq 0 ]; then
        echo "Nu exista C.M.M.D.C.-ul numerelor $A si $B!"
        return
    fi
    if [ $(( $1%$2 )) -eq 0 ]; then
        echo "C.M.M.D.C.-ul numerelor $A si $B este: $2"
    else
        CMMDC $2 $(( $1%$2 ))
    fi
}

CMMDC $1 $2