#ifndef __MYSQL_ROW_HPP
#define __MYSQL_ROW_HPP
#include "database.hpp"
#include <iostream>
namespace mysql {
class row {
	MYSQL_ROW row_;
	field *fields_;
	unsigned int field_count_;
public:
	row(MYSQL_ROW r, field *fields, unsigned int field_count) : row_(r), fields_(fields), field_count_(field_count) {}
	const char * operator[](int i) const { return row_[i]; }
	const char * operator[](const char *) const;
};
} // namespace mysql

#endif
