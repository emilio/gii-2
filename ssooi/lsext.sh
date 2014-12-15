#!/bin/bash

#
# @author Emilio Cobos Álvarez <emiliocobos@usal.es>
#

PWD=`pwd`;
SELF="$PWD/lsext.sh"


# This vars are reinitialized in get_extensions
# global var with list of extensions
# populated by `get_extensions`
EXT="";

# files without extension
FILES_WITHOUT_EXTENSION=();

# directories
DIRECTORIES=();

# Get if an extension has already been loaded
# Eg: has_extension ".txt" returns true if ".txt" is in $EXT
# Returns 0 on false and 1 on true
function has_extension() {
	local ext=$1;
	local added=`echo $EXT | grep $ext`;

	if [ "$added" == "" ]; then
		return 0;
	fi

	return 1;
}

# Returns true if the lower version of $1 is in $EXT
function has_lower_extension() {
	local lower=`echo $1 | tr '[:upper:]' '[:lower:]'`;

	has_extension "$lower"

	return $?
}

function has_upper_extension() {
	local upper=`echo $1 | tr '[:lower:]' '[:upper:]'`;

	has_extension "$upper"
	return $?
}

# returns true if $1 == upper($1)
function is_upper_extension() {
	local upper=`echo $1 | tr '[:lower:]' '[:upper:]'`;

	if [ "$1" == "$upper" ]; then
		return 1;
	fi
	
	return 0
}

function get_extensions() {
	local file;
	local ext;
	local added;
	
	# To allow recursivity we must initialize some globals here
	EXT="";
	DIRECTORIES=();
	FILES_WITHOUT_EXTENSION=();


	for file in *; do
		if [ -d "$file" ]; then
			DIRECTORIES+=("$file")
		elif [ -f "$file" ]; then
			ext=${file##*.}
			# "$ext" != "$file" checks for files without extension
			if [ "$ext" != "$file" -a "${#ext}" -gt 0 -a "${#ext}" -lt 5 ];then
				has_extension ".$ext"
				if [ $? -eq 0 ]; then
					EXT+=" .$ext";
				fi
			else
				FILES_WITHOUT_EXTENSION+=("$file")
			fi
		fi
	done
}


# prints $1 with custom format
function heading() {
	echo -e "\e[1;32m$1\e[0m"
}

function extension_heading() {
	echo -e "\e[32m$1\e[0m"
}

function warn() {
	echo -e "\e[33m$1\e[0m"
}

function main() {
	local ext;
	local nocase=0;
	local recursive=0;
	local ls_options;
	local upper;
	local files_with_extension;
	local current_dir=`pwd`;

	if [ "$1" == "--help" ]; then
		echo "Forma de uso: $SELF [opciones] [fichero(s) o directorio(s)]"
		echo "  Lista ficheros clasificados por su extensión"
		echo "Opciones:"
		echo "  Todas las de ls (ver \`ls --help\`)"
		echo "  --nocase: No distinguir mayúsculas y minúsculas"
		return 1;
	fi


	# Right now -R must be used as different option, not something like -laR
	for opt in $*; do
		if [ "$opt" == "--nocase" ];then
			nocase=1;
		elif [ "$opt" == "-R" ]; then
			recursive=1;
		else
			ls_options+=" $opt";
		fi
	done

	get_extensions

	heading "Directory: $current_dir"

	# When --nocase is disabled we simplify it
	if [ $nocase -eq 0 ]; then
		for ext in $EXT; do
			extension_heading "Extension: \"$ext\"";
			ls$ls_options *$ext
		done
	else
		for ext in $EXT; do
			is_upper_extension "$ext"
			if [ $? -eq 1 ]; then
				has_lower_extension "$ext"
				if [ $? -eq 1 ]; then
					continue; # If extension is uppercase and has lowercase skip, we'll print it with lowercase
				fi
				# If not we must print it
				extension_heading "Extension: \"$ext\"";
				ls$ls_options *$ext
			else # If it's lower we always print
				extension_heading "Extension: \"$ext\"";
				has_upper_extension "$ext"
				if [ $? -eq 1 ]; then
					upper=`echo $ext | tr '[:lower:]' '[:upper:]'`;
					ls$ls_options *$ext *$upper
				else
					ls$ls_options *$ext
				fi
			fi
		done
	fi

	extension_heading "Files without extension:"
	if [ ! "${#FILES_WITHOUT_EXTENSION[@]}" -eq 0 ]; then
		ls$ls_options "${FILES_WITHOUT_EXTENSION[@]}"
	else
		warn "No files"
	fi

	# We must print everything without extension
	if [ $recursive -eq 1 ]; then
		current=`pwd`;
		for dir in "${DIRECTORIES[@]}"; do
			cd "$current_dir/$dir"
			read
			main "$@"
		done
	fi
	
	return 0;
}

main "$@"

exit $?