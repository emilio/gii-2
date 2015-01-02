#include "structure.hpp"
namespace database {
namespace {

table *parse_table(const mysql::database& db, const char *table_name) {
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

	// std::cout << *description;

	table *t = new table(table_name, description->fields(), description->field_count());

	delete description;

	foreign_keys = db.query(foreign_query);

	// std::cout << *foreign_keys;

	for ( mysql::result::const_iterator it = foreign_keys->begin(); it != foreign_keys->end(); ++it )
		t->add_reference(
			new reference((*it)[0], (*it)[1], (*it)[2], (*it)[3]));

	delete foreign_keys;

	return t;
}

} // namespace

structure::structure(const mysql::database& db) : db_(db) {
	mysql::result *r;

	r = db_.query("SHOW TABLES");

	for ( mysql::result::const_iterator row = r->begin(); row != r->end(); ++row )
		tables_.push_back(parse_table(db_, (*row)[0]));

	delete r;
}

structure::~structure() {
	for ( std::vector<table *>::iterator it = tables_.begin(); it != tables_.end(); ++it )
		delete *it;
}

} // namespace database
