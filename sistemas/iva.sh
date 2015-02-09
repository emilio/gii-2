#!/bin/bash
if [ "$1" == "" -o "$2" == "" -o "$3" != "" ]; then
	echo "Número inválido de argumentos";
fi

price="$1";
units="$2";

total=`expr $price \* $units`;

echo "$total + $total * 0.21" | bc;
