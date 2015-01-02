#ifndef __MYSQL_HPP
#define __MYSQL_HPP
#include <mysql/mysql.h>
#include <stdexcept>
#define MYSQL_TRHOW() throw std::runtime_error(mysql_error(conn_));
#define MYSQL_ASIGN_POINTER(var, expr) do { \
	var = (expr); \
	if ( var == NULL ) \
		MYSQL_TRHOW(); \
} while (0);

#define MYSQL_EXPRESSION(expr) do { \
	if ( (expr) == NULL ) \
		MYSQL_TRHOW(); \
} while (0);

namespace mysql {
typedef MYSQL_FIELD field;
}

#include "database.hpp"
#include "row.hpp"
#include "result.hpp"
#endif
