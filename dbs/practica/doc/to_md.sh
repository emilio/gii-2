#!/bin/bash

file="$1"
filename=$(basename "$file")
extension="${filename##*.}"
srctype="$extension"
if [ "$extension" == "pc" -o "$extension" == "h" ]; then
    srctype="c"
fi

echo "### ${file:3}" # remove ../
echo "\`\`\`$srctype"
cat $1
echo "\`\`\`"
