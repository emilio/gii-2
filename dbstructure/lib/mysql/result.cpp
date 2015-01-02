#include "database.hpp" // necessary to avoid problems with database#query
#include "result.hpp"
namespace mysql {

namespace {
using std::max;

void display_fields(std::ostream& os, const field* fields, unsigned int field_count) {
	std::streamsize original_w = os.width();
	os << "| ";
	for ( unsigned int i = 0; i < field_count; ++i ) {
		os.width(max<long unsigned int>(fields[i].name_length, fields[i].max_length));
		os << fields[i].name;
		os.width(original_w);
		os << " | ";
	}
	os << std::endl;
}

void display_row(std::ostream& os, const row& r, const field *fields, unsigned int field_count) {
	std::streamsize original_w = os.width();
	os << "| ";
	for ( unsigned int i = 0; i < field_count; ++i ) {
		os.width(max<long unsigned int>(fields[i].name_length, fields[i].max_length));
		if ( r[i] == NULL )
			os << "NULL";
		else
			os << r[i];
		os.width(original_w);
		os << " | ";
	}
	os << std::endl;
}

} // anonymous namespace

result::result(MYSQL *conn, MYSQL_RES *r) {
	field *original_fields;
	unsigned int i;

	original_result_ = r;
	row_count_ = mysql_num_rows(r);
	field_count_ = mysql_num_fields(r);

	rows_ = end_ = NULL;
	fields_ = NULL;

	if ( row_count_ ) {
		rows_ = new row[row_count_];
		for ( i = 0; i < row_count_; ++i )
			rows_[i] = mysql_fetch_row(r);
		end_ = rows_ + row_count_;
	}

	if ( field_count_ ) {
		fields_ = new field[field_count_];
		original_fields = mysql_fetch_fields(r);

		for ( i = 0; i < field_count_; ++i )
			fields_[i] = original_fields[i];
	}
}

result::~result() {
	if ( row_count_ )
		delete[] rows_;

	if ( field_count_ )
		delete[] fields_;

	mysql_free_result(original_result_);
}

void result::display(std::ostream& os) const {
	display_fields(os, fields_, field_count_);

	for ( const_iterator it = begin(); it != end(); ++it )
		display_row(os, *it, fields_, field_count_);
}

std::ostream& operator<<(std::ostream& os, const result& r) {
	r.display(os);
	return os;
}

} // namespace mysql
