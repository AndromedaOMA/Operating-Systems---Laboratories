#!/bin/bash

#-x=1 -w=2 -r=4

touch Test.txt

chmod 767 Test.txt

cat Test.txt | cut -d : -f 1 | sort #pentru afisarea in ordine cresc a val de pe col 2
