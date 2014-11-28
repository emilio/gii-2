#!/bin/bash

#------------------------------------------------------#
# ssooi.sh                                             #
#------------------------------------------------------#
#                                                      #
# @author Emilio Cobos Álvarez <emiliocobos@usal.es>   #
#                                                      #
#------------------------------------------------------#

SELF="ssooi.sh"
PASSWORD=""
CONTACTS="agenda.txt" # Better if it's a hidden file but anyways...

if [ "$HOSTNAME" == "encina" ]; then
	CONTACTS="/home/p1777031/agenda.txt"
fi

# Check if password is valid
function valid_password() {
	if [ "$1" == "$PASSWORD" ]; then
		return 1;
	fi
	return 0;
}

# Get a password, validate it and if correct exec $1
function auth_bridge() {
	local password=""

	info "Authentication required, insert password: "
	read -r -s password

	valid_password "$password"

	if [ $? -ne 1 ]; then
		error "Invalid password"
	else
		"$1"
	fi
}

# Get a menu and wait for options
# Eg: menu "Print hello" "Print bye" should return either 1 or 2
function show_menu() {
	local max=$#;
	local i=1;
	local chosen=0;

	# /dev/null to avoid warnings when user doesn't input a number
	until [ "$chosen" -gt 0 -a "$chosen" -le $max 2> /dev/null ]; do
		i=1;
		for option in "$@"; do
			echo " $i) $option";
			i=`expr $i + 1`;
		done

		read -r -p "Type an option: " chosen;
	done

	return $chosen;
}

function h1() {
	echo -e "\e[1;32m$1\e[0m$2"
}

function h2() {
	echo -e "\e[32m$1\e[0m$2"
}

function warn() {
	echo -e "\e[33m$1\e[0m$2"
}

function info() {
	echo -e "\e[34m$1\e[0m$2"
}

function error() {
	echo -e "\e[1;31m$1\e[0m$2" >&2; # < to stderr
}

function display_info() {
	local week_day="";
	h1 "Information"

	h2 "Author: " "Emilio Cobos Álvarez <emiliocobos@usal.es>"

	h2 "Group: " "B1"

	h2 "Subject: " "SSOO I"

	h2 "Username: " `whoami`

	h2 "Current date: " `date +"%d-%m-%Y %H:%M:%S"`

	case `date +%u` in
		1) week_day="Monday";;
		2) week_day="Tuesday";;
		3) week_day="Wednesday";;
		4) week_day="Thursday";;
		5) week_day="Friday";;
		6) week_day="Saturday";;
		7) week_day="Sunday";;
	esac

	h2 "Current day of the week: " "$week_day"

	h2 "Hostname: " "$HOSTNAME"

	read

}

# Prints number of contacts found
# Just convenience
function contacts_found() {
	found="$1"
	if [ $found -eq 0 ]; then
		warn "Sorry, no contacts found"
	else
		info "Found $found contacts"
	fi
}

function contacts_search() {
	q="$1"

	if [ "$q" == "-a" ]; then
		h2 "Contacts (all):"
		# cat "$CONTACTS"
		# found=`wc -l "$CONTACTS"`;

		awk '{if ( $0 != "" ) print $0}' "$CONTACTS"
		echo ""
		found=`awk 'BEGIN{i=0}{if ( $0 != "" ) i++;}END{print i}' "$CONTACTS"`

		contacts_found $found

		return
	fi


	# We make it case-insensitive
	q=`echo "$q" | tr '[:upper:]' '[:lower:]'`

	if [ ${#q} -eq 1 ]; then
		initial_search=1;
		h2 "Contacts (initial \"$q\"):"
	else
		h2 "Contacts (exact \"$q\"):"
	fi

	while read -r line; do
		last_name=`echo "$line" | cut -d " " -f 2 | tr '[:upper:]' '[:lower:]'`; # get last_name lower cased
		if [ $initial_search -eq 1 ]; then
			last_name=`echo "$last_name" | cut -c 1`;
		fi

		if [ "$last_name" == "$q" ]; then
			found=`expr $found + 1`
			echo "$line"
		fi
	done < "$CONTACTS"

	contacts_found $found

}

function contacts() {
	local q=""; # User query
	local initial_search=0;
	local line="";
	local last_name="";
	local found=0;

	if [ ! -f "$CONTACTS" -o ! -r "$CONTACTS" ]; then
		warn "Contacts file \"$CONTACTS\" is not a file or is not readable."
		read -r -p "Give me the contacts file: " CONTACTS
		contacts
		return
	fi

	h1 "Contacts"

	while [ ${#q} -lt 1 ]; do
		echo "Input one letter for initial search and more for full search"
		echo "-a to show all, -q to quit"
		read -r q
	done

	if [ "$q" == "-q" ]; then
		return;
	fi

	contacts_search "$q"

	read # just pause this
	clear # and iterate again
	contacts
}

function calculator() {
	local option=0;

	h1 "Calculator"

	show_menu "Rectangular to polar" "Polar to rectangular" "Add / substract two rectangular numbers" "Multiply / divide two polar numbers" "Exit"
	option=$?

	clear

	case $option in
		1) calculator__rectangular_to_polar;;
		2) calculator__polar_to_rectangular;;
		3) calculator__add_rectangulars;;
		4) calculator__mul_polars;;
		5) return 0;;
		*) error "Something went wrong: \$option = $option";;
	esac

	read
	clear
	calculator
}


