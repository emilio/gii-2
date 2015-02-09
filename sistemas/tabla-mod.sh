#!/bin/bash
while true; do
	read -r -p "Proporciona un número: " num;
	case $num in
		[0-9]) break;;
		[0-9][0-9]) break;;
		[0-9][0-9][0-9]) break;;
	esac
done

file="tabla.$num";

echo "" > "$file"
for i in `seq 1 10`; do
	res=`expr $num \* $i`;
	res="$num * $i = $res";
	echo "$res";
	echo "$res" >> "$file";
done
