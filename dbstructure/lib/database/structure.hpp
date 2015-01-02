#ifndef __DATABASE_STRUCTURE_HPP
#define __DATABASE_STRUCTURE_HPP

#include "table.hpp"
#include "../mysql/mysql.hpp"
#include <vector>

namespace database {

class structure {
	const mysql::database& db_;
	std::vector<table *> tables_;
public:
	structure(const mysql::database& db);
	~structure();
	const std::vector<table *>& tables() { return tables_; }
};

} // namespace database
#endif
