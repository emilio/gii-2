#include "database.hpp"
#include <iostream>
namespace mysql {

void database::connect(const char *server, const char *user, const char *pass, const char *db) {
	server_ = server;
	user_ = user;
	pass_ = pass;
	name_ = db;
	MYSQL_EXPRESSION(mysql_real_connect(conn_, server, user, pass, db, 0, NULL, 0));
}

void database::connect(const char *server, const char *user, const char *pass) {
	return connect(server, user, pass, NULL);
}

result * database::query(const char * q) const {
	std::clog << "(query) " << q << std::endl;
	if ( mysql_query(conn_, q) )
		MYSQL_TRHOW();
	return new result(conn_, mysql_store_result(conn_));
}

} // namespace mysql
