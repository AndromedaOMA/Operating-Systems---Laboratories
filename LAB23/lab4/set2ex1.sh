#isPrime

#!/bin/bash
n=0
p=0
read n
read p

isPrime()
{
	if (( p==1 || (p > 2 && p%2==0) )); then
		echo 0
	else
		let max=p/3
		for (( d=3; d <= max; d++)); do
			if (( p%d == 0 )); then
				echo 0
			fi
		done
	fi
	echo 1
}

if [ $(isPrime) -eq 1 ]; then
	x=0
	sum=0
	for ((i = 1; i <= n; i++)); do
		read x
		if (( x%p == 0 )); then
			((sum += x**3))
		fi
	done
else
	echo "p nu este prim"
fi

echo $sum
