#!/bin/bash
LOG="/var/log/apache2/access.log"

if [ "$1" != "" ]; then
	LOG="$1";
fi

echo "Apache log analyzer"

if [ ! -f "$LOG" ];then
	echo "ERROR: Log file not found";
	exit;
fi

echo "Access count: "
cut -d " " -f 1 "$LOG" | sort | uniq | wc -l

echo "Dates: "
lines=`wc -l "$LOG"`
case $lines in
	0) echo "No dates available";;
	1) date=`cut -d " " -f 4 "$LOG"`; echo "One date: $date";;
	*) date_1=`head -1 "$LOG" | cut -d " " -f 4`; date_2=`tail -1 "$LOG" | cut -d " " -f 4`;
		echo "Dates from $date_1 to $date_2";;
esac



