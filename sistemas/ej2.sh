#!/bin/bash
file="$1";
dir="$2"

if [ ! -f "$file" ]; then
	echo "El primer argumento no es un archivo";
	exit;
fi

first=`echo $file | head -c 1`;

if [ "$first" != "/" ]; then
	file="$PWD/$file";
fi

ls $file > ubicacion.txt;

if [ "$dir" == "" ]; then
	echo "No se ha especificado directorio.";
fi

if [ ! -d "$dir" ];then
	mkdir "$dir";
fi

mv ubicacion.txt $dir/ubicacion.txt
cat $dir/ubicacion.txt
