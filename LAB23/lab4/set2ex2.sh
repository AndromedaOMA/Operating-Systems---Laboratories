#fibonacci

#!/bin/bash
n=0
n1=0
n2=0
n3=0
n4=1

if [ $# -eq 0 ]; then
	read n
else
	((n = $1))
fi

fibonacci()
{
	if [ $1 -le 2 ]
	then
		echo 1
	else
		echo $(( $(fibonacci $(($1 - 1)) ) + $(fibonacci $(($1 -2)) ) ))
	fi
}



fibonacci $n


