#!/bin/bash
folder="$1";
if [ "$folder" == "" ]; then
	folder=".";
fi

if [ ! -d "$folder" ]; then
	echo "Debes introducir un directorio válido.";
	exit;
fi

ls -t -1 $folder;
