#ifndef __MYSQL_HPP
#define __MYSQL_HPP
#include <mysql.h>
#include "macros.hpp"

#define MYSQL_ASIGN_POINTER(var, expr) do { \
	var = (expr); \
	if ( var == NULL ) \
		FATAL_ERROR(mysql_error(conn)); \
} while (0);

#define MYSQL_EXPRESSION(expr) do { \
	if ( (expr) == NULL ) \
		FATAL_ERROR(mysql_error(conn)); \
} while (0);

class MySQL {
	MYSQL *conn;
public:
	MySQL() {
		MYSQL_ASIGN_POINTER(conn, mysql_init(NULL));
	}
	~MySQL() {
		mysql_close(conn);
	}

	bool connect(char *server, char *user, char *pass, char *db) {
		MYSQL_EXPRESSION(mysql_real_connect(conn, server, user, pass, db, 0, NULL, 0));
	}
};

#endif
