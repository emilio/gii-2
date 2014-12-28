#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "MySQL.hpp"
#include "macros.hpp"

#define SELF "dbstructure"
using std::cout;
using std::endl;


void program_help() {
	std::cout << "Use: "SELF" <mysql_table_name> [--user user=root] [--password pass=root]\n" << std::endl;
	exit(1);
}

int main(int argc, const char **argv) {
	const char *name = NULL,
		*user = "root",
		*pass = "root";

	int i;

	for ( i = 1; i < argc; ++i )
		if ( strcmp(argv[i], "--help") == 0 )
			program_help();
		else if ( strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--user") == 0 )
			if ( argc == i + 1 )
				FATAL_ERROR("No user specified");
			else
				user = argv[++i];
		else if ( strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--password") == 0 )
			if ( argc == i + 1 )
				FATAL_ERROR("No password specified");
			else
				pass = argv[++i];
		else if ( name == NULL )
			name = argv[i];
		else
			FATAL_ERROR("Option not recognized: \"" << argv[i] << "\"");

	if ( name == NULL )
		FATAL_ERROR("Name not provided " << std::endl << "Run with --help to see a list of options");

	cout << "Name: " << name << endl
		<< "User: " << user << endl
		<< "Pass: " << pass << endl;

	connect(name, user, pass);
}
