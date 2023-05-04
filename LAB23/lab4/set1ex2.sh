#tetranacci

#!/bin/bash
n=0
read n
n1=0
n2=0
n3=0
n4=1

tetranacci()
{
	if [ $1 -le 3 ]
	then
		echo 0
	elif [ $1 -eq 4 ]
	then
		echo 1
	else
		echo $(( $(tetranacci $(($1 - 1)) ) + $(tetranacci $(($1 -2)) ) + $(tetranacci $(($1 - 3)) ) + $(tetranacci $(($1 - 4)) ) ))
	fi
}



tetranacci $n


