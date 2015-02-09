#!/bin/bash
num="$1";

if [ "$num" == "" ]; then
	echo "Proporciona un número";
fi

file="tabla.$num";

echo "" > "$file"
for i in `seq 1 10`; do
	res=`expr $num \* $i`;
	res="$num * $i = $res";
	echo "$res";
	echo "$res" >> "$file";
done
