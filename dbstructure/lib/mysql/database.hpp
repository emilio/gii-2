#ifndef __MYSQL_DATABASE_HPP
#define __MYSQL_DATABASE_HPP
#include "mysql.hpp"
#include "result.hpp"
namespace mysql {
class database {
	MYSQL *conn_;
	const char *server_;
	const char *user_;
	const char *pass_;
	const char *name_;
public:

	database() {
		MYSQL_ASIGN_POINTER(conn_, mysql_init(NULL));
	}

	~database() {
		mysql_close(conn_);
	}

	const char *server() { return server_; }
	const char *user() { return user_; }
	const char *pass() { return pass_; }
	const char *name() { return name_; }

	void connect(const char *server, const char *user, const char *pass, const char *db);
	void connect(const char *server, const char *user, const char *pass);

	result * query(const char * q) const;
	result * query(const std::string& q) const { return query(q.c_str()); }
};
} // namespace mysql
#endif
