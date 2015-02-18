#!/bin/bash

SSH_USER="i0912324@olivo.usal.es"

if [ "$1" == "" ]; then
	echo "Usage: $0 <sql_file> [<results_file>=<sql_file>.results]";
	exit;
fi

SOURCE_FILE="$1";
if [ "$2" == "" ];then
	DEST_FILE="$SOURCE_FILE.results"
else
	DEST_FILE="$2";
fi

if [ ! -f "$SOURCE_FILE" ]; then
	echo "ERROR: $SOURCE_FILE must exists";
	exit;
fi

# Put $SOURCE_FILE in __tmp
scp "$SOURCE_FILE" "$SSH_USER:__tmp"

ssh "$SSH_USER" << EOF
	echo "SPOOL spool;" > __tmp.sql
	cat __tmp >> __tmp.sql
	echo "SPOOL OFF;" >> __tmp.sql
	echo "EXIT" >> __tmp.sql

	rm __tmp
	sqlplus / @__tmp.sql
	rm __tmp.sql
	if [ ! -f "spool.lst" ];then echo "Something went wrong..."; fi
EOF

scp "$SSH_USER:spool.lst" "$DEST_FILE"
