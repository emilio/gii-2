#!/bin/bash

if [ "$1" == "" -o "$2" == "" ]; then
	echo "No has especificado suficientes argumentos";
fi

for i in `seq $1 $2`; do
	if [ `expr $i % 2` -eq 0 ];then
		echo "$i";
	fi
done
