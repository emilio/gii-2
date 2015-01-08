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
	unsigned int i;

	original_result_ = r;
	row_count_ = mysql_num_rows(r);
	field_count_ = mysql_num_fields(r);

	rows_ = end_ = NULL;
	fields_ = NULL;

	if ( field_count_ )
		fields_ = mysql_fetch_fields(r);

	if ( row_count_ ) {
		rows_ = reinterpret_cast<row *>(new unsigned char[row_count_ * sizeof(row)]);
		for ( i = 0; i < row_count_; ++i )
			rows_[i] = row(mysql_fetch_row(r), fields_, field_count_);
		end_ = rows_ + row_count_;
	}
}

result::~result() {
	if ( row_count_ )
		delete[] rows_;


	mysql_free_result(original_result_);
}

void result::display(std::ostream& os) const {
	if ( empty() )
		return;

	display_fields(os, fields_, field_count_);

	for ( const_iterator it = begin(); it != end(); ++it )
		display_row(os, *it, fields_, field_count_);
}

std::ostream& operator<<(std::ostream& os, const result& r) {
	r.display(os);
	return os;
}

} // namespace mysql
