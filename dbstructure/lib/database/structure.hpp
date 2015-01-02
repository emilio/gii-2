#ifndef __DATABASE_STRUCTURE_HPP
#define __DATABASE_STRUCTURE_HPP
#include <vector>
#include <cstring>
#include "../mysql/mysql.hpp"
namespace database {
namespace {
	inline char *copy_string(const char *source, size_t len) {
		char *ret = new char[len + 1];
		strncpy(ret, source, len);
		ret[len] = '\0';
		return ret;
	}

	inline char *copy_string(const char *source) {
		return copy_string(source, strlen(source));
	}
} // namespace

struct reference {
	const char *table_name;
	const char *column_name;
	const char *referenced_table_name;
	const char *referenced_column_name;

	reference(const char *tname, const char *cname, const char *rtname, const char *rcname) {
		table_name = copy_string(tname);
		column_name = copy_string(cname);
		referenced_table_name = copy_string(rtname);
		referenced_column_name = copy_string(rcname);
	};
	~reference() {
		delete[] table_name;
		delete[] column_name;
		delete[] referenced_table_name;
		delete[] referenced_column_name;
	}
};

// reduced mysql field struct
struct field {
	char *name;
	unsigned int flags;
	enum enum_field_types type;
	// char *default;
	field() : name(NULL) {};
	field(const char *n, size_t n_len, unsigned int f, enum enum_field_types t)
	: flags(f), type(t) {
		name = copy_string(n, n_len);
	}

	field(const char *n, unsigned int f, enum enum_field_types t) {
		field(n, strlen(n), f, t);
	}

	~field() {
		if ( name != NULL )
			delete[] name;
	}
};


struct table {
	const char *name;
	std::vector<field *> fields;
	unsigned int field_count;
	std::vector<reference *> references;
	table(const char *n, const mysql::field *f, unsigned int c)
	: name(n), field_count(c) {
		fields = std::vector<field*>(field_count);

		for ( unsigned int i = 0; i < field_count; ++i )
			fields[i] = new field(f[i].name, f[i].name_length, f[i].flags, f[i].type);
	};

	~table() {
		for ( std::vector<reference *>::iterator it = references.begin(); it != references.end(); ++it )
			delete *it;

		for ( std::vector<field *>::iterator it = fields.begin(); it != fields.end(); ++it )
			delete *it;
	}
};


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
