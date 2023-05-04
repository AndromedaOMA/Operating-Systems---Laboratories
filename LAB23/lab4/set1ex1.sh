# 1^3 + 2^3 +...+ n^3

#!/bin/bash
k=0
result=0
read k

for ((i = 1; i <= k; i++))
do
	((result += i**3))
done

echo $result


