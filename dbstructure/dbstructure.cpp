#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "lib/mysql/mysql.hpp"
#include "lib/database/structure.hpp"
#include "macros.hpp"

#define SELF "dbstructure"
using std::cout;
using std::endl;


void program_help() {
	std::cout << "Use: " SELF " <mysql_db_name> [--user user=root] [--password pass=root] [--server server=localhost]\n" << std::endl;
	exit(1);
}

int init(mysql::database& db) {
	typedef std::vector<database::table *> table_vector;
	typedef std::vector<database::reference *> reference_vector;
	typedef std::vector<database::field *> field_vector;

	database::structure structure = database::structure(db);
	const table_vector& tables = structure.tables();

	for ( table_vector::const_iterator it = tables.begin(); it != tables.end(); ++it ) {
		const database::table *table = *it;
		const reference_vector& references = table->references();
		const field_vector& fields = table->fields();

		cout << table->name() << " (" << endl;

		for ( field_vector::const_iterator it = fields.begin(); it != fields.end(); ++it ) {
			const database::reference *ref = (*it)->ref();
			cout << "\t" << (*it)->name();
			if ( ref != NULL )
				cout << "->" << ref->referenced_table_name() << "(" << ref->referenced_column_name() << ")";

			cout << endl;
		}
		cout << ")" << endl;

		cout << endl;
	}

	return 0;
}

int main(int argc, const char **argv) {
	const char *name = NULL,
		*server = "localhost",
		*user = "root",
		*pass = "root";
	mysql::database db;

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
		else if ( strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--server") == 0 )
			if ( argc == i + 1 )
				FATAL_ERROR("No server specified");
			else
				server = argv[++i];
		else if ( name == NULL )
			name = argv[i];
		else
			FATAL_ERROR("Option not recognized: \"" << argv[i] << "\"");

	if ( name == NULL )
		FATAL_ERROR("Name not provided " << std::endl << "Run with --help to see a list of options");

	cout << "Name: " << name << endl
		<< "Server: " << server << endl
		<< "User: " << user << endl
		<< "Pass: " << pass << endl;

	try {
		db.connect(server, user, pass, name);
		return init(db);
	} catch ( std::runtime_error& e ) {
		FATAL_ERROR(e.what());
	}

	return 0;
}
