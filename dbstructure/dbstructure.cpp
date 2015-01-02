#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "lib/mysql/mysql.hpp"
#include "macros.hpp"

#define SELF "dbstructure"
using std::cout;
using std::endl;
/**
Next step:

struct table_schema {
	const char *table_name;
	unsigned int field_count;
	const mysql::field *fields;
};

struct reference {
	const char *table_name;
	const char *column_name;
	const char *referenced_table_name;
	const char *referenced_column_name;
};

std::map<table_name, mysql::field*> tables;
std::vector<reference> refs;

Map wit

struct columninfo {
	const char *table_name;
	const char *column_name;
};*/

void program_help() {
	std::cout << "Use: " SELF " <mysql_db_name> [--user user=root] [--password pass=root] [--server server=localhost]\n" << std::endl;
	exit(1);
}

void display_table(mysql::database& db, const char *table_name) {
	mysql::result *description;
	mysql::result *foreign_keys;

	/**
	 * With this we can get all the fields via r->fields(), with types and primary + unique keys
	 * We could use a DESCRIBE table, but it will give the results per row and we would have to create
	 *  a custom field struct... which is not needed given the amount of info we get with fields()
	 */
	std::string description_query = "SELECT * FROM `";
	description_query += table_name;
	description_query += "` LIMIT 0";

	/** With this query we get all references **from** this table */
	std::string foreign_query = "SELECT TABLE_NAME, COLUMN_NAME, REFERENCED_TABLE_NAME, REFERENCED_COLUMN_NAME "
	                            "FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE WHERE "
	                            "TABLE_SCHEMA = '";
	foreign_query += db.name();
	foreign_query += "' AND TABLE_NAME = '";
	foreign_query += table_name;
	foreign_query += "' AND REFERENCED_TABLE_SCHEMA = '";
	foreign_query += db.name();
	foreign_query += "'";

	description = db.query(description_query);

	cout << *description;

	foreign_keys = db.query(foreign_query);
	cout << *foreign_keys;

	delete description;
	delete foreign_keys;
}

void render_db_structure(mysql::database& db) {
	mysql::result *r;
	cout << "Starting rendering" << endl;
	r = db.query("SHOW TABLES");

	cout << "Field count: " << r->field_count() << endl;

	for ( mysql::result::const_iterator row = r->begin(); row != r->end(); ++row )
		display_table(db, (*row)[0]);

	delete r;
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
		render_db_structure(db);
	} catch ( std::runtime_error& e ) {
		FATAL_ERROR(e.what());
	}

	return 0;
}
