#!/bin/bash

#
# @author Emilio Cobos <emiliocobos@usal.es>
# @license GPLv3 or greater
#

SSH_USER="i0912324@olivo.usal.es"

USE_TRANSACTION=0

if [ "$1" == "" ]; then
	echo "Usage: $0 <sql_file> [<results_file>=<sql_file>.results] [--transaction]";
	exit;
fi

SOURCE_FILE="$1";

if [ "$2" == "" ];then
	DEST_FILE="$SOURCE_FILE.results"
else
	DEST_FILE="$2";
fi

if [ "$3" == "--transaction" ]; then
	USE_TRANSACTION=1
fi

if [ ! -f "$SOURCE_FILE" ];then
	echo "ERROR: $SOURCE_FILE must exist";
	exit;
fi

# Put $SOURCE_FILE in __tmp
scp "$SOURCE_FILE" "$SSH_USER:__tmp"

BEFORE_CONTENT="";
AFTER_CONTENT="";

if [ $USE_TRANSACTION -eq 1 ];then
	AFTER_CONTENT="COMMIT;";
else
	BEFORE_CONTENT="SET AUTOCOMMIT ON;"
fi

ssh "$SSH_USER" << EOF
	if [ ! -f "__tmp" ]; then echo "Expected temporary file __tmp"; exit; fi

	echo "SPOOL spool;" > __tmp.sql
	echo "$BEFORE_CONTENT" >> __tmp.sql
	cat __tmp >> __tmp.sql
	echo "$AFTER_CONTENT" >> __tmp.sql
	echo "SPOOL OFF;" >> __tmp.sql
	echo "EXIT;" >> __tmp.sql

	rm __tmp
	sqlplus / @__tmp.sql
	rm __tmp.sql
	if [ ! -f "spool.lst" ];then echo "Something went wrong..."; fi
EOF

scp "$SSH_USER:spool.lst" "$DEST_FILE"