function calculator__rectangular_to_polar() {
	local x=0;
	local y=0;
	local r=0;
	local a=0;

	h2 "Rectangular to polar calculator"

	echo "Please introduce the real part: "
	read -r x; # Assume it's numeric, I don't want to check everything

	echo "Please introduce the imaginary part:"
	read -r y;

	r=`echo "sqrt($x^2 + $y^2)" | bc -l`;
	a=`echo "a($x / $y)" | bc -l`

	info "$x + $y * i = $r($a)";
}


function calculator__polar_to_rectangular() {
	local x=0;
	local y=0;
	local r=0;
	local a=0;

	h2 "Rectangular to polar calculator"

	echo "Please introduce the module: "
	read -r r; # Assume it's numeric, I don't want to check everything

	echo "Please introduce the angle:"
	read -r a;

	x=`echo "$r * c($a)" | bc -l`;
	y=`echo "$r * s($a)" | bc -l`

	info "$r($a) = $x + $y * i";
}

function calculator__add_rectangulars() {
	local x1=0;
	local y1=0;
	local x2=0;
	local y2=0;
	local x=0;
	local y=0;
	local operator="";
	h2 "Add / substract rectangular numbers"

	echo "First number (real part):"
	read x1;

	echo "First number (imaginary part):"
	read y1;

	echo "Second number (real part):"
	read x2;

	echo "Second number (imaginary part):"
	read y2;

	echo "What do you want to do?"
	show_menu "Add" "Substract"
	case $? in
		1) operator="+";;
		2) operator="-";;
		*) error "This shit blew up, congratulations"; exit 1;;
	esac

	x=`echo "$x1 $operator $x2" | bc -l`
	y=`echo "$y1 $operator $y2" | bc -l`

	info "$x1 + $y2 * i $operator $x2 + $y2 * i = $x + $y * i"
}

function calculator__mul_polars() {
	local r1=0;
	local a1=0;
	local r2=0;
	local a2=0;
	local r=0;
	local a=0;
	local operator="";
	local secondary_operator="";
	h2 "Multiply / divide rectangular numbers"

	echo "First number (module):"
	read r1;

	echo "First number (angle):"
	read a1;

	echo "Second number (module):"
	read r2;

	echo "Second number (angle):"
	read a2;

	echo "What do you want to do?"
	show_menu "Multiply" "Divide"
	case $? in
		1) operator="*"; secondary_operator="+";;
		2) operator="/"; secondary_operator="-";;
		*) error "This shit blew up, congratulations"; exit 1;;
	esac

	r=`echo "$r1 $operator $r2" | bc -l`
	a=`echo "$a1 $secondary_operator $a2" | bc -l`

	info "$r1($a1) $operator $r2($a2) = $r($a)"
}

function main() {
	local option=0;

	clear

	h1 "Práctica SSOO I"

	show_menu "Info" "Contacts" "Rectangular to Polar coordinates" "Exit"
	option=$?
	clear
	case $option in
		1) display_info;;
		2) auth_bridge "contacts";;
		3) calculator;;
		4) return 0;;
		*) error "Something went wrong: \$option = $option"; return 1;;
	esac

	main "$@"
}

PASSWORD="$1"
main "$@" # Not required but...
exit $?
