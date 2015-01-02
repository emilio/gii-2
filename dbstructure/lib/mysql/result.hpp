#ifndef __MYSQL_RESULT_HPP
#define __MYSQL_RESULT_HPP
#include "mysql.hpp"
#include "row.hpp"
#include <iostream>
namespace mysql {

class result {
	unsigned int row_count_;
	unsigned int field_count_;
	MYSQL_RES *original_result_;

	field *fields_;
	row *rows_;
	row *end_;
public:
	result(MYSQL *conn, MYSQL_RES *r);
	~result();

	// typedef row * iterator; // no non-const iterator
	typedef row const * const_iterator;

	const_iterator begin() const { return rows_; }
	const_iterator end() const { return end_;	} // rows_ + row_count_
	bool empty() const { return row_count_ == 0; }

	unsigned int field_count() const { return field_count_;	}
	unsigned int row_count() const { return row_count_;	}
	const field * fields() { return fields_; }
	const row * rows() { return rows_; }
	const row& operator[](size_t index) { return rows_[index]; }
	void display(std::ostream& os) const;
};

std::ostream& operator<< (std::ostream& os, const result& r);

} // namespace mysql

#endif
